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
 * @file    javascript_functions.cpp
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */

#include <dpl/log/log.h>
#include <dpl/scoped_array.h>
#include <js_overlay_functions.h>

namespace JSCFunctions {
std::string ConvertJSStringToStdString(JSStringRef value)
{
    int nSize = JSStringGetLength(value) + 1;
    DPL::ScopedArray<char> textStr(new char[nSize]);
    JSStringGetUTF8CString(value, textStr.Get(), nSize);
    std::string ret = textStr.Get();
    return ret;
}

JSValueRef JavaScriptPrintProc(JSContextRef context,
                               JSObjectRef /*object*/,
                               JSObjectRef /*thisObject*/,
                               size_t argumentCount,
                               const JSValueRef arguments[],
                               JSValueRef* exception)
{
    if (argumentCount == 0 || !JSValueIsString(context, arguments[0])) {
        LogError("Argument is not string");
        return JSValueMakeUndefined(context);
    }

    JSStringRef textRef = JSValueToStringCopy(context, arguments[0], exception);
    int nSize = JSStringGetLength(textRef) + 1;

    DPL::ScopedArray<char> textStr(new char[nSize]);

    JSStringGetUTF8CString(textRef, textStr.Get(), nSize);
    LogDebug("\033[00;35m[jsPrint] " << textStr.Get());

    JSStringRelease(textRef);
    return JSValueMakeBoolean(context, true);
}

JSValueRef JavaScriptHookProc(
    JSContextRef context,
    JSObjectRef /*object*/,
    JSObjectRef /*thisObject*/,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    bool inError = false;
    if (argumentCount < 2 ||
        argumentCount > 3 ||
        !JSValueIsString(context, arguments[0]) ||
        !JSValueIsString(context, arguments[1]))
    {
        inError = true;
    }

    if (inError) {
        LogError("*********************************************");
        LogError("*********************************************");
        LogError("Cannot print test Result");
        LogError("*********************************************");
        LogError("*********************************************");
        return JSValueMakeUndefined(context);
    }

    std::string id, result, message;
    JSStringRef idRef = JSValueToStringCopy(context, arguments[0], exception);
    id = ConvertJSStringToStdString(idRef);
    JSStringRelease(idRef);
    JSStringRef idResult = JSValueToStringCopy(context,
                                               arguments[1],
                                               exception);
    result = ConvertJSStringToStdString(idResult);
    JSStringRelease(idResult);

    if (argumentCount == 3 && !JSValueIsString(context, arguments[2])) {
        JSStringRef idMessage = JSValueToStringCopy(context,
                                                    arguments[0],
                                                    exception);
        message = ConvertJSStringToStdString(idMessage);
        JSStringRelease(idMessage);
    }

    LogDebug("\033[00;35m***********************************************");
    LogDebug("\033[00;35m***********************************************");
    LogDebug("\033[00;35m TEST ID: " << id);
    LogDebug("\033[00;35m RESULT: " << result);
    LogDebug("\033[00;35m MESSAGE: " << message);
    LogDebug("\033[00;35m***********************************************");
    LogDebug("\033[00;35m***********************************************");

    return JSValueMakeBoolean(context, true);
}
}
