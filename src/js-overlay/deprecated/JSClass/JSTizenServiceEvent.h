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
 * @file       JSTizenServiceEvent.h
 * @author     Yunchan Cho (yunchan.cho@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef _WRT_PLUGIN_JS_TIZEN_SERVICE_EVENT_H_
#define _WRT_PLUGIN_JS_TIZEN_SERVICE_EVENT_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <TizenServiceEvent/ITizenServiceEvent.h>

namespace WrtPlugins {
namespace Tizen {
typedef WrtDeviceApis::CommonsJavaScript::PrivateObjectT<
    WrtDeviceApis::TizenServiceEvent::Api::ITizenServiceEventPtr>::Type
JSTizenServiceEventPrivateObject;

class JSTizenServiceEvent
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
    static JSValueRef getScale(
        JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

    static JSValueRef getBundle(
        JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

    static WrtDeviceApis::TizenServiceEvent::Api::ITizenServiceEventPtr
    getPrivateObject(JSObjectRef arg);
};
} // Tizen
} // WrtPlugins

#endif // _WRT_PLUGIN_JS_TIZEN_SERVICE_EVENT_H_
