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
 * @file        Popup.h
 * @author      Janusz Kozerski (j.kozerski@samsung.com)
 * @version     1.0
 */

#include <Elementary.h>
#include <ace_api_common.h>

struct ace_popup_data {
    ace_popup_t popup_type;
    ace_resource_t resource_name;
    ace_session_id_t session_id;
    ace_param_list_t param_list;
    ace_widget_handle_t handle;
    ace_bool_t validation_result;
    ace_return_t validation_return;

    Evas_Object          *popup;
    Eina_Bool per_session;
    Eina_Bool always;

    ace_validity_t remember_choice;
};
