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

#ifndef WRT_POPUP_INVOKER_H
#define WRT_POPUP_INVOKER_H

#include <string>

#include <dpl/named_input_pipe.h>
#include <dpl/named_output_pipe.h>

/*
 *
 * Example usage:
 *
 * bool result = PopupInvoker().askYesNo("title", "message");
 *
 */

namespace Wrt {
namespace Popup {
enum PopupResponse {
    NO_DO_REMEMBER,
    NO_DONT_REMEMBER,
    YES_DONT_REMEMBER,
    YES_DO_REMEMBER
};

class PopupInvoker
{
  public:
    class Exception
    {
      public:
        DECLARE_EXCEPTION_TYPE(DPL::Exception, Base)
        DECLARE_EXCEPTION_TYPE(Base, PopupInvokerException)
    };

    PopupInvoker();
    ~PopupInvoker();

    bool askYesNo(const std::string &title, const std::string &message);
    void showInfo(const std::string &title,
                  const std::string &message,
                  const std::string &buttonLabel = std::string("OK"));
    PopupResponse askYesNoCheckbox(const std::string& title,
                                   const std::string& message,
                                   const std::string& checkboxLabel);

  private:

    void executePopup();

    DPL::NamedInputPipe m_input;
    DPL::NamedOutputPipe m_output;
    std::string m_inputName;
    std::string m_outputName;
};
} // Popup
} // Wrt

#endif
