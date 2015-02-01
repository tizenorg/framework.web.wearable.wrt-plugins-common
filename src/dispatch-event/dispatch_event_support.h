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
 * @file       dispatch_event_support.h
 * @author     Tae-Jeong Lee (taejeong.lee@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef _DISPATCH_EVENT_SUPPORT_H_
#define _DISPATCH_EVENT_SUPPORT_H_

#include <string>
#include <dpl/optional_typedefs.h>
#include <dpl/log/log.h>
#include <EWebKit.h>
#include <EWebKit_internal.h>
#include <JavaScriptCore/JavaScript.h>

namespace DispatchEventSupport {

void dispatchAppServiceEvent(JSContextRef context, const float scale, const std::string bundle);
void dispatchSoftKeyboardChangeEvent(JSContextRef context, const std::string state, const int width, const int height);
void dispatchStorageEvent(JSContextRef context,
                          const DPL::OptionalStdString& key,
                          const DPL::OptionalStdString& oldValue,
                          const DPL::OptionalStdString& newValue,
                          const std::string& url);
void dispatchHwKeyEvent(Evas_Object* ewkView, const std::string key);
void dispatchAppControlEvent(Evas_Object* ewkView);
}
#endif //_DISPATCH_EVENT_SUPPORT_H_
