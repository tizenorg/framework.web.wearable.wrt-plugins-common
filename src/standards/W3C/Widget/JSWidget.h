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
 * @file       JSWidget.h
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef _WRT_PLUGIN_JS_WIDGET_H_
#define _WRT_PLUGIN_JS_WIDGET_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <wrt_plugin_export.h>

namespace WrtPlugins {
namespace W3C {
class JSWidget
{
  public:
    /**
     * This method initializes this in the JS Engine.
     */
    static JSClassRef getClassRef();

    /**
     * Gets object's class description.
     */
    static const JSClassDefinition* getClassInfo();

    static JSObjectRef callAsConstructor(JSContextRef context,
                                         JSObjectRef constructor,
                                         size_t argumentCount,
                                         const JSValueRef arguments[],
                                         JSValueRef* exception);

  private:
    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context,
                           JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * This structure describes a statically declared value property.
     */
    static JSStaticValue m_property[];

    /**
     * This structure contains properties and callbacks that define a type of
     * object.
     */
    static JSClassDefinition m_classInfo;

    static JSClassRef m_jsClassRef;

    // getters for properties
    static JSValueRef getAuthor(JSContextRef context,
                                JSObjectRef object,
                                JSStringRef propertyName,
                                JSValueRef* exception);
    static JSValueRef getAuthorEmail(JSContextRef context,
                                     JSObjectRef object,
                                     JSStringRef propertyName,
                                     JSValueRef* exception);
    static JSValueRef getAuthorHref(JSContextRef context,
                                    JSObjectRef object,
                                    JSStringRef propertyName,
                                    JSValueRef* exception);
    static JSValueRef getDescription(JSContextRef context,
                                     JSObjectRef object,
                                     JSStringRef propertyName,
                                     JSValueRef* exception);
    static JSValueRef getId(JSContextRef context,
                            JSObjectRef object,
                            JSStringRef propertyName,
                            JSValueRef* exception);
    static JSValueRef getName(JSContextRef context,
                              JSObjectRef object,
                              JSStringRef propertyName,
                              JSValueRef* exception);
    static JSValueRef getShortName(JSContextRef context,
                                   JSObjectRef object,
                                   JSStringRef propertyName,
                                   JSValueRef* exception);
    static JSValueRef getVersion(JSContextRef context,
                                 JSObjectRef object,
                                 JSStringRef propertyName,
                                 JSValueRef* exception);
    static JSValueRef getHeight(JSContextRef context,
                                JSObjectRef object,
                                JSStringRef propertyName,
                                JSValueRef* exception);
    static JSValueRef getWidth(JSContextRef context,
                               JSObjectRef object,
                               JSStringRef propertyName,
                               JSValueRef* exception);

    static bool hasProperty(JSContextRef context,
                            JSObjectRef object,
                            JSStringRef propertyName);

    static JSValueRef getProperty(JSContextRef context,
                                  JSObjectRef object,
                                  JSStringRef propertyName,
                                  JSValueRef* exception);

    static bool setProperty(JSContextRef context,
                            JSObjectRef object,
                            JSStringRef propertyName,
                            JSValueRef value,
                            JSValueRef* exception);
};
}
}

#endif /* _WRT_PLUGIN_JS_WIDGET_INTERFACE_H_ */
