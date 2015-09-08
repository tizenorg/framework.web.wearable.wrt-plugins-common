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
 * @file        JSPreferences.cpp
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version     0.1
 */

#include "JSPreferences.h"

#include <string>
#include <dpl/assert.h>
#include <dpl/log/secure_log.h>
#include <dpl/optional_typedefs.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include <StorageEvent/StorageEventMgr.h>
#include <StorageEvent/IStorageEvent.h>
#include <dispatch-event/dispatch_event_support.h>

// import temporarily from JSContextRefPrivate.h
extern "C" JSGlobalContextRef JSContextGetGlobalContext(JSContextRef ctx);

using namespace std;
using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::LocalStorage::Api;
using namespace WrtDeviceApis::StorageEvent::Api;

#define PREFERENCES_PLUGIN_NAME       "preferences"
#define LOCAL_STORAGE_PROPERTY_LENGTH "length"

#define CATCH_EXCEPTION_NO_MODIFABLE \
    Catch(Commons::LocalStorageValueNoModifableException) { \
        _E("The item is read only"); \
        return JSDOMExceptionFactory:: \
                   NoModificationAllowedException.make(context, exception); \
    }

#define CATCH_EXCEPTION_CONVERSION \
    Catch(Commons::ConversionException) { \
        _E("Error on conversion"); \
        return JSDOMExceptionFactory:: \
                   UnknownException.make(context, exception); \
    }

#define CATCH_EXCEPTION_NULL_PTR \
    Catch(Commons::NullPointerException) { \
        _E("Error on pointer, null value"); \
        return JSDOMExceptionFactory:: \
                   UnknownException.make(context, exception); \
    }

#define CATCH_EXCEPTION_PLATFORM_ERROR \
    Catch(Commons::PlatformException){ \
        _E("PlatformException occured"); \
        return JSDOMExceptionFactory:: \
                   UnknownException.make(context, exception); \
    }

#define CATCH_EXCEPTION_SECURITY \
    Catch(Commons::SecurityException){ \
        _E("Security exception occured"); \
        return JSDOMExceptionFactory:: \
                   SecurityException.make(context, exception); \
    }

#define CATCH_EXCEPTION_OUT_OF_RANGE \
    Catch(Commons::OutOfRangeException) { \
        _E("OutOfRangeException"); \
        return JSDOMExceptionFactory:: \
                   QuotaExceededException.make(context, exception); \
    }

#define CATCH_EXCEPTION_INVALID_ARG \
    Catch(Commons::InvalidArgumentException) { \
        _E("Pair for given key doesnt exist"); \
        return JSValueMakeNull(context); \
    }

namespace WrtPlugins {
namespace W3C {
ILocalStoragePtr getIStorage(JSObjectRef arg)
{
    _D("get localstorage object");
    LocalStoragePrivateData* priv =
        static_cast<LocalStoragePrivateData*>(JSObjectGetPrivate(arg));

    if (!priv) {
        _E("Private object not initialized");
        ThrowMsg(Commons::NullPointerException,
                 "Private object not initialized");
    }

    return priv->istorage;
}

JSObjectRef getWidgetObject(JSObjectRef arg)
{
    _D("get widget object");
    LocalStoragePrivateData* priv =
        static_cast<LocalStoragePrivateData*>(JSObjectGetPrivate(arg));

    if (!priv) {
        _E("Private object not initialized");
        ThrowMsg(Commons::NullPointerException,
                 "Private object not initialized");
    }

    return priv->widgetObject;
}

enum class ModificationType
{
    RemoveItem,
    SetItem
};

//this function may throw exceptions
IStorageEventPtr modifyItemAndCreateEvent(ModificationType type,
                                          JSObjectRef thisObject,
                                          const string& key,
                                          const string& value = std::string())
{
    DPL::OptionalStdString oldValue =
        getIStorage(thisObject)->getValue(key);

    if (ModificationType::SetItem == type) {
        getIStorage(thisObject)->setItem(key, value, false);
    } else if (ModificationType::RemoveItem == type) {
        getIStorage(thisObject)->removeItem(key);
    } else {
        Assert(!"Wrong Modification type");
    }

    DPL::OptionalStdString newValue =
        getIStorage(thisObject)->getValue(key);

    //create event object
    IStorageEventPtr storageEvent = getStorageEvent();

    //key
    storageEvent->setKey(key);
    //oldvalue
    if (!!oldValue) {
        storageEvent->setOldValue(*oldValue);
    }
    //newValue
    if (!!newValue) {
        storageEvent->setNewValue(*newValue);
    }

    return storageEvent;
}

JSClassDefinition JSPreferences::m_classInfo = {
    0,
    kJSClassAttributeNone,
    PREFERENCES_PLUGIN_NAME,
    0,
    m_property,
    m_function,
    initialize,
    finalize,
    hasProperty,
    getProperty,
    setProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL, //ConvertToType,
};

JSStaticFunction JSPreferences::m_function[] = {
    { "setItem", JSPreferences::setItem, kJSPropertyAttributeNone },
    { "removeItem", JSPreferences::removeItem, kJSPropertyAttributeNone },
    { "getItem", JSPreferences::getItem, kJSPropertyAttributeNone },
    { "clear", JSPreferences::clear, kJSPropertyAttributeNone },
    { "key", JSPreferences::key, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSStaticValue JSPreferences::m_property[] = {
    { LOCAL_STORAGE_PROPERTY_LENGTH,
      JSPreferences::getLength,
      NULL,
      kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSPreferences::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSPreferences::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSPreferences::m_jsClassRef =
    JSClassCreate(JSPreferences::getClassInfo());

void JSPreferences::initialize(JSContextRef /* context */,
                               JSObjectRef object)
{
    _D("entered");

    LocalStoragePrivateData* priv =
        static_cast<LocalStoragePrivateData*>(JSObjectGetPrivate(object));

    AssertMsg(priv, "private object of preferences is NULL");
}

void JSPreferences::finalize(JSObjectRef object)
{
    _D("entered");
    LocalStoragePrivateData* priv =
        static_cast<LocalStoragePrivateData*>(JSObjectGetPrivate(object));

    delete priv;
}

JSValueRef JSPreferences::removeItem(JSContextRef context,
                                     JSObjectRef /*object*/,
                                     JSObjectRef thisObject,
                                     size_t /*argumentCount*/,
                                     const JSValueRef arguments[],
                                     JSValueRef* exception)
{
    _D("entered");

    Try {
        Converter converter(context);

        std::string key = converter.toString(arguments[0]);

        DPL::OptionalStdString oldValue = getIStorage(thisObject)->getValue(key);
        getIStorage(thisObject)->removeItem(key);
        DPL::OptionalStdString newValue = getIStorage(thisObject)->getValue(key);

        JSContextRef g_context  = JSContextGetGlobalContext(context);

        DispatchEventSupport::dispatchStorageEvent(g_context, key, oldValue, newValue, "");

        return JSValueMakeNull(context);
    }
    CATCH_EXCEPTION_NO_MODIFABLE
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

JSValueRef JSPreferences::setItem(JSContextRef context,
                                  JSObjectRef /*object*/,
                                  JSObjectRef thisObject,
                                  size_t /*argumentCount*/,
                                  const JSValueRef arguments[],
                                  JSValueRef* exception)
{
    _D("entered");

    Try {
        Converter converter(context);

        std::string key = converter.toString(arguments[0]);
        std::string value = converter.tryString(arguments[1]);

        DPL::OptionalStdString oldValue = getIStorage(thisObject)->getValue(key);
        getIStorage(thisObject)->setItem(key, value, false);
        DPL::OptionalStdString newValue = getIStorage(thisObject)->getValue(key);

        JSContextRef g_context  = JSContextGetGlobalContext(context);

        DispatchEventSupport::dispatchStorageEvent(g_context, key, oldValue, newValue, "");

        return JSValueMakeUndefined(context);
    }
    CATCH_EXCEPTION_NO_MODIFABLE
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
    CATCH_EXCEPTION_SECURITY
        CATCH_EXCEPTION_OUT_OF_RANGE
}

JSValueRef JSPreferences::clear(JSContextRef context,
                                JSObjectRef /*object*/,
                                JSObjectRef thisObject,
                                size_t /*argumentCount*/,
                                const JSValueRef /*arguments*/[],
                                JSValueRef* exception)
{
    _D("entered");

    Try {
        getIStorage(thisObject)->clear(false);

        JSContextRef g_context  = JSContextGetGlobalContext(context);

        const auto& null = DPL::OptionalStdString();

        DispatchEventSupport::dispatchStorageEvent(g_context, null, null, null, "");

        return JSValueMakeNull(context);
    }
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

JSValueRef JSPreferences::getItem(JSContextRef context,
                                  JSObjectRef /*object*/,
                                  JSObjectRef thisObject,
                                  size_t /*argumentCount*/,
                                  const JSValueRef arguments[],
                                  JSValueRef* exception)
{
    _D("entered");

    Try {
        Converter converter(context);

        std::string key = converter.tryString(arguments[0]);

        _D("Getting item for key %s", key.c_str());

        DPL::OptionalStdString value =
            getIStorage(thisObject)->getValue(key);

        if (!value) {
            return JSValueMakeNull(context);
        } else {
            return converter.toJSValueRef(*value);
        }
    }
    CATCH_EXCEPTION_INVALID_ARG
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY
}

JSValueRef JSPreferences::key(JSContextRef context,
                              JSObjectRef /*object*/,
                              JSObjectRef thisObject,
                              size_t argumentCount,
                              const JSValueRef arguments[],
                              JSValueRef* exception)
{
    _D("entered");

    Try {
        if (argumentCount < 1) {
            _E("No argument found");
            Throw(Commons::InvalidArgumentException);
        }

        Converter converter(context);

        size_t n = converter.toSizeT(arguments[0]);

        std::string value = getIStorage(thisObject)->getKeyByIndex(n);

        return converter.toJSValueRef(value);
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
    CATCH_EXCEPTION_SECURITY
        CATCH_EXCEPTION_INVALID_ARG
}

JSValueRef JSPreferences::getLength(JSContextRef context,
                                    JSObjectRef object,
                                    JSStringRef /*propertyName*/,
                                    JSValueRef* exception)
{
    _D("enter");

    Try
    {
        Converter converter(context);

        size_t value = getIStorage(object)->getStorageSize();
        return converter.toJSValueRef(value);
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
    CATCH_EXCEPTION_SECURITY
        CATCH_EXCEPTION_INVALID_ARG
}

bool JSPreferences::hasProperty(JSContextRef context,
                                JSObjectRef object,
                                JSStringRef propertyName)
{
    _D("enter");

    Try {
        Converter converter(context);

        std::string key = converter.toString(propertyName);

        if (!getIStorage(object)->getValue(key)) {
            return false;
        } else {
            return true;
        }
    }

    Catch(Commons::InvalidArgumentException) {
        _E("Pair for given key doesnt exist");
    }

    Catch(Commons::ConversionException) {
        _E("Error on conversion");
    }

    Catch(Commons::NullPointerException) {
        _E("Error on pointer, null value");
    }

    Catch(Commons::PlatformException){
        _E("PlatformException occured");
    }
    return false;
}

JSValueRef JSPreferences::getProperty(JSContextRef context,
                                      JSObjectRef object,
                                      JSStringRef propertyName,
                                      JSValueRef* exception)
{
    _D("enter");

    Try {
        Converter converter(context);

        std::string key = converter.toString(propertyName);

        DPL::OptionalStdString value =
            getIStorage(object)->getValue(key);

        if (!value) {
            return JSValueMakeNull(context);
        } else {
            return converter.toJSValueRef(*value);
        }
    }

    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_INVALID_ARG
}

bool JSPreferences::setProperty(JSContextRef context,
                                JSObjectRef object,
                                JSStringRef propertyName,
                                JSValueRef jvalue,
                                JSValueRef* exception)
{
    _D("enter");

    Try {
        Converter converter(context);

        std::string key = converter.toString(propertyName);
        std::string value = converter.toString(jvalue);

        DPL::OptionalStdString oldValue = getIStorage(object)->getValue(key);
        getIStorage(object)->setItem(key, value, false);
        DPL::OptionalStdString newValue = getIStorage(object)->getValue(key);

        JSContextRef g_context  = JSContextGetGlobalContext(context);
        std::string oldValueStr = "";
        std::string newValueStr = "";

        if (!!oldValue) { oldValueStr = *oldValue; }
        if (!!newValue) { newValueStr = *newValue; }

        DispatchEventSupport::dispatchStorageEvent(g_context, key, oldValueStr, newValueStr, "");

        return true;
    }
    CATCH_EXCEPTION_NO_MODIFABLE
    CATCH_EXCEPTION_OUT_OF_RANGE
    CATCH_EXCEPTION_INVALID_ARG
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
    CATCH_EXCEPTION_PLATFORM_ERROR
        CATCH_EXCEPTION_SECURITY

    return false;
}

#undef CATCH_EXCEPTION_NO_MODIFABLE
#undef CATCH_EXCEPTION_CONVERSION
#undef CATCH_EXCEPTION_NULL_PTR
#undef CATCH_EXCEPTION_PLATFORM_ERROR
#undef CATCH_EXCEPTION_SECURITY
#undef CATCH_EXCEPTION_OUT_OF_RANGE
#undef CATCH_EXCEPTION_INVALID_ARG
}
}
