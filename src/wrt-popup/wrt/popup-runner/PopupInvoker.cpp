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

#include "PopupInvoker.h"
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <dpl/log/log.h>
#include <dpl/waitable_handle.h>
#include <dpl/binary_queue.h>
#include <dpl/serialization.h>
#include <dpl/exception.h>
#include <unistd.h>
#include <stdlib.h>
#include "PopupEnum.h"
#include "PopupSerializer.h"

namespace {
const char *POPUP_EXEC = "/usr/bin/wrt-popup-wrt-runtime";
}

namespace Wrt {
namespace Popup {
PopupInvoker::PopupInvoker()
{
    char tmp[L_tmpnam + 1];
    if (NULL == tmpnam(tmp))
        ThrowMsg(DPL::Exception, "Failed to get pipe name");
    m_inputName = tmp;

    if (NULL == tmpnam(tmp))
        ThrowMsg(DPL::Exception, "Failed to get pipe name");
    m_outputName = tmp;

    m_input.Create(m_inputName);
    m_output.Create(m_outputName);
    LogDebug("Pipes created");
}

PopupInvoker::~PopupInvoker()
{
    Try
    {
        m_input.Destroy(m_inputName);
        m_output.Destroy(m_outputName);
        LogDebug("Pipes destroyed");
    }
    Catch(DPL::Exception)
    {
        LogError("Cannot destroy pipes");
    }
}

bool PopupInvoker::askYesNo(const std::string& title,
                            const std::string& message)
{
    Try
    {
        DPL::BinaryQueue data;
        PopupSerializer::appendArg(YES_NO_PROMPT, data);
        PopupSerializer::appendArg(title, data);
        PopupSerializer::appendArg(message, data);
        DPL::NamedInputPipe tmp;
        tmp.Open(m_outputName);
        m_output.Open(m_outputName);
        m_input.Open(m_inputName);
        m_output.Write(data, data.Size());

        executePopup();

        //Result from popup application is available. Read it.
        DPL::BinaryQueueAutoPtr resultData =
            m_input.Read(std::numeric_limits<std::size_t>::max());
        const int success = PopupSerializer::getIntArg(*resultData);
        bool retVal = false;
        if (success) {
            const int result = PopupSerializer::getIntArg(*resultData);

            LogDebug("Popup result is: " << result);

            Assert(resultData->Empty());
            retVal = (!!result);
        } else {
            LogWarning("Failed to show popup.");
        }

        tmp.Close();
        m_input.Close();
        m_output.Close();

        return retVal;
    }
    Catch(DPL::Exception)
    {
        LogError("error occured");
    }

    return false;
}

void PopupInvoker::showInfo(const std::string& title,
                            const std::string& message,
                            const std::string& buttonLabel)
{
    Try
    {
        DPL::BinaryQueue data;
        PopupSerializer::appendArg(INFO_PROMPT, data);
        PopupSerializer::appendArg(title, data);
        PopupSerializer::appendArg(message, data);
        PopupSerializer::appendArg(buttonLabel, data);
        DPL::NamedInputPipe tmp;
        tmp.Open(m_outputName);
        m_output.Open(m_outputName);
        m_input.Open(m_inputName);
        m_output.Write(data, data.Size());

        executePopup();
        DPL::BinaryQueueAutoPtr resultData =
            m_input.Read(std::numeric_limits<std::size_t>::max());
        const int success = PopupSerializer::getIntArg(*resultData);
        if (!success) {
            LogWarning("Failed to show popup.");
        }
        //ignore result

        tmp.Close();
        m_input.Close();
        m_output.Close();
    }
    Catch(DPL::Exception)
    {
        LogError("error occured");
    }
}

PopupResponse PopupInvoker::askYesNoCheckbox(const std::string& title,
                                             const std::string& message,
                                             const std::string& checkboxLabel)
{
    Try
    {
        DPL::BinaryQueue data;
        PopupSerializer::appendArg(YES_NO_CHECK_PROMPT, data);
        PopupSerializer::appendArg(title, data);
        PopupSerializer::appendArg(message, data);
        PopupSerializer::appendArg(checkboxLabel, data);
        DPL::NamedInputPipe tmp;
        tmp.Open(m_outputName);
        m_output.Open(m_outputName);
        m_input.Open(m_inputName);
        m_output.Write(data, data.Size());

        executePopup();

        //Result from popup application is available. Read it.
        DPL::BinaryQueueAutoPtr resultData =
            m_input.Read(std::numeric_limits<std::size_t>::max());
        const int success = PopupSerializer::getIntArg(*resultData);
        if (success) {
            const int result = PopupSerializer::getIntArg(*resultData);
            const int rememberResult = PopupSerializer::getIntArg(*resultData);

            LogDebug(
                "Popup result is: " << result << " remeber: " << rememberResult);

            Assert(resultData->Empty());
            tmp.Close();
            m_input.Close();
            m_output.Close();

            if (1 == result) {
                if (rememberResult == 1) {
                    return YES_DO_REMEMBER;
                } else {
                    return YES_DONT_REMEMBER;
                }
            } else {
                if (rememberResult == 1) {
                    return NO_DO_REMEMBER;
                } else {
                    return NO_DONT_REMEMBER;
                }
            }
        } else {
            LogWarning("Popup failed to execute.");
            tmp.Close();
            m_input.Close();
            m_output.Close();
            return NO_DONT_REMEMBER;
        }
    }
    Catch(DPL::Exception)
    {
        LogError("error occured");
    }
    return NO_DONT_REMEMBER;
}

void PopupInvoker::executePopup()
{
    pid_t pid = fork();
    if (pid == -1) {
        //error occured
        LogError("Failed to create popup process.");
        Assert(false);
    }
    if (pid == 0) {
        //child process
        int ret = execl(POPUP_EXEC,
                        POPUP_EXEC,
                        m_outputName.c_str(),
                        m_inputName.c_str(),
                        NULL);
        if (ret == -1) {
            //execl returns -1 on error
            LogError("Failed to set popup binary");
            //write something to pipe to unblock caller process
            DPL::NamedOutputPipe errOut;
            errOut.Open(m_inputName);
            DPL::BinaryQueue data;
            PopupSerializer::appendArg(false, data);
            errOut.Write(data, data.Size());
            errOut.Close();

            Assert(false);
        }
    }

    DPL::WaitableHandle handle = m_input.WaitableReadHandle();
    DPL::WaitForSingleHandle(handle);
}
} // Popup
} // Wrt
