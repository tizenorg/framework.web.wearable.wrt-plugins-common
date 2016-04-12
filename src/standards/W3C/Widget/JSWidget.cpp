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
 * @file        JSWidget.cpp
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version     0.1
 */

#include "JSWidget.h"
#include <memory>
#include <CommonsJavaScript/Converter.h>
#include <dpl/log/log.h>
#include <dpl/log/secure_log.h>
#include <dpl/assert.h>
#include <Widget/WidgetFactory.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include "JSPreferences.h"
#include <Widget/IWidget.h>
#include <LocalStorage/LocalStorageMgr.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <CommonsJavaScript/JSLifeManager.h>

// temporary
extern "C" JSGlobalContextRef JSContextGetGlobalContext(JSContextRef ctx);

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

#define WIDGET_PLUGIN_NAME "Widget"

#define WRT_WIDGET_PROPERTY_AUTHOR       "author"
#define WRT_WIDGET_PROPERTY_AUTHOR_EMAIL "authorEmail"
#define WRT_WIDGET_PROPERTY_AUTHOR_HREF  "authorHref"
#define WRT_WIDGET_PROPERTY_DESCRIPTION  "description"
#define WRT_WIDGET_PROPERTY_ID           "id"
#define WRT_WIDGET_PROPERTY_NAME         "name"
#define WRT_WIDGET_PROPERTY_SHORT_NAME   "shortName"
#define WRT_WIDGET_PROPERTY_VERSION      "version"
#define WRT_WIDGET_PROPERTY_HEIGHT       "height"
#define WRT_WIDGET_PROPERTY_WIDTH        "width"

namespace WrtPlugins {
namespace W3C {
using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::Widget;

struct WidgetPrivateObject
{
    Widget::Api::IWidgetPtr iwidget;
    JSObjectRef preferencesObject;
    JSObjectRef widgetObject;
};
typedef std::shared_ptr<WidgetPrivateObject> WidgetPrivateObjectPtr;

typedef WrtDeviceApis::CommonsJavaScript::PrivateObjectT
<WidgetPrivateObjectPtr>::Type JSWidgetPrivateObject;

WrtDeviceApis::Widget::Api::IWidgetPtr getIWidget(JSObjectRef arg)
{
    JSWidgetPrivateObject* priv =
        static_cast<JSWidgetPrivateObject*>(JSObjectGetPrivate(arg));

    if (!priv) {
        LogError("Private object not initialized");
        ThrowMsg(Commons::NullPointerException,
                 "Private object not initialized");
    }

    return priv->getObject()->iwidget;
}

LocalStorage::Api::ILocalStoragePtr getLocalStorage(int widgetId)
{
    LocalStorage::Api::ILocalStoragePtr storage(
        LocalStorage::Api::getLocalStorage(widgetId));

    return storage;
}

JSObjectRef getPreferences(JSObjectRef arg)
{
    JSWidgetPrivateObject* priv =
        static_cast<JSWidgetPrivateObject*>(JSObjectGetPrivate(arg));

    if (!priv) {
        LogError("Private object not initialized");
        return NULL;
    }

    return priv->getObject()->preferencesObject;
}

JSObjectRef createPreferencesObject(JSContextRef context,
                                    JSObjectRef widgetObject,
                                    int widgetId)
{
    AssertMsg(widgetObject, "Widget Object can'n be null");
    //delete is invoked in JSPreferences::finalize
    LocalStoragePrivateData* priv = new LocalStoragePrivateData;
    AssertMsg(priv, "Private data is null");
    priv->istorage = getLocalStorage(widgetId);
    priv->widgetObject = widgetObject;

    JSObjectRef preferences = JSObjectMake(context,
                                           JSPreferences::getClassRef(),
                                           priv);

    if (!preferences) {
        LogError("Preferences object is null");
        delete priv;
    }
    //Unprotect is called in JSWidget::finalize
    JSValueSafeProtect(context, preferences);

    return preferences;
}

JSClassDefinition JSWidget::m_classInfo = {
    0,
    kJSClassAttributeNone,
    WIDGET_PLUGIN_NAME,
    0,
    m_property,
    NULL,
    initialize,
    finalize,
    hasProperty,
    getProperty,
    setProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    callAsConstructor,
    NULL, //HasInstance,
    NULL, //ConvertToType,
};

JSStaticValue JSWidget::m_property[] = {
    { WRT_WIDGET_PROPERTY_AUTHOR, JSWidget::getAuthor,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_AUTHOR_EMAIL, JSWidget::getAuthorEmail,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_AUTHOR_HREF, JSWidget::getAuthorHref,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_DESCRIPTION, JSWidget::getDescription,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_ID, JSWidget::getId,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_NAME, JSWidget::getName,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_SHORT_NAME, JSWidget::getShortName,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_VERSION, JSWidget::getVersion,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_HEIGHT, JSWidget::getHeight,
      0, kJSPropertyAttributeReadOnly },
    { WRT_WIDGET_PROPERTY_WIDTH, JSWidget::getWidth,
      0, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef JSWidget::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSWidget::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSWidget::m_jsClassRef = JSClassCreate(JSWidget::getClassInfo());

void JSWidget::initialize(JSContextRef context,
                          JSObjectRef object)
{
    JSGlobalContextRef g_context = JSContextGetGlobalContext(context);

    LogDebug("entered. Context : " << context << "(global:" << g_context << "), Object: " << object);

    if (context != g_context)
    {
        context = g_context;
    }

    JSWidgetPrivateObject* priv =
        static_cast<JSWidgetPrivateObject*>(JSObjectGetPrivate(object));

    if (!priv) {
        LogDebug("creation private object");

        Try {
            using namespace WrtDeviceApis::Commons;

            Widget::Api::IWidgetPtr widget =
                Api::WidgetFactory::createWidget();
            int widgetId = WrtAccessSingleton::Instance().getWidgetId();
            JSObjectRef preferences =
                createPreferencesObject(context,
                                        object,
                                        widgetId);
            if (!preferences) {
                LogError("Failed to create preferences object");
            }

            WidgetPrivateObjectPtr widgetPriv(new WidgetPrivateObject);
            widgetPriv->iwidget = widget;
            widgetPriv->preferencesObject = preferences;

            priv = new JSWidgetPrivateObject(context, widgetPriv);
            JSObjectSetPrivate(object, priv);
        }
        Catch(Commons::InvalidArgumentException){
            LogError("You should register widget id in ON_WIDGET_START");
            return;
        }
        Catch(DPL::Exception) {
            LogError("Failed to create private object for JSWidget");
            return;
        }
    }
}

void JSWidget::finalize(JSObjectRef object)
{
    LogDebug("entered");
    LogDebug("Object: " << object);
    JSWidgetPrivateObject* priv =
        static_cast<JSWidgetPrivateObject*>(JSObjectGetPrivate(object));

    if (priv) {
        JSValueSafeUnprotect(priv->getContext(),
                         priv->getObject()->preferencesObject);

        delete priv;
        LogDebug("private object is released");
    } else {
        LogDebug("private object wasn't created");
    }
}

JSValueRef JSWidget::getAuthor(JSContextRef context,
                               JSObjectRef object,
                               JSStringRef /*propertyName*/,
                               JSValueRef* exception)
{
    LogDebug("entered");

    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getAuthor());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getAuthorEmail(JSContextRef context,
                                    JSObjectRef object,
                                    JSStringRef /*propertyName*/,
                                    JSValueRef* exception)
{
    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getAuthorEmail());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getAuthorHref(JSContextRef context,
                                   JSObjectRef object,
                                   JSStringRef /*propertyName*/,
                                   JSValueRef* exception)
{
    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getAuthorHref());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getDescription(JSContextRef context,
                                    JSObjectRef object,
                                    JSStringRef /*propertyName*/,
                                    JSValueRef* exception)
{
    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getDescription());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getId(JSContextRef context,
                           JSObjectRef object,
                           JSStringRef /*propertyName*/,
                           JSValueRef* exception)
{
    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getId());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getName(JSContextRef context,
                             JSObjectRef object,
                             JSStringRef /*propertyName*/,
                             JSValueRef* exception)
{
    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getName());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getShortName(JSContextRef context,
                                  JSObjectRef object,
                                  JSStringRef /*propertyName*/,
                                  JSValueRef* exception)
{
    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getShortName());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getVersion(JSContextRef context,
                                JSObjectRef object,
                                JSStringRef /*propertyName*/,
                                JSValueRef* exception)
{
    Try {
        Converter converter(context);
        return converter.toJSValueRef(getIWidget(object)->getVersion());
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getHeight(JSContextRef context,
                               JSObjectRef object,
                               JSStringRef /*propertyName*/,
                               JSValueRef* exception)
{
    Try {
        Converter converter(context);
        unsigned int height = getIWidget(object)->getHeight();
        if (0 == height) {
            height = 1;
        }
        return converter.toJSValueRef(height);
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

JSValueRef JSWidget::getWidth(JSContextRef context,
                              JSObjectRef object,
                              JSStringRef /*propertyName*/,
                              JSValueRef* exception)
{
    Try {
        Converter converter(context);
        unsigned int width = getIWidget(object)->getWidth();
        if (0 == width) {
            width = 1;
        }
        return converter.toJSValueRef(width);
    }
    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_PLATFORM_ERROR
}

bool JSWidget::hasProperty(JSContextRef context,
                           JSObjectRef /*object*/,
                           JSStringRef propertyName)
{
    LogDebug("enter");

    Try {
        Converter converter(context);

        std::string key = converter.toString(propertyName);
        if (key == "preferences") {
            return true;
        }
    }
    Catch(Commons::InvalidArgumentException) {
        LogDebug("Pair for given key doesnt exist");
    }

    Catch(Commons::ConversionException) {
        LogError("Error on conversion");
    }

    Catch(Commons::NullPointerException) {
        LogError("Error on pointer, null value");
    }

    Catch(Commons::PlatformException){
        LogError("PlatformException occured");
    }
    return false;
}

JSValueRef JSWidget::getProperty(JSContextRef context,
                                 JSObjectRef object,
                                 JSStringRef propertyName,
                                 JSValueRef* exception)
{
    LogDebug("Object: " << object);

    Try {
        Converter converter(context);

        std::string key = converter.toString(propertyName);

        if (key == "preferences") {
            Converter converter(context);
            JSObjectRef pref = getPreferences(object);
            if (!pref) {
                LogError("Preferences object is NULL");
                return JSValueMakeUndefined(context);
            }
            return pref;
        }
        LogError("Property NOT supported: " << propertyName);
        return JSValueMakeUndefined(context);
    }

    CATCH_EXCEPTION_CONVERSION
    CATCH_EXCEPTION_NULL_PTR
        CATCH_EXCEPTION_INVALID_ARG
}

bool JSWidget::setProperty(JSContextRef context,
                           JSObjectRef /*object*/,
                           JSStringRef propertyName,
                           JSValueRef /*jvalue*/,
                           JSValueRef* exception)
{
    LogDebug("enter");

    Try {
        Converter converter(context);

        std::string key = converter.toString(propertyName);
        if (key == "preferences") {
            LogError("Object is read only");
            return true;
        }
    }
    CATCH_EXCEPTION_INVALID_ARG
    CATCH_EXCEPTION_CONVERSION
        CATCH_EXCEPTION_NULL_PTR

    return false;
}

JSObjectRef JSWidget::callAsConstructor(JSContextRef context,
                                        JSObjectRef /*constructor*/,
                                        size_t /*argumentCount*/,
                                        const JSValueRef /*arguments*/[],
                                        JSValueRef* /*exception*/)
{
    LogDebug("widget constructor");
    return JSObjectMake(context, JSWidget::getClassRef(), NULL);
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
