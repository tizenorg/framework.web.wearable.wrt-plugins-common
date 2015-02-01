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
 * @author      Karol Majewski (k.majewski@samsung.com)
 * @version     0.1
 * @brief
 */

#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_PENDING_OPERATION_PRIVATE_OBJECT_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_PENDING_OPERATION_PRIVATE_OBJECT_H_

#include <cassert>
#include <dpl/log/log.h>
#include <dpl/shared_ptr.h>
#include <Commons/EventReceiver.h>
#include <Commons/IExternEventCanceler.h>
#include <Commons/IEvent.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
class IJSPendingOperationPrivateObject
{
  public:
    explicit IJSPendingOperationPrivateObject(
        const DPL::SharedPtr<Commons::IEventController>
        &event) :
        m_event(event)
    {
        assert(NULL != m_event.Get());
    }

    virtual bool cancel()
    {
        LogDebug("PendingOperation tries to cancel the request");
        return m_event->cancelRequest();
    }

    virtual ~IJSPendingOperationPrivateObject()
    {}

  protected:
    DPL::SharedPtr<Commons::IEventController> m_event;
};

/*
 * This class implements private object for JavaScript PendingOperation
 * with support to external cancel function.
 *
 * When the cancel() method is invoked on PendingOperation,
 * the OnCancelEvent(<EventType>) method is invoked on user object
 * with apropriate * arguments.
 * It allows user to perform an additional action when PendingOperation
 * is destroyed.
 *
 * The Object which want to handle OnCancelEvent have to inherit from
 * Platform::IExternEventCanceler<EventType>.
 *
 * The cancel's handler have to be passed as second argument
 * */
template<class TemplateEvent>
class IJSExtCancelPendingOperationPrivateObject :
    public IJSPendingOperationPrivateObject
{
  public:
    IJSExtCancelPendingOperationPrivateObject(
        const DPL::SharedPtr< TemplateEvent> &event,
        const DPL::SharedPtr<
            Commons::IExternEventCanceler<TemplateEvent> > & cancel) :
        IJSPendingOperationPrivateObject(
            DPL::StaticPointerCast<Commons::IEventController>(event)),
        m_canceler(cancel)
    {
        assert(NULL != m_canceler);
    }

    virtual bool cancel()
    {
        bool result = IJSPendingOperationPrivateObject::cancel();
        if (!result) {
            LogDebug("Controller can not cancel event, trying platform cancel");
        } else {
            return result;
        }

        if (m_canceler) {
            LogDebug("Calling extern cancel");
            m_canceler->OnCancelEvent(
                DPL::StaticPointerCast<TemplateEvent>(m_event));
        }

        return result;
    }

    virtual ~IJSExtCancelPendingOperationPrivateObject()
    {}

  protected:
    DPL::SharedPtr< Commons::IExternEventCanceler<TemplateEvent> > m_canceler;
};
}
}
#endif /* _JS_WAC_PENDING_OPERATION_PRIVATE_OBJECT_H_ */
