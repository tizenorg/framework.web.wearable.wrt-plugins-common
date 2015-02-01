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
 * @file        js_types.h
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version     1.0
 */

#ifndef WRT_PLUGIN_COMMON_API_SUPPORT_JS_TYPES_H_
#define WRT_PLUGIN_COMMON_API_SUPPORT_JS_TYPES_H_

#include <string>

//forward declaration
extern "C" {
typedef struct OpaqueJSContext* JSGlobalContextRef;
typedef struct OpaqueJSValue* JSObjectRef;
}

namespace WrtPluginsApi {
struct JavaScriptObject
{
    JSObjectRef instance;
    std::string name;
};
}

#endif
