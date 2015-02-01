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
 * @file        JSPendingOperation.cpp
 * @author      Pete Cole (peter.cole@partner.samsung.com)
 * @version     0.1
 *              0.2 k.majewski@samsung.com
 * @brief       Implementation of the JSPendingOperation class
 */

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "JSPendingOperation.h"
#include "JSPendingOperationPrivateObject.h"

namespace WrtDeviceApis {
namespace CommonsJavaScript {
namespace {
const char* PLUGIN_NAME = "PendingOperation";
}

JSClassRef JSPendingOperation::m_classRef = NULL;

JSClassDefinition JSPendingOperation::m_classInfo = {
    0,
    kJSClassAttributeNone,
    PLUGIN_NAME,
    0,
    NULL,
    m_functions,
    initialize,
    finalize,
    NULL, //hasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //getPropertyNames,
    NULL,
    NULL,
    NULL,
    NULL, //ConvertToType,
};

JSStaticFunction JSPendingOperation::m_functions[] = {
    { "cancel", cancel, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSPendingOperation::getClassRef()
{
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

void JSPendingOperation::initialize(JSContextRef /*context*/,
                                    JSObjectRef object)
{
    assert(NULL != JSObjectGetPrivate(object));
}

void JSPendingOperation::finalize(JSObjectRef object)
{
    delete static_cast<IJSPendingOperationPrivateObject *>(
        JSObjectGetPrivate(object));
}

JSValueRef JSPendingOperation::cancel(JSContextRef context,
                                      JSObjectRef object,
                                      JSObjectRef thisObject,
                                      size_t argumentCount,
                                      const JSValueRef arguments[],
                                      JSValueRef* exception)
{
    (void) object;
    (void) argumentCount;
    (void) arguments;
    (void) exception;
    LogDebug(__FUNCTION__);
    IJSPendingOperationPrivateObject *priv =
        static_cast<IJSPendingOperationPrivateObject *>(
            JSObjectGetPrivate(thisObject));
    assert(NULL != priv);
    bool cancelResult = priv->cancel();
    LogDebug("cancel result : " << cancelResult);
    return JSValueMakeBoolean(context, cancelResult);
}
} // CommonsJavaScript
} // WrtDeviceApis

