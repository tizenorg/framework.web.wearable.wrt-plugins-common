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
 * @file        JSStorageEvent.cpp
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 */

#include "JSStorageEvent.h"
#include <dpl/log/log.h>
#include <dpl/assert.h>
#include <StorageEvent/IStorageEvent.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::StorageEvent::Api;

#define WIDGET_PLUGIN_NAME "StorageEvent"

#define WRT_STORAGE_EVENT_PROPERTY_KEY          "key"
#define WRT_STORAGE_EVENT_PROPERTY_OLD_VALUE    "oldValue"
#define WRT_STORAGE_EVENT_PROPERTY_NEW_VALUE    "newValue"
#define WRT_STORAGE_EVENT_PROPERTY_URL          "url"
#define WRT_STORAGE_EVENT_PROPERTY_STORAGE_AREA "storageArea"

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

#define CATCH_EXCEPTION_SECURITY \
    Catch(Commons::SecurityException){ \
        LogError("Security exception occured"); \
        return JSDOMExceptionFactory:: \
                   SecurityException.make(context, exception); \
    }

namespace WrtPlugins {
namespace W3C {
JSClassDefinition JSStorageEvent::m_classInfo = {
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

JSStaticValue JSStorageEvent::m_property[] = {
    { WRT_STORAGE_EVENT_PROPERTY_KEY, JSStorageEvent::getKey,
      0, kJSPropertyAttributeReadOnly },
    { WRT_STORAGE_EVENT_PROPERTY_OLD_VALUE, JSStorageEvent::getOldValue,
      0, kJSPropertyAttributeReadOnly },
    { WRT_STORAGE_EVENT_PROPERTY_NEW_VALUE, JSStorageEvent::getNewValue,
      0, kJSPropertyAttributeReadOnly },
    { WRT_STORAGE_EVENT_PROPERTY_URL, JSStorageEvent::getUrl,
      0, kJSPropertyAttributeReadOnly },
    { WRT_STORAGE_EVENT_PROPERTY_STORAGE_AREA, JSStorageEvent::getStorageArea,
      0, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSStorageEvent::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSStorageEvent::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSStorageEvent::m_jsClassRef = JSClassCreate(
        JSStorageEvent::getClassInfo());

void JSStorageEvent::initialize(JSContextRef /*context*/,
                                JSObjectRef object)
{
    LogDebug("entered");

    JSStorageEventPrivateObject* priv =
        static_cast<JSStorageEventPrivateObject*>(JSObjectGetPrivate(object));

    AssertMsg(priv, "Missing private object");
}

void JSStorageEvent::finalize(JSObjectRef object)
{
    LogDebug("entered");
    JSStorageEventPrivateObject* priv =
        static_cast<JSStorageEventPrivateObject*>(JSObjectGetPrivate(object));

    delete priv;
    LogDebug("private object is realised");
}

JSValueRef JSStorageEvent::getKey(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        Converter converter(context);

        return converter.toJSValueRef(getPrivateObject(object)->getKey());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

JSValueRef JSStorageEvent::getOldValue(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        Converter converter(context);

        DPL::OptionalString oldValue = getPrivateObject(object)->getOldValue();

        if (!oldValue) {
            return JSValueMakeNull(context);
        } else {
            return converter.toJSValueRef(DPL::ToUTF8String(*oldValue));
        }
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

JSValueRef JSStorageEvent::getNewValue(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        Converter converter(context);

        DPL::OptionalString newValue = getPrivateObject(object)->getNewValue();

        if (!newValue) {
            return JSValueMakeNull(context);
        } else {
            return converter.toJSValueRef(DPL::ToUTF8String(*newValue));
        }
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

JSValueRef JSStorageEvent::getUrl(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        Converter converter(context);

        return converter.toJSValueRef(getPrivateObject(object)->getUrl());
    }
    CATCH_EXCEPTION_CONVERSION
        CATCH_EXCEPTION_NULL_PTR
}

JSValueRef JSStorageEvent::getStorageArea(
    JSContextRef context,
    JSObjectRef /*object*/,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        LogError("Not implemented");
        return JSValueMakeUndefined(context);
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

IStorageEventPtr JSStorageEvent::getPrivateObject(JSObjectRef arg)
{
    JSStorageEventPrivateObject* priv =
        static_cast<JSStorageEventPrivateObject*>(JSObjectGetPrivate(arg));

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
#undef CATCH_EXCEPTION_SECURITY
}
}
