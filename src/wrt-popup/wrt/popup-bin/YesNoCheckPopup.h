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
 * @file        YesNoCheckPopup.cpp
 * @author      Przmeyslaw Ciezkowsko (p.ciezkowski@samsung.com)
 * @version     1.0
 * @brief       Popup that contains 'Yes' and 'No' buttons and checkbox.
 * Inplementation
 */

#include "YesNoPopup.h"
#include "wrt-popup.h"

#ifndef WRT_YES_NO_CHECK_POPUP_H
#define WRT_YES_NO_CHECK_POPUP_H

namespace Wrt {
namespace Popup {
class YesNoCheckPopup : public YesNoPopup
{
  public:
    virtual ~YesNoCheckPopup();

  protected:
    virtual Renderer::CtrlPopupPtr createPopup(DPL::BinaryQueueAutoPtr data);
    virtual void responseCallback(const Renderer::AnswerCallbackData &answer);
};
} // Popup
} // Wrt

#endif /* WRT_YES_NO_CHECK_POPUP_H */

