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
 * @file       dispatch_event_support.cpp
 * @author     Tae-Jeong Lee (taejeong.lee@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef _DISPATCH_EVENT_SUPPORT_CPP_
#define _DISPATCH_EVENT_SUPPORT_CPP_

#include "dispatch_event_support.h"

namespace DispatchEventSupport {

static std::string getExceptionString(JSContextRef context, JSValueRef exception)
{
    if (exception)
    {
        JSStringRef jsString = JSValueToStringCopy(context, exception, NULL);
        size_t bufSize = JSStringGetMaximumUTF8CStringSize(jsString);

        char* buf = new char[bufSize];
        JSStringGetUTF8CString(jsString, buf, bufSize);
        JSStringRelease(jsString);

        std::string ret = buf;
        delete[] buf;

        return ret;
    }
    else
    {
        return "NULL";
    }
}

void dispatchAppServiceEvent(JSContextRef context, const float scale, const std::string bundle)
{
    LogDebug("DispatchTizenServiceEvent(" << context << ", " << scale << ", " << bundle << ")");

    if (context != NULL)
    {
        std::stringstream script;

        script << "var __event = document.createEvent(\"CustomEvent\");\n"
               << "__event.initCustomEvent(\"appservice\", true, true);\n"
               << "__event.scale = " << scale << ";\n"
               << "__event.__bundle = \"" << bundle << "\";\n"
               << "document.dispatchEvent(__event);\n"
               << "\n"
               << "for (var i=0; i < window.frames.length; i++)\n"
               << "{ window.frames[i].document.dispatchEvent(__event); }";

        // just for debugging
        // LogDebug("script :\n" << script.str());

        JSValueRef exception = NULL, ret = NULL;
        JSStringRef jsScript = JSStringCreateWithUTF8CString(script.str().c_str());

        ret = JSEvaluateScript(context, jsScript, NULL, NULL, 1, &exception);

        JSStringRelease(jsScript);

        LogDebug("JSEvaluateScript() - ret:" << ret << ", exception:" << exception << " (" << getExceptionString(context, exception) << ")");
    }
}

void dispatchSoftKeyboardChangeEvent(JSContextRef context, const std::string state, const int width, const int height)
{
    LogDebug("dispatchSoftKeyboardChangeEvent(" << context << ", " << state << ", " << width << ", " << height << ")");

    if (context != NULL)
    {
        std::stringstream script;

        script << "var __event = document.createEvent(\"CustomEvent\");\n"
               << "__event.initCustomEvent(\"softkeyboardchange\", true, true);\n"
               << "__event.state = \"" << state << "\";\n"
               << "__event.width = " << width << ";\n"
               << "__event.height = " << height << ";\n"
               << "document.dispatchEvent(__event);\n"
               << "\n"
               << "for (var i=0; i < window.frames.length; i++)\n"
               << "{ window.frames[i].document.dispatchEvent(__event); }";

        // just for debugging
        // LogDebug("script :\n" << script.str());

        JSValueRef exception = NULL, ret = NULL;
        JSStringRef jsScript = JSStringCreateWithUTF8CString(script.str().c_str());

        ret = JSEvaluateScript(context, jsScript, NULL, NULL, 1, &exception);

        JSStringRelease(jsScript);

        LogDebug("JSEvaluateScript() - ret:" << ret << ", exception:" << exception << " (" << getExceptionString(context, exception) << ")");
    }
}

void dispatchStorageEvent(JSContextRef context,
                          const DPL::OptionalStdString& key,
                          const DPL::OptionalStdString& oldValue,
                          const DPL::OptionalStdString& newValue,
                          const std::string& url)
{
    LogDebug("dispatchStorageEvent(" <<
             context << ", " <<
             (!(key) ? "null" : *key) << ", " <<
             (!(oldValue) ? "null" : *oldValue) << ", " <<
             (!(newValue) ? "null" : *newValue) << ", " <<
             url << ")");

    if (context != NULL)
    {
        std::stringstream script;

        auto toJSValue =
            [](const DPL::OptionalStdString& value) -> std::string
            {
                return ((!value) ? "null" : "\"" + *value + "\"");
            };

        script << "var __event = document.createEvent(\"CustomEvent\");"
               << "__event.initCustomEvent(\"storage\", true, true);"
               << "__event.key = " << toJSValue(key) << ";"
               << "__event.oldValue = " << toJSValue(oldValue) << ";"
               << "__event.newValue = " << toJSValue(newValue) << ";"
               << "__event.url = \"" << url << "\";"
               << "__event.storageArea = widget.preferences;"
               << "document.dispatchEvent(__event);"
               << "for (var i=0; i < window.frames.length; i++)"
               << "{ window.frames[i].document.dispatchEvent(__event); }";


        // just for debugging
        // LogDebug("script :\n" << script.str());

        JSValueRef exception = NULL, ret = NULL;
        JSStringRef jsScript = JSStringCreateWithUTF8CString(script.str().c_str());

        ret = JSEvaluateScript(context, jsScript, NULL, NULL, 1, &exception);

        JSStringRelease(jsScript);

        LogDebug("JSEvaluateScript() - ret:" << ret << ", exception:" << exception << " (" << getExceptionString(context, exception) << ")");
    }
}

void dispatchHwKeyEvent(Evas_Object* ewkView, const std::string key)
{
    LogDebug("dispatchHwKeyEvent(" << ewkView << ", " << key << ")");

    if (ewkView != NULL && !key.empty())
    {
        std::stringstream script;

        script << "var __event = document.createEvent(\"CustomEvent\");\n"
               << "__event.initCustomEvent(\"tizenhwkey\", true, true);\n"
               << "__event.keyName = \"" << key << "\";\n"
               << "document.dispatchEvent(__event);\n"
               << "\n"
               << "for (var i=0; i < window.frames.length; i++)\n"
               << "{ window.frames[i].document.dispatchEvent(__event); }";

        // just for debugging
        // LogDebug("script :\n" << script.str());

        if (ewk_view_script_execute(ewkView, script.str().c_str(), NULL, NULL) != EINA_TRUE)
        {
            LogWarning("ewk_view_script_execute returned FALSE!");
        }
    }
}

void dispatchAppControlEvent(Evas_Object* ewkView)
{
    LogDebug("dispatchAppControlEvent(" << ewkView << ")");

    if (ewkView != NULL)
    {
        std::stringstream script;

        script << "var __event = document.createEvent(\"CustomEvent\");\n"
               << "__event.initCustomEvent(\"appcontrol\", true, true);\n"
               << "document.dispatchEvent(__event);\n"
               << "\n"
               << "for (var i=0; i < window.frames.length; i++)\n"
               << "{ window.frames[i].document.dispatchEvent(__event); }";

        // just for debugging
        // LogDebug("script :\n" << script.str());

        if (ewk_view_script_execute(ewkView, script.str().c_str(), NULL, NULL) != EINA_TRUE)
        {
            LogWarning("ewk_view_script_execute returned FALSE!");
        }
    }
}

}
#endif //_DISPATCH_EVENT_SUPPORT_CPP_
