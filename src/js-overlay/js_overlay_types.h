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
 *
 * @file       js_overlay_types.h
 * @author     Yunchan Cho (yunchan.cho@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef _WRT_PLUGINS_JS_OVERLAY_TYPES_H_
#define _WRT_PLUGINS_JS_OVERLAY_TYPES_H_

#include <string>

namespace WrtPlugins {
namespace W3C {
// If needed, enum for new custom event should be defined here
enum CustomEventType {
    StorageCustomEvent,
    ServiceCustomEvent,
    SoftKeyboardChangeCustomEvent,
};

// Argument structure of SoftKeyboardChangeCustomEvent
typedef struct SoftKeyboardChangeArgs {
    std::string state; // value is 'on' or 'off'
    int width;
    int height;
    SoftKeyboardChangeArgs() :
        width(0),
        height(0)
    {}
} SoftKeyboardChangeArgs;

// If needed, argument structure of other custom events should be defined here
} // W3C
} // WrtPlugins
#endif // _WRT_PLUGINS_JS_OVERLAY_TYPES_H_
