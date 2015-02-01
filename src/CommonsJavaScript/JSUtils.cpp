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
#include "Converter.h"
#include "ScopedJSStringRef.h"
#include "JSUtils.h"

namespace WrtDeviceApis {
namespace CommonsJavaScript {
using namespace WrtDeviceApis::Commons;

bool JSUtils::hasProperty(JSStaticValue* properties,
                          JSStringRef name)
{
    JSStaticValue* property = properties;
    while (property->name) {
        if (JSStringIsEqualToUTF8CString(name, property->name)) {
            return true;
        }
        ++property;
    }
    return false;
}

JSValueRef JSUtils::getJSProperty(JSContextRef context,
                                  JSValueRef jsValue,
                                  const std::string &name,
                                  JSValueRef *exception)
{
    ScopedJSStringRef jsPropName(JSStringCreateWithUTF8CString(name.c_str()));
    Converter converter(context);
    JSObjectRef jsObject = converter.toJSObjectRef(jsValue);
    if (JSObjectHasProperty(context, jsObject, jsPropName.get())) {
        return JSObjectGetProperty(context, jsObject,
                                   jsPropName.get(), exception);
    }
    return NULL;
}

JSValueRef JSUtils::getJSProperty(JSContextRef context,
                                  JSObjectRef object,
                                  const std::string& name)
{
    Converter converter(context);
    Try {
        ScopedJSStringRef propName(converter.toJSStringRef(name));
        if (JSObjectHasProperty(context, object, propName.get())) {
            JSValueRef result = JSObjectGetProperty(context,
                                                    object,
                                                    propName.get(),
                                                    NULL);
            if (!JSValueIsUndefined(context, result)) {
                return result;
            }
        }
    }
    Catch(ConversionException) {}
    return NULL;
}

JSValueRef JSUtils::getJSPropertyOrUndefined(JSContextRef context,
                                             JSObjectRef object,
                                             const std::string& name)
{
    Converter converter(context);
    Try {
        ScopedJSStringRef propName(converter.toJSStringRef(name));
        if (JSObjectHasProperty(context, object, propName.get())) {
            return JSObjectGetProperty(context, object, propName.get(), NULL);
        }
    }
    Catch(ConversionException) {}
    return JSValueMakeUndefined(context);
}
} // CommonsJavaScript
} // WrtDeviceApis
