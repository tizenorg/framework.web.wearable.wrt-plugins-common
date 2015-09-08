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
 * @file        JSSoftKeyboardChangeEvent.cpp
 * @author      Yunchan Cho (yunchan.cho@samsung.com)
 * @version     0.1
 */

#include "JSSoftKeyboardChangeEvent.h"
#include <dpl/log/log.h>
#include <dpl/assert.h>
#include <SoftKeyboardChangeEvent/ISoftKeyboardChangeEvent.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::SoftKeyboardChangeEvent::Api;

#define WIDGET_PLUGIN_NAME              "SoftKeyboardChangeEvent"
#define SOFTKEYBOARD_CHANGE_EVENT_PROPERTY_STATE   "state"
#define SOFTKEYBOARD_CHANGE_EVENT_PROPERTY_WIDTH   "width"
#define SOFTKEYBOARD_CHANGE_EVENT_PROPERTY_HEIGHT  "height"

#define CATCH_EXCEPTION_CONVERSION \
    Catch(Commons::ConversionException) { \
        LogError("Error on conversion"); \
        return JSDOMExceptionFactory:: \
                   UnknownException.make(context, exception); \
    }

#define CATCH_EXCEPTION_NULL_PTR \
    Catch(Commons::NullPointerException) { \
        LogError("Error on pointer, null value"); \
        return JSDOMExceptionFactory:: \
                   UnknownException.make(context, exception); \
    }

#define CATCH_EXCEPTION_PLATFORM_ERROR \
    Catch(Commons::PlatformException){ \
        LogError("PlatformException occured"); \
        return JSDOMExceptionFactory:: \
                   UnknownException.make(context, exception); \
    }

namespace WrtPlugins {
namespace Tizen {
JSClassDefinition JSSoftKeyboardChangeEvent::m_classInfo = {
    0,
    kJSClassAttributeNone,
    WIDGET_PLUGIN_NAME,
    0,
    m_property,
    NULL,
    initialize,
    finalize,
    NULL, //HasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL, //ConvertToType,
};

JSStaticValue JSSoftKeyboardChangeEvent::m_property[] = {
    { SOFTKEYBOARD_CHANGE_EVENT_PROPERTY_STATE,
      JSSoftKeyboardChangeEvent::getState,
      0, kJSPropertyAttributeReadOnly },
    { SOFTKEYBOARD_CHANGE_EVENT_PROPERTY_WIDTH,
      JSSoftKeyboardChangeEvent::getWidth,
      0, kJSPropertyAttributeReadOnly },
    { SOFTKEYBOARD_CHANGE_EVENT_PROPERTY_HEIGHT,
      JSSoftKeyboardChangeEvent::getHeight,
      0, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSSoftKeyboardChangeEvent::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSoftKeyboardChangeEvent::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSoftKeyboardChangeEvent::m_jsClassRef = JSClassCreate(
        JSSoftKeyboardChangeEvent::getClassInfo());

void JSSoftKeyboardChangeEvent::initialize(JSContextRef /*context*/,
                                           JSObjectRef object)
{
    LogDebug("entered");

    JSSoftKeyboardChangeEventPrivateObject* priv =
        static_cast<JSSoftKeyboardChangeEventPrivateObject*>(JSObjectGetPrivate(
                                                                 object));

    AssertMsg(priv, "Missing private object");
}

void JSSoftKeyboardChangeEvent::finalize(JSObjectRef object)
{
    LogDebug("entered");
    JSSoftKeyboardChangeEventPrivateObject* priv =
        static_cast<JSSoftKeyboardChangeEventPrivateObject*>(JSObjectGetPrivate(
                                                                 object));

    delete priv;
    LogDebug("private object is realised");
}

JSValueRef JSSoftKeyboardChangeEvent::getState(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered!");

    Try
    {
        Converter converter(context);
        return converter.toJSValueRef(getPrivateObject(object)->getState());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSSoftKeyboardChangeEvent::getWidth(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered!");

    Try
    {
        Converter converter(context);
        return converter.toJSValueRef(getPrivateObject(object)->getWidth());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSSoftKeyboardChangeEvent::getHeight(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        Converter converter(context);
        return converter.toJSValueRef(getPrivateObject(object)->getHeight());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

ISoftKeyboardChangeEventPtr JSSoftKeyboardChangeEvent::getPrivateObject(
    JSObjectRef arg)
{
    JSSoftKeyboardChangeEventPrivateObject* priv =
        static_cast<JSSoftKeyboardChangeEventPrivateObject*>(JSObjectGetPrivate(
                                                                 arg));

    if (!priv) {
        LogError("Private object not initialized");
        ThrowMsg(Commons::NullPointerException,
                 "Private object not initialized");
    }

    return priv->getObject();
}

#undef CATCH_EXCEPTION_CONVERSION
#undef CATCH_EXCEPTION_NULL_PTR
#undef CATCH_EXCEPTION_PLATFORM_ERROR
}
}
