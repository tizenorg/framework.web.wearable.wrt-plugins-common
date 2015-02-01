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
 * @file    js_function_manager.cpp
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @author  Yunchan Cho (yunchan.cho@samgsung.com)
 * @version
 * @brief
 */

#include <dpl/singleton_safe_impl.h>
#include <js_function_manager.h>
#include <js_overlay_functions.h>
#include <wrt_plugin_export.h>
#include <dpl/log/secure_log.h>

IMPLEMENT_SAFE_SINGLETON(JsFunctionManager)

namespace {
const char* JSPRINT_NAME = "jsPrint";
const char* JSGLOBAL_OBJECT = "GLOBAL_OBJECT";
const char* JSHOOK_NAME = "jsHook";
}

namespace JavaScriptFunctions {
//options
class_definition_options_t jsHookfunctionsOptions = {
    JS_FUNCTION,
    CREATE_INSTANCE,
    NONE_NOTICE,
    USE_OVERLAYED,     //ignored
    NULL,
    NULL,
    reinterpret_cast<js_function_impl>(JSCFunctions::JavaScriptHookProc)
};

class_definition_options_t jsPrintfunctionsOptions = {
    JS_FUNCTION,
    CREATE_INSTANCE,
    NONE_NOTICE,
    USE_OVERLAYED,     //ignored
    NULL,
    NULL,
    reinterpret_cast<js_function_impl>(JSCFunctions::JavaScriptPrintProc)
};

js_entity_definition_t jsPrint = {
    JSGLOBAL_OBJECT,
    JSPRINT_NAME,
    "",
    NULL,
    NULL,
    &jsPrintfunctionsOptions
};

js_entity_definition_t jsHook = {
    JSGLOBAL_OBJECT,
    JSHOOK_NAME,
    "",
    NULL,
    NULL,
    &jsHookfunctionsOptions
};

const js_entity_definition_ptr_t jsPrintPtr = &jsPrint;
const js_entity_definition_ptr_t jsHookPtr = &jsHook;
}

bool JsFunctionManager::initialize()
{
    _D("JSObjectDeclaration for js functions are intialized");
    JSObjectDeclarationPtr jsPrintObj(
        new JSObjectDeclaration(JavaScriptFunctions::jsPrintPtr));

    JSObjectDeclarationPtr jsHookObj(
        new JSObjectDeclaration(JavaScriptFunctions::jsHookPtr));

    m_functions.push_back(jsPrintObj);
    m_functions.push_back(jsHookObj);

    return true;
}

JsFunctionManager::Functions JsFunctionManager::getFunctions()
{
    _D("get standard js fucntions");
    static bool initialized = initialize();
    (void) initialized;
    return m_functions;
}
