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
#include "JSDOMException.h"

#include <dpl/log/log.h>

#include "Converter.h"

namespace {
const char* PLUGIN_NAME = "DOMException";
const char* PROPERTY_CODE = "code";
const char* PROPERTY_MESSAGE = "message";

struct Error
{
    const char* name;
    const unsigned short value;
};
// This array CAN'T be left empty!
const Error PROPERTY_ERROR[] = {
    { "UNKNOWN_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::UNKNOWN_ERR },
    { "INDEX_SIZE_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::INDEX_SIZE_ERR },
    { "DOMSTRING_SIZE_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::DOMSTRING_SIZE_ERR },
    { "HIERARCHY_REQUEST_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::
          HIERARCHY_REQUEST_ERR },
    { "WRONG_DOCUMENT_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::WRONG_DOCUMENT_ERR },
    { "INVALID_CHARACTER_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::
          INVALID_CHARACTER_ERR },
    { "NO_DATA_ALLOWED_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::
          NO_DATA_ALLOWED_ERR },
    { "NO_MODIFICATION_ALLOWED_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::
          NO_MODIFICATION_ALLOWED_ERR },
    { "NOT_FOUND_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::NOT_FOUND_ERR },
    { "NOT_SUPPORTED_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::NOT_SUPPORTED_ERR },
    { "INUSE_ATTRIBUTE_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::
          INUSE_ATTRIBUTE_ERR },
    { "INVALID_STATE_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::INVALID_STATE_ERR },
    { "SYNTAX_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::SYNTAX_ERR },
    { "INVALID_MODIFICATION_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::
          INVALID_MODIFICATION_ERR },
    { "NAMESPACE_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::NAMESPACE_ERR },
    { "INVALID_ACCESS_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::INVALID_ACCESS_ERR },
    { "VALIDATION_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::VALIDATION_ERR },
    { "TYPE_MISMATCH_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::TYPE_MISMATCH_ERR },
    { "SECURITY_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::SECURITY_ERR },
    { "NETWORK_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::NETWORK_ERR },
    { "ABORT_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::ABORT_ERR },
    { "TIMEOUT_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::TIMEOUT_ERR },
    { "INVALID_VALUES_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::INVALID_VALUES_ERR },
    { "IO_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::IO_ERR },
    { "QUOTA_EXCEEDED_ERR",
      WrtDeviceApis::CommonsJavaScript::JSDOMException::QUOTA_EXCEEDED_ERR }
};
} // namespace

namespace WrtDeviceApis {
namespace CommonsJavaScript {
JSClassRef JSDOMException::m_classRef = NULL;

JSClassDefinition JSDOMException::m_classInfo = {
    0,
    kJSClassAttributeNone,
    PLUGIN_NAME,
    0,
    m_properties,
    NULL, //__function,
    initialize,
    finalize,
    hasProperty,
    getProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    getPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    hasInstance,
    NULL, //ConvertToType,
};

JSStaticValue JSDOMException::m_properties[] = {
    { PROPERTY_CODE, getStaticProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_MESSAGE, getStaticProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSDOMException::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSDOMException::getClassRef()
{
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

void JSDOMException::initialize(JSContextRef /*context*/,
                                JSObjectRef /*object*/)
{}

void JSDOMException::finalize(JSObjectRef object)
{
    PrivateObject* privateObject =
        static_cast<PrivateObject*>(JSObjectGetPrivate(object));
    if (privateObject) {
        JSObjectSetPrivate(object, NULL);
        delete privateObject;
    }
}

bool JSDOMException::hasProperty(JSContextRef /*context*/,
                                 JSObjectRef /*object*/,
                                 JSStringRef propertyName)
{
    const size_t size = sizeof(PROPERTY_ERROR) / sizeof(PROPERTY_ERROR[0]);
    for (size_t i = 0; i < size; ++i) {
        if (JSStringIsEqualToUTF8CString(propertyName,
                                         PROPERTY_ERROR[i].name))
        {
            return true;
        }
    }
    return false;
}

JSValueRef JSDOMException::getStaticProperty(JSContextRef context,
                                             JSObjectRef object,
                                             JSStringRef propertyName,
                                             JSValueRef* /*exception*/)
{
    PrivateObject* privateObject =
        static_cast<PrivateObject*>(JSObjectGetPrivate(object));
    if (!privateObject) {
        LogError("Private object is not set.");
        return JSValueMakeUndefined(context);
    }

    Converter converter(context);
    try {
        if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_CODE)) {
            return converter.toJSValueRef(privateObject->getObject()->getCode());
        } else if (JSStringIsEqualToUTF8CString(propertyName,
                                                PROPERTY_MESSAGE))
        {
            return converter.toJSValueRef(
                       privateObject->getObject()->getMessage());
        }
    } catch (const Commons::ConversionException& ex) {
        LogError("Exception: " << ex.GetMessage());
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSDOMException::getProperty(JSContextRef context,
                                       JSObjectRef /*object*/,
                                       JSStringRef propertyName,
                                       JSValueRef* /*exception*/)
{
    Converter converter(context);
    try {
        std::string prop = converter.toString(propertyName);

        const size_t size = sizeof(PROPERTY_ERROR) / sizeof(PROPERTY_ERROR[0]);
        for (size_t i = 0; i < size; ++i) {
            if (prop == PROPERTY_ERROR[i].name) {
                return converter.toJSValueRef(PROPERTY_ERROR[i].value);
            }
        }
    } catch (const Commons::ConversionException& ex) {
        LogError("Exception: " << ex.GetMessage());
    }

    return JSValueMakeUndefined(context);
}

void JSDOMException::getPropertyNames(
    JSContextRef /*context*/,
    JSObjectRef /*object*/,
    JSPropertyNameAccumulatorRef accumulator)
{
    const size_t size = sizeof(PROPERTY_ERROR) / sizeof(PROPERTY_ERROR[0]);
    for (size_t i = 0; i < size; ++i) {
        JSPropertyNameAccumulatorAddName(accumulator,
                                         JSStringCreateWithUTF8CString(
                                             PROPERTY_ERROR[i].name));
    }
}

bool JSDOMException::hasInstance(JSContextRef context,
                                 JSObjectRef /*constructor*/,
                                 JSValueRef possibleInstance,
                                 JSValueRef* /*exception*/)
{
    return JSValueIsObjectOfClass(context, possibleInstance, getClassRef());
}
} // CommonsJavaScript
} // WrtDeviceApis

