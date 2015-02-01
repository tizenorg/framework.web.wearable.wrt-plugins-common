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
 * @file        InfoPopup.cpp
 * @author      Lukasz Marek(l.marek@samsung.com)
 * @version     1.0
 * @brief       Popup that displays information. Inplementation
 */

#include "InfoPopup.h"
#include <memory>
#include <string.h>
#include "popup_manager.h"
#include "popup.h"
#include "PopupSerializer.h"

namespace Wrt {
namespace Popup {
InfoPopup::InfoPopup() :
        m_parent(NULL)
{}

void InfoPopup::show(DPL::BinaryQueueAutoPtr data, WrtPopup* parent)
{
    LogDebug("Entered");
    std::string title = PopupSerializer::getStringArg(*data);
    std::string message = PopupSerializer::getStringArg(*data);
    std::string button = PopupSerializer::getStringArg(*data);
    Assert(data->Empty());
    LogDebug("title: " << title << " message: " << message);

    m_parent = parent;

    Renderer::CtrlPopupPtr popup =
        Renderer::PopupControllerSingleton::Instance().CreatePopup();

    popup->SetTitle(title);
    popup->Append(new Renderer::PopupObject::Label(message));

    popup->Append(new Renderer::PopupObject::Button(button, 0));

    ListenForAnswer(popup);

    Renderer::ShowPopupEventShort event(popup,
                                        MakeAnswerCallback(
                                            this,
                                            &InfoPopup::responseCallback));

    CONTROLLER_POST_EVENT(Renderer::PopupController,
                          event);

    LogDebug("Exited");
    return;
}

void InfoPopup::responseCallback(const Renderer::AnswerCallbackData &answer)
{
    DPL::BinaryQueue retValue;
    PopupSerializer::appendArg(true, retValue);
    PopupSerializer::appendArg(answer.buttonAnswer, retValue);
    m_parent->response(retValue);
}
} // Popup
} // Wrt
