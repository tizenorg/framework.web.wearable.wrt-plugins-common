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
 * @author          Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_UTILS_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_UTILS_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include <CommonsJavaScript/JSPendingOperation.h>
#include <CommonsJavaScript/JSPendingOperationPrivateObject.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
/**
 * Creates pending operation object.
 * @param context Global context.
 * @param event Event or other object enclosed in shared ptr and convertible
 *              to @see WrtDeviceApis::Commons::IEventController.
 * @return Pending operation object.
 */
template<typename EventType>
JSObjectRef makePendingOperation(JSContextRef context,
                                 const DPL::SharedPtr<EventType>& event)
{
    Commons::IEventControllerPtr eventController =
        DPL::StaticPointerCast<Commons::IEventController>(event);
    IJSPendingOperationPrivateObject* pendingOperation =
        new IJSPendingOperationPrivateObject(eventController);
    return JSObjectMake(context,
                        JSPendingOperation::getClassRef(), pendingOperation);
}

template<typename EventType>
JSObjectRef makeExtCancelSupportedPendingOperation(
    JSContextRef context,
    const DPL::SharedPtr<EventType>& event,
    const DPL::SharedPtr<Commons::IExternEventCanceler<EventType> >&
    cancel)
{
    IJSPendingOperationPrivateObject* pendingOperation =
        new IJSExtCancelPendingOperationPrivateObject<EventType>(event, cancel);

    return JSObjectMake(context,
                        JSPendingOperation::getClassRef(),
                        pendingOperation);
}
} // CommonsJavaScript
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_COMMONSJAVASCRIPT_UTILS_H_
