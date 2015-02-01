/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
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
 *
 * @file        SoftKeyboardChangeEvent.h
 * @author      Yunchan Cho (yunchan.cho@samsung.com)
 * @version     0.1
 * @brief       softkeyboardchange event class
 */

#ifndef WRT_PLUGIN_SOFTKEYBOARD_CHANGE_EVENT_CLASS_H
#define WRT_PLUGIN_SOFTKEYBOARD_CHANGE_EVENT_CLASS_H

#include <string>
#include "ISoftKeyboardChangeEvent.h"

namespace WrtDeviceApis {
namespace SoftKeyboardChangeEvent {
namespace Api {
class SoftKeyboardChangeEvent : public ISoftKeyboardChangeEvent
{
  public:
    explicit SoftKeyboardChangeEvent(
        std::string state,
        int width,
        int height);
    ~SoftKeyboardChangeEvent();

    std::string getState() const;
    int getWidth() const;
    int getHeight() const;

  private:
    std::string m_state;
    int m_width;
    int m_height;
};
} // Api
} // SoftKeyboardChangeEvent
} // WrtDeviceApis

#endif // WRT_PLUGIN_SOFTKEYBOARD_CHANGE_EVENT_CLASS_H
