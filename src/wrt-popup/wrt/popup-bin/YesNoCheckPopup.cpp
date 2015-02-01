/*
 * File:   YesNoCheckPopup.cpp
 * Author: pciezkowski
 *
 * Created on December 12, 2012, 9:23 AM
 */

#include "YesNoCheckPopup.h"
#include <memory>
#include <string.h>
#include "popup_manager.h"
#include "PopupSerializer.h"

namespace Wrt {
namespace Popup {
YesNoCheckPopup::~YesNoCheckPopup()
{}

Renderer::CtrlPopupPtr YesNoCheckPopup::createPopup(
    DPL::BinaryQueueAutoPtr data)
{
    std::string title = PopupSerializer::getStringArg(*data);
    std::string message = PopupSerializer::getStringArg(*data);
    std::string checkboxLabel = PopupSerializer::getStringArg(*data);
    Assert(data->Empty());
    LogDebug("title: " << title << " message: " << message << " checkbox: " <<
             checkboxLabel);
    Renderer::CtrlPopupPtr popup =
        Renderer::PopupControllerSingleton::Instance().CreatePopup();

    popup->SetTitle(title);
    popup->Append(new Renderer::PopupObject::Label(message));
    popup->Append(new Renderer::PopupObject::Check(checkboxLabel));
    popup->Append(new Renderer::PopupObject::Button(YES_LABEL, POPUP_YES_VALUE));
    popup->Append(new Renderer::PopupObject::Button(NO_LABEL, POPUP_NO_VALUE));
    return popup;
}

void YesNoCheckPopup::responseCallback(
    const Renderer::AnswerCallbackData &answer)
{
    bool result = (POPUP_YES_VALUE == answer.buttonAnswer);
    DPL::BinaryQueue retValue;
    PopupSerializer::appendArg(true, retValue);
    PopupSerializer::appendArg(result, retValue);
    LogDebug("Check state: " << answer.chackState);
    PopupSerializer::appendArg(answer.chackState, retValue);
    m_parent->response(retValue);
}
} // Popup
} // Wrt
