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
 * @file        js_function_wrapper.h
 * @author
 * @version     1.0
 * @brief
 */

#ifndef _JAVASCRIPT_OVERLAY_SUPPORT_H
#define _JAVASCRIPT_OVERLAY_SUPPORT_H

#include <string>
#include <dpl/noncopyable.h>
#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/JSObjectRef.h>
#include <Commons/JSObject.h>

namespace JSOverlaySupport {
JSObjectPtr createWrappedFunction(
    JSGlobalContextRef ctx,
    const JSObjectPtr& originalFunction,
    const JSObjectPtr& overlayFunction,
    const std::string& name);
}

#endif
