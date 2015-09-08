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
 * @file        popup-runner.cpp
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
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>

#include <dpl/log/log.h>

#include "popup-runner.h"

namespace { // anonymous
const char *POPUP_EXEC = "/usr/bin/wrt-popup-ace-runtime";

void _ace_params_serializer (const ace_param_list_t* ace_param_list,
                             Wrt::Popup::BinaryStream *stream)
{
    LogDebug("_ace_params_serializer");

    if (NULL == ace_param_list || NULL == stream) {
        LogError("*ace_param_list or *stream is NULL; return");
        return;
    }
    LogDebug("Param count : " << ace_param_list->count);

    // serialize ace_param_list->count;
    size_t count = static_cast <size_t>(ace_param_list->count);
    DPL::Serialization::Serialize(*stream, count);

    for (size_t i = 0; i < count; ++i) {
        std::string name(ace_param_list->items[i].name);
        DPL::Serialization::Serialize(*stream, name);

        std::string value(ace_param_list->items[i].value);
        DPL::Serialization::Serialize(*stream, value);
    }
}
} // anonymous namespace

namespace Wrt {
namespace Popup {
// BinaryStream class implementation
void BinaryStream::Read(size_t num, void * bytes)
{
    size_t max_size = m_data.size();
    for (size_t i = 0; i < num; ++i) {
        if (i + m_readPosition >= max_size) {
            return;
        }
        static_cast <unsigned char*>(bytes)[i] = m_data[i + m_readPosition];
    }
    m_readPosition += num;
}

void BinaryStream::Write(size_t num, const void * bytes)
{
    for (size_t i = 0; i < num; ++i) {
        m_data.push_back(static_cast <const unsigned char*>(bytes)[i]);
    }
}

BinaryStream::BinaryStream()
{
    m_readPosition = 0;
}

BinaryStream::~BinaryStream() {}

const unsigned char* BinaryStream::char_pointer() const
{
    return &m_data[0];
}

size_t BinaryStream::size() const
{
    return m_data.size();
}
// BinaryStream

ace_return_t run_popup(
    ace_popup_t popup_type,
    const ace_resource_t resource_name,
    const ace_session_id_t session_id,
    const ace_param_list_t* ace_param_list,
    ace_widget_handle_t handle,
    ace_bool_t* validation_result)
{
    if (NULL == resource_name ||
        NULL == session_id ||
        NULL == ace_param_list ||
        NULL == validation_result)
    {
        LogError("run_popup : ACE_INVALID_ARGUMENTS");
        return ACE_INVALID_ARGUMENTS;
    }
    LogDebug("popup_type    : " << popup_type);
    LogDebug("resource_name : " << resource_name);
    LogDebug("session_id    : " << session_id);
    LogDebug("widget handle : " << handle);

    int popup_type_int = static_cast <int>(popup_type);
    char *resource_name_char = static_cast <char *>(resource_name);
    char *session_id_char = static_cast <char *>(session_id);
    int handle_int = static_cast <int>(handle);

    // serialization
    Wrt::Popup::BinaryStream stream;
    DPL::Serialization::Serialize(stream, popup_type_int);

    std::string resource_name_str(resource_name_char);
    DPL::Serialization::Serialize(stream, resource_name_str);

    std::string session_id_str(session_id_char);
    DPL::Serialization::Serialize(stream, session_id_str);

    DPL::Serialization::Serialize(stream, handle_int);

    _ace_params_serializer(ace_param_list, &stream);

    int fd_send_to_child[2];
    int fd_send_to_parent[2];
    pid_t childpid;

    if (0 != pipe(fd_send_to_child)) {
        LogError("Cannot create pipes!");
        return ACE_INTERNAL_ERROR;
    }
    if (0 != pipe(fd_send_to_parent)) {
        LogError("Cannot create pipes!");
        return ACE_INTERNAL_ERROR;
    }

    if ((childpid = fork()) == -1) {
        LogError("Fork() ERROR");
        return ACE_ACE_UNKNOWN_ERROR;
    }

    if (childpid == 0) { // Child process
        LogDebug("Child");

        // read data from parent
        close(fd_send_to_child[1]);

        // send data to parent
        close(fd_send_to_parent[0]);

        std::stringstream pipe_in_buff;
        std::stringstream pipe_out_buff;
        pipe_in_buff << fd_send_to_parent[1];
        pipe_out_buff << fd_send_to_child[0];
        std::string pipe_in = pipe_in_buff.str();
        std::string pipe_out = pipe_out_buff.str();

        LogDebug(
            "Passed file descriptors: " << fd_send_to_child[0] << ", " <<
            fd_send_to_parent[1]);

        if (execl(POPUP_EXEC, POPUP_EXEC, pipe_out.c_str(), pipe_in.c_str(),
                  NULL) < 0)
        {
            LogError("execlp FAILED");
        }

        LogError("This should not happened!!!");
    } // end of child process - from now you can use DEBUG LOGS
    else { // Parent process
        LogDebug("Parent");

        int buff_size = 1024;
        char result[buff_size];
        // send data to child
        close(fd_send_to_child[0]);

        //writing to child
        LogDebug("Sending message to popup-bin process");
        if (-1 ==
            TEMP_FAILURE_RETRY(write(fd_send_to_child[1], stream.char_pointer(),
                                     stream.size())))
        {
            LogError("Write to pipe failed!");
            return ACE_INTERNAL_ERROR;
        }
        close(fd_send_to_child[1]); // cleanup
        LogDebug("Message has been sent");

        // read data from child
        close(fd_send_to_parent[1]);

        int status;
        wait(&status);
        LogDebug("STATUS EXIT ON POPUP (CHILD): " << status);
        switch (status) {
        case ACE_OK:
            LogDebug("ACE_OK");
            break;

        case ACE_INVALID_ARGUMENTS:
            LogDebug("ACE_INVALID_ARGUMENTS");
            close(fd_send_to_parent[0]);
            return static_cast <ace_return_t>(status);
            break;

        case ACE_INTERNAL_ERROR:
            LogDebug("ACE_INTERNAL_ERROR");
            close(fd_send_to_parent[0]);
            return static_cast <ace_return_t>(status);
            break;

        case ACE_ACE_UNKNOWN_ERROR:
            LogDebug("ACE_ACE_UNKNOWN_ERROR");
            close(fd_send_to_parent[0]);
            return static_cast <ace_return_t>(status);
            break;

        default:
            LogDebug("UNKNOWN_ERROR");
            close(fd_send_to_parent[0]);
            status = (int) ACE_ACE_UNKNOWN_ERROR;
            return static_cast <ace_return_t>(status);
            break;
        }

        int count;
        count = TEMP_FAILURE_RETRY(read(fd_send_to_parent[0], result, buff_size));
        close(fd_send_to_parent[0]); // cleanup


        if (0 < count) {
            BinaryStream stream_in;
            int validation_result_int;
            stream_in.Write(count, result);
            LogDebug("RESULT FROM POPUP (CHILD) : [ " << count << " ]");
            DPL::Deserialization::Deserialize(stream_in, validation_result_int);
            *validation_result = static_cast <ace_bool_t>(validation_result_int);

            LogDebug("validation_result :");
            switch (*validation_result) {
            case ACE_FALSE:
                LogDebug("ACE_FALSE");
                break;
            case ACE_TRUE:
                LogDebug("ACE_TRUE");
                break;
            default:
                LogDebug("UNKNOWN - DEFAULT");
                break;
            }
        } else {
            LogDebug("count = " << count);
            LogDebug("UNKNOWN_ERROR");
            return ACE_ACE_UNKNOWN_ERROR;
        }

        LogDebug("popup-runner: EXIT");
        return (ace_return_t) status;
    }

    LogError("This should not happend!!!");
    return ACE_ACE_UNKNOWN_ERROR;
}
} // Popup
} // Wrt
