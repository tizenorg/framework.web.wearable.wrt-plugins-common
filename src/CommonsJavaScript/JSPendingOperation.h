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
/*
 * @file        JSPendingOperation.h
 * @author      Pete Cole (peter.cole@partner.samsung.com)
 * @version     0.1
 *              0.2 k.majewski@samsung.com
 * @brief       Declaration of the JSPendingOperation class
 */

#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_PENDINGOPERATION_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_PENDINGOPERATION_H_

#include <JavaScriptCore/JavaScript.h>
#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <CommonsJavaScript/JSPendingOperationPrivateObject.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
//typedef Commons::PrivateObjectT<IJSPendingOperationPrivateObject*>::Type
// PendingOperationPrivateObject;

class JSPendingOperation
{
  public:
    /*
     * This initializes this JS class in the JS Engine.
     */
    static JSClassRef getClassRef();

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
     * This structure contains properties and callbacks that define a type of
     * object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_functions[];

    static JSClassRef m_classRef;

    /**
     * Cancel the pending operation, if it has yet not completed.
     */
    static JSValueRef cancel(JSContextRef context,
                             JSObjectRef object,
                             JSObjectRef thisObject,
                             size_t argumentCount,
                             const JSValueRef arguments[],
                             JSValueRef* exception);
};
} // CommonsJavaScript
} // WrtDeviceApis

#endif // _JS_WAC_PENDINGOPERATION_H_

