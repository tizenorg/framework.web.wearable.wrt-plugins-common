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
 * @file        JSTizenServiceEvent.cpp
 * @author      Yunchan Cho (yunchan.cho@samsung.com)
 * @version     0.1
 */

#include "JSTizenServiceEvent.h"
#include <dpl/log/log.h>
#include <dpl/assert.h>
#include <TizenServiceEvent/ITizenServiceEvent.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::TizenServiceEvent::Api;

#define WIDGET_PLUGIN_NAME "TizenServiceEvent"

#define TIZEN_SERVICE_EVENT_PROPERTY_SCALE      "scale"
#define TIZEN_SERVICE_EVENT_PROPERTY_BUNDLE     "__bundle"

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
namespace Tizen {
JSClassDefinition JSTizenServiceEvent::m_classInfo = {
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

JSStaticValue JSTizenServiceEvent::m_property[] = {
    { TIZEN_SERVICE_EVENT_PROPERTY_SCALE, JSTizenServiceEvent::getScale,
      0, kJSPropertyAttributeReadOnly },
    { TIZEN_SERVICE_EVENT_PROPERTY_BUNDLE, JSTizenServiceEvent::getBundle,
      0, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSTizenServiceEvent::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSTizenServiceEvent::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSTizenServiceEvent::m_jsClassRef = JSClassCreate(
        JSTizenServiceEvent::getClassInfo());

void JSTizenServiceEvent::initialize(JSContextRef /*context*/,
                                     JSObjectRef object)
{
    LogDebug("entered");

    JSTizenServiceEventPrivateObject* priv =
        static_cast<JSTizenServiceEventPrivateObject*>(JSObjectGetPrivate(
                                                           object));

    AssertMsg(priv, "Missing private object");
}

void JSTizenServiceEvent::finalize(JSObjectRef object)
{
    LogDebug("entered");
    JSTizenServiceEventPrivateObject* priv =
        static_cast<JSTizenServiceEventPrivateObject*>(JSObjectGetPrivate(
                                                           object));

    delete priv;
    LogDebug("private object is realised");
}

JSValueRef JSTizenServiceEvent::getScale(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        Converter converter(context);
        return converter.toJSValueRef(getPrivateObject(object)->getScale());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

JSValueRef JSTizenServiceEvent::getBundle(
    JSContextRef context,
    JSObjectRef object,
    JSStringRef /*propertyName*/,
    JSValueRef* exception)
{
    LogDebug("entered");

    Try
    {
        Converter converter(context);
        return converter.toJSValueRef(getPrivateObject(object)->getBundle());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

ITizenServiceEventPtr JSTizenServiceEvent::getPrivateObject(JSObjectRef arg)
{
    JSTizenServiceEventPrivateObject* priv =
        static_cast<JSTizenServiceEventPrivateObject*>(JSObjectGetPrivate(arg));

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
