/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
/*
 * @file        Popup.c
 * @author      Janusz Kozerski (j.kozerski@samsung.com)
 * @version     1.0
 */

/*
 * This is just a example pop-up that shows how to use a new C-API in
 * wrt-security.
 * This pop-up should be re-implemented by members of NGWAP.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <ctime>

#include <Elementary.h>

#include <dpl/log/log.h>
#include <dpl/serialization.h>
#include <ace_api_common.h>
#include <ace_api_popup_validation.h>

#include "popup-runner.h"
#include "Popup.h"

namespace { // anonymous
static void set_validity_from_checkbox(struct ace_popup_data *pdp)
{
    if (NULL == pdp) {
        LogError("pdp is NULL; return");
        return;
    }

    if (pdp->always) {
        LogDebug("Remember ALWAYS");
        pdp->remember_choice = ACE_ALWAYS;
        return;
    }
    if (pdp->per_session) {
        LogDebug("Remember PER_SESSION");
        pdp->remember_choice = ACE_PER_SESSION;
        return;
    }
    LogDebug("Remember ONCE");
    pdp->remember_choice = ACE_ONCE;
    return;
}

static void on_done(void)
{
    // Quit the efl-mainloop
    LogDebug("elm_exit()");
    elm_exit();
}

static void grant_answer(void *data,
                         Evas_Object * /* obj */,
                         void * /* event_info */)
{
    LogDebug("grant_answer");
    if (NULL == data) {
        LogError("data is NULL; return");
        return;
    }
    struct ace_popup_data *pdp = static_cast <struct ace_popup_data *>(data);

    set_validity_from_checkbox(pdp);

    ace_popup_validation_initialize();
    pdp->validation_return = ace_validate_answer(
            ACE_TRUE,
            pdp->remember_choice,
            pdp->resource_name,
            pdp->session_id,
            &(pdp->param_list),
            pdp->handle,
            &(pdp->validation_result));
    ace_popup_validation_shutdown();

    on_done();
}

static void deny_answer(void *data,
                        Evas_Object * /* obj */,
                        void * /* event_info */)
{
    LogDebug("deny_answer");
    if (NULL == data) {
        LogError("data is NULL; return");
        return;
    }
    struct ace_popup_data *pdp = static_cast <struct ace_popup_data *>(data);

    set_validity_from_checkbox(pdp);

    ace_popup_validation_initialize();
    pdp->validation_return = ace_validate_answer(
            ACE_FALSE,
            pdp->remember_choice,
            pdp->resource_name,
            pdp->session_id,
            &(pdp->param_list),
            pdp->handle,
            &(pdp->validation_result));
    ace_popup_validation_shutdown();

    on_done();
}

static int _ace_params_deserializer(ace_param_list_t* ace_param_list,
                                    Wrt::Popup::BinaryStream *stream)
{
    LogDebug("_ace_params_deserializer");

    if (NULL == ace_param_list || NULL == stream) {
        LogError("ace_param_list or stream is NULL; return -1");
        return -1;
    }

    // deserialize ace_param_list->count;
    size_t count = 0;
    DPL::Deserialization::Deserialize(*stream, count);
    ace_param_list->count = count;
    LogDebug("count : " << count);

    std::string name;
    std::string value;
    if (count == 0) {
        ace_param_list->items = NULL;
        return 0;
    }
    ace_param_list->items =
        static_cast <ace_param_t *>(malloc(count * sizeof(ace_param_t)));
    memset(ace_param_list->items, 0, count * sizeof(ace_param_t));

    for (size_t i = 0; i < count; ++i) {
        DPL::Deserialization::Deserialize(*stream, name);
        ace_param_list->items[i].name =
            strdup(const_cast <char *>(name.c_str()));

        DPL::Deserialization::Deserialize(*stream, value);
        ace_param_list->items[i].value =
            strdup(const_cast <char *>(value.c_str()));
    }
    return 0;
}

static void _ace_params_finalize(ace_param_list_t* ace_param_list)
{
    if (!ace_param_list || !ace_param_list->items) {
        LogDebug("List is null, nothing to do");
        return;
    }
    for (size_t i = 0; i < ace_param_list->count; ++i) {
        free(ace_param_list->items[i].name);
        free(ace_param_list->items[i].value);
    }
    free(ace_param_list->items);
    ace_param_list->items = NULL;
    ace_param_list->count = 0;
}

static void show_popup(struct ace_popup_data *pdp)
{
    LogDebug("show_popup()");

    if (NULL == pdp) {
        LogError("pdp is NULL; return");
        return;
    }

    const char *resource_type = static_cast <char *>(pdp->resource_name);

    Evas_Object *win = NULL;
    Evas_Object *box = NULL;
    Evas_Object *label = NULL;
    Evas_Object *grant_button = NULL;
    Evas_Object *deny_button = NULL;
    pdp->per_session = EINA_FALSE;
    pdp->always = EINA_FALSE;

    win = elm_win_add(NULL, "The wrt-client popup", ELM_WIN_NOTIFICATION);
    elm_win_autodel_set(win, EINA_TRUE);
    elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
    evas_object_show(win);
    elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_TRANSLUCENT);

    pdp->popup = elm_popup_add(win);

    box = elm_box_add(pdp->popup);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0);

    label = elm_label_add(pdp->popup);
    elm_object_style_set(label, "popup_description/default");
    elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
    char *buff = NULL;
    if (-1 ==
        asprintf(&buff, "Application need an access to %s.<br>Grant or deny?",
                 resource_type))
    {
        LogError("asprintf failed - returned -1");
        evas_object_del(label);
        evas_object_del(pdp->popup);
        evas_object_del(win);
        return;
    }
    elm_object_text_set(label, buff);
    free(buff);
    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(label);
    elm_box_pack_end(box, label);

    LogDebug("popup_type == " << pdp->popup_type);
    if (pdp->popup_type == ACE_SESSION || pdp->popup_type == ACE_BLANKET) {
        LogDebug("popup_type == ACE_SESSION || ACE_BLANKET");
        Evas_Object *cb_session = elm_check_add(pdp->popup);
        elm_object_text_set(cb_session, "Remember choice for this session");
        elm_check_state_pointer_set(cb_session, &(pdp->per_session));
        evas_object_smart_callback_add(cb_session, "changed", NULL, NULL);
        evas_object_size_hint_align_set(cb_session,
                                        EVAS_HINT_FILL,
                                        EVAS_HINT_FILL);
        evas_object_size_hint_weight_set(cb_session,
                                         EVAS_HINT_EXPAND,
                                         EVAS_HINT_EXPAND);
        evas_object_show(cb_session);
        elm_box_pack_end(box, cb_session);

        if (pdp->popup_type == ACE_BLANKET) {
            LogDebug("popup_type == ACE_BLANKET");
            Evas_Object *cb_always = elm_check_add(pdp->popup);
            elm_object_text_set(cb_always, "Remember choice forever");
            elm_check_state_pointer_set(cb_always, &(pdp->always));
            evas_object_smart_callback_add(cb_always, "changed", NULL, NULL);
            evas_object_size_hint_align_set(cb_always,
                                            EVAS_HINT_FILL,
                                            EVAS_HINT_FILL);
            evas_object_size_hint_weight_set(cb_always,
                                             EVAS_HINT_EXPAND,
                                             EVAS_HINT_EXPAND);
            evas_object_show(cb_always);
            elm_box_pack_end(box, cb_always);
        }
    }

    elm_object_part_content_set(pdp->popup, "default", box);

    grant_button = elm_button_add(pdp->popup);
    elm_object_style_set(grant_button, "popup");
    elm_object_text_set(grant_button, "Grant");
    elm_object_part_content_set(pdp->popup, "button1", grant_button);
    evas_object_smart_callback_add(grant_button, "clicked", grant_answer, pdp);

    deny_button = elm_button_add(pdp->popup);
    elm_object_style_set(deny_button, "popup");
    elm_object_text_set(deny_button, "Deny");
    elm_object_part_content_set(pdp->popup, "button2", deny_button);
    evas_object_smart_callback_add(deny_button, "clicked", deny_answer, pdp);

    evas_object_show(pdp->popup);

    // Showing the popup window
    evas_object_show(win);

    // Run the efl mainloop
    elm_run();
}
} // anonymous

EAPI_MAIN int
elm_main(int argc, char ** argv)
{
    UNHANDLED_EXCEPTION_HANDLER_BEGIN
    {
        // int pipe_in and int pipe_out should be passed to Popup via args.

        //  These parameters should be passed to Popup via pipe:
        //  ace_popup_t             popup_type
        //  const ace_resource_t    resource_name
        //  const ace_session_id_t  session_id
        //  ace_widget_handle_t     handle
        //  const ace_param_list_t  param_list

        try {
            DPL::Log::LogSystemSingleton::Instance().SetTag("WRT-POPUP-BIN");
        } Catch(DPL::Exception) {
            //cannot run logger
            return ACE_INTERNAL_ERROR;
        }
        LogDebug(
            "############################ popup binary ################################");

        if (argc < 3) {
            LogError(
                "To few args passed in exec to popup-bin - should be at least 3:");
            LogError("(binary-name, pipe_in, pipe_out)");
            LogError("return ACE_INTERNAL_ERROR");
            return ACE_INTERNAL_ERROR;
        }
        LogDebug("Passed args: " << argv[0] << ", " << argv[1] << ", " <<
                 argv[2]);

        int pipe_in;
        int pipe_out;
        std::stringstream parsing_stream;

        // Parsing args (pipe_in, pipe_out)
        parsing_stream.str(argv[1]);
        parsing_stream >> pipe_in;
        if ( parsing_stream.fail() ) {
            LogError("Error while parsing pipe_in; return ACE_INTERNAL_ERROR");
            return ACE_INTERNAL_ERROR;
        }
        parsing_stream.clear();
        parsing_stream.str(argv[2]);
        parsing_stream >> pipe_out;
        if ( parsing_stream.fail() ) {
            LogError("Error while parsing pipe_out; return ACE_INTERNAL_ERROR");
            return ACE_INTERNAL_ERROR;
        }
        parsing_stream.clear();
        LogDebug("Parsed pipes: IN: " << pipe_in << ", OUT: " << pipe_out);

        int buff_size = 1024;
        char line[buff_size];

        struct ace_popup_data pd;
        struct ace_popup_data *pdp = &pd;

        pdp->popup = NULL;

        ssize_t count = 0;
        time_t time_start;
        time(&time_start); // now
        double timeout = 10.0;
        // try to read parameters from pipe_in
        // timeout is set for 10 seconds
        do {
            count = TEMP_FAILURE_RETRY(read(pipe_in, line, buff_size));
            if (timeout < difftime(time(NULL), time_start) ) {
                LogError("Timeout reached! Exit popup - ACE_INTERNAL_ERROR");
                close(pipe_in);
                close(pipe_out);
                return ACE_INTERNAL_ERROR;
            }
        } while (0 == count);
        if (count < 0) {
            close(pipe_in);
            close(pipe_out);
            LogError("read returned a negative value (" << count << ")");
            LogError("errno: " << strerror(errno) );
            LogError("Exit popup - ACE_INTERNAL_ERROR");
            return ACE_INTERNAL_ERROR;
        }
        LogDebug("Read bytes : " << count << " (in " <<
                 difftime(time_start, time(NULL)) << " seconds)");
        close(pipe_in); // cleanup

        Wrt::Popup::BinaryStream stream;
        stream.Write(count, static_cast <void *>(line));
        int popup_type = ACE_ONESHOT;
        std::string resource_name_str;
        std::string session_id_str;
        int handle = 0;

        LogDebug("------- Deserialization -------");
        // Deserialization order:
        // popup_type, resource_name, session_id, handle, param_list

        DPL::Deserialization::Deserialize(stream, popup_type);
        LogDebug("popup_type_int : " << popup_type);
        pdp->popup_type = static_cast <ace_popup_t>(popup_type);

        DPL::Deserialization::Deserialize(stream, resource_name_str);
        LogDebug("resource_name_char : " << resource_name_str.c_str());
        pdp->resource_name =
            strdup(const_cast <ace_resource_t>(resource_name_str.c_str()));

        DPL::Deserialization::Deserialize(stream, session_id_str);
        LogDebug("session_id_char : " << session_id_str.c_str());
        pdp->session_id =
            strdup(const_cast <ace_session_id_t>(session_id_str.c_str()));

        DPL::Deserialization::Deserialize(stream, handle);
        LogDebug("handle_int : " << handle);
        pdp->handle = static_cast <ace_widget_handle_t>(handle);

        if (_ace_params_deserializer(&(pdp->param_list), &stream)) {
            return ACE_INTERNAL_ERROR;
        }

        pdp->per_session = EINA_FALSE;
        pdp->always = EINA_FALSE;
        pdp->validation_return = ACE_ACE_UNKNOWN_ERROR;

        show_popup(pdp); // Showing popup

        // sending validation_result to popup-runner
        Wrt::Popup::BinaryStream stream_out;

        LogDebug("pdp->validation_result : " << pdp->validation_result);
        int validation_result_int = (int) pdp->validation_result;
        LogDebug("validation_result_int : " << validation_result_int);
        DPL::Serialization::Serialize(stream_out, validation_result_int);
        if (-1 ==
            TEMP_FAILURE_RETRY(write(pipe_out, stream_out.char_pointer(),
                                     stream_out.size())))
        {
            LogError("Write to pipe failed!");
            close(pipe_out);
            return ACE_INTERNAL_ERROR;
        }
        close(pipe_out);

        if (pdp->validation_return == ACE_OK) {
            LogDebug("ACE_OK");
        } else if (pdp->validation_return == ACE_INVALID_ARGUMENTS) {
            LogError("ACE_INVALID_ARGUMENTS");
        } else if (pdp->validation_return == ACE_INTERNAL_ERROR) {
            LogError("ACE_INTERNAL_ERROR");
        } else if (pdp->validation_return == ACE_ACE_UNKNOWN_ERROR) {
            LogError("ACE_ACE_UNKNOWN_ERROR");
        } else {
            LogError("Really unknown error!!!");
        }

        LogDebug(
            "############################ /popup binary ################################");

        // Shutdown elementary
        //LogDebug("elm_shutdown()");
        //elm_shutdown();
        // This is commented because, it causes that popup exits with 139 code
        // (Segmentatation violation).
        // Not calling elm_shutdown() should not have any negative consequences
        // because this binary ends
        // in next line, and system should clear the memory.

        _ace_params_finalize(&(pdp->param_list));
        return pdp->validation_return;
    }
    UNHANDLED_EXCEPTION_HANDLER_END
}
ELM_MAIN()
