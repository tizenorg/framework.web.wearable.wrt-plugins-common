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
#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_DOM_EXCEPTION_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_DOM_EXCEPTION_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <CommonsJavaScript/DOMExceptionData.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
class JSDOMException
{
  public:
    typedef PrivateObjectT<DOMExceptionDataPtr>::Type PrivateObject;

  public:
    static const unsigned short UNKNOWN_ERR = 0;
    static const unsigned short INDEX_SIZE_ERR = 1;
    static const unsigned short DOMSTRING_SIZE_ERR = 2;
    static const unsigned short HIERARCHY_REQUEST_ERR = 3;
    static const unsigned short WRONG_DOCUMENT_ERR = 4;
    static const unsigned short INVALID_CHARACTER_ERR = 5;
    static const unsigned short NO_DATA_ALLOWED_ERR = 6;
    static const unsigned short NO_MODIFICATION_ALLOWED_ERR = 7;
    static const unsigned short NOT_FOUND_ERR = 8;
    static const unsigned short NOT_SUPPORTED_ERR = 9;
    static const unsigned short INUSE_ATTRIBUTE_ERR = 10;
    static const unsigned short INVALID_STATE_ERR = 11;
    static const unsigned short SYNTAX_ERR = 12;
    static const unsigned short INVALID_MODIFICATION_ERR = 13;
    static const unsigned short NAMESPACE_ERR = 14;
    static const unsigned short INVALID_ACCESS_ERR = 15;
    static const unsigned short VALIDATION_ERR = 16;
    static const unsigned short TYPE_MISMATCH_ERR = 17;
    static const unsigned short SECURITY_ERR = 18;
    static const unsigned short NETWORK_ERR = 19;
    static const unsigned short ABORT_ERR = 20;
    static const unsigned short TIMEOUT_ERR = 21;
    static const unsigned short INVALID_VALUES_ERR = 22;
    static const unsigned short IO_ERR = 100;

    //TODO INVALID_VALUES_ERR has the same error code ??
    static const unsigned short QUOTA_EXCEEDED_ERR = 22;

  public:
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
     * The callback invoked when getting a property's value.
     */
    static bool hasProperty(JSContextRef context,
                            JSObjectRef object,
                            JSStringRef propertyName);

    /**
     * The callback invoked when getting a property's value.
     */
    static JSValueRef getProperty(JSContextRef context,
                                  JSObjectRef object,
                                  JSStringRef propertyName,
                                  JSValueRef* exception);

    /**
     * The callback invoked when getting a property's value.
     */
    static JSValueRef getStaticProperty(JSContextRef context,
                                        JSObjectRef object,
                                        JSStringRef propertyName,
                                        JSValueRef* exception);

    static void getPropertyNames(JSContextRef context,
                                 JSObjectRef object,
                                 JSPropertyNameAccumulatorRef accumulator);

    /**
     * The callback invoked when an object is used as the target of an
     * 'instanceof' expression.
     */
    static bool hasInstance(JSContextRef context,
                            JSObjectRef constructor,
                            JSValueRef possibleInstance,
                            JSValueRef* exception);

    /**
     * This structure contains properties and callbacks that define a type of
     * object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared value property.
     */
    static JSStaticValue m_properties[];

    static JSClassRef m_classRef;
};
} // CommonsJavaScript
} // WrtDeviceApis

#endif //

