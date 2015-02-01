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
/**
 * @file        YesNoPopup.cpp
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     1.0
 * @brief       Popup that contains 'Yes' and 'No' buttons. Inplementation
 */

#include "YesNoPopup.h"
#include <memory>
#include <string.h>
#include "popup_manager.h"
#include "PopupSerializer.h"

namespace Wrt {
namespace Popup {
void YesNoPopup::show(DPL::BinaryQueueAutoPtr data, WrtPopup* parent)
{
    LogDebug("Entered");
    m_parent = parent;
    Renderer::CtrlPopupPtr popup = createPopup(data);
    ListenForAnswer(popup);

    Renderer::ShowPopupEventShort event(popup,
                                        MakeAnswerCallback(
                                            this,
                                            &YesNoPopup::responseCallback));

    CONTROLLER_POST_EVENT(Renderer::PopupController,
                          event);

    LogDebug("Exited");
    return;
}

void YesNoPopup::responseCallback(const Renderer::AnswerCallbackData &answer)
{
    bool result = (POPUP_YES_VALUE == answer.buttonAnswer);
    DPL::BinaryQueue retValue;
    PopupSerializer::appendArg(true, retValue);
    PopupSerializer::appendArg(result, retValue);
    m_parent->response(retValue);
}

YesNoPopup::~YesNoPopup()
{}

Renderer::CtrlPopupPtr YesNoPopup::createPopup(DPL::BinaryQueueAutoPtr data)
{
    std::string title = PopupSerializer::getStringArg(*data);
    std::string message = PopupSerializer::getStringArg(*data);
    Assert(data->Empty());
    LogDebug("title: " << title << " message: " << message);
    Renderer::CtrlPopupPtr popup =
        Renderer::PopupControllerSingleton::Instance().CreatePopup();

    popup->SetTitle(title);
    popup->Append(new Renderer::PopupObject::Label(message));

    popup->Append(new Renderer::PopupObject::Button(YES_LABEL, POPUP_YES_VALUE));
    popup->Append(new Renderer::PopupObject::Button(NO_LABEL, POPUP_NO_VALUE));
    return popup;
}
} // Popup
} // Wrt
