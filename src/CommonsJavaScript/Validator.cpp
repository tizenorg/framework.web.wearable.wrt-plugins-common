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
#include "Validator.h"
#include <list>
#include <dpl/assert.h>
#include "ScopedJSStringRef.h"
#include "Converter.h"

namespace WrtDeviceApis {
namespace CommonsJavaScript {
Validator::Validator(JSContextRef context,
                     JSValueRef* exception) :
    m_context(context),
    m_exception(exception)
{
    AssertMsg(NULL != m_context, "Context cannot be NULL.");
}

Validator::~Validator()
{}

bool Validator::isDate(const JSValueRef& arg)
{
    if (JSValueIsNull(m_context,
                      arg) ||
        JSValueIsUndefined(m_context,
                           arg) || !JSValueIsObject(m_context, arg))
    {
        return false;
    }
    Converter converter(m_context);
    Try
    {
        converter.toDateTm(arg);
    }
    Catch(Commons::ConversionException)
    {
        return false;
    }
    return true;
}

bool
Validator::isCallback(const JSValueRef& arg)
{
    Converter converter(m_context);
    return !JSValueIsNull(m_context, arg) &&
           !JSValueIsUndefined(m_context, arg) &&
           JSObjectIsFunction(m_context, converter.toJSObjectRef(arg));
}

bool
Validator::checkArrayKeys(const std::vector<std::string> &allowed,
                          JSValueRef argument)
{
    if (argument == NULL) {
        return true;
    }
    if (!JSValueIsObject(m_context, argument)) {
        return false;
    }
    JSObjectRef jsOptions = JSValueToObject(m_context, argument, NULL);
    if (jsOptions == NULL) {
        return false;
    }
    JSPropertyNameArrayRef jsProps = JSObjectCopyPropertyNames(m_context,
                                                               jsOptions);
    if (jsProps == NULL) {
        // No properties found; and empty array.
        return true;
    }
    size_t nCount = JSPropertyNameArrayGetCount(jsProps);
    bool found;

    std::list<JSStringRef> allowedJS;
    for (size_t j = 0; j < allowed.size(); j++) {
        allowedJS.push_back(JSStringCreateWithUTF8CString(allowed[j].c_str()));
    }

    for (size_t i = 0; i < nCount; i++) {
        found = false;
        for (std::list<JSStringRef>::const_iterator it = allowedJS.begin();
             it != allowedJS.end();
             ++it)
        {
            if (JSStringIsEqual(*it,
                                JSPropertyNameArrayGetNameAtIndex(jsProps,
                                                                  i)))
            {
                found = true;
                break;
            }
        }
        if (!found) {
            for (size_t j = 0; j < allowed.size(); j++) {
                JSStringRelease(allowedJS.front());
                allowedJS.pop_front();
            }
            JSPropertyNameArrayRelease(jsProps);
            return false;
        }
    }

    for (size_t j = 0; j < allowed.size(); j++) {
        JSStringRelease(allowedJS.front());
        allowedJS.pop_front();
    }
    JSPropertyNameArrayRelease(jsProps);
    return true;
}

bool Validator::isNullOrUndefined(const JSValueRef& arg)
{
    return (JSValueIsNull(m_context, arg) ||
            JSValueIsUndefined(m_context, arg));
}
} // CommonsJavaScript
} // WrtDeviceApis
