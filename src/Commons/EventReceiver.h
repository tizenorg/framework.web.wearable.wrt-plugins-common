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
#ifndef WRTDEVICEAPIS_COMMONS_EVENT_RECEIVER_H_
#define WRTDEVICEAPIS_COMMONS_EVENT_RECEIVER_H_

#include <memory>
#include <assert.h>
#include <dpl/event/thread_event_dispatcher.h>
#include <dpl/shared_ptr.h>
#include <dpl/event/controller.h>
#include <dpl/type_list.h>
#include <dpl/event/abstract_event_call.h>
#include <dpl/log/secure_log.h>
#include <dpl/mutex.h>
#include <dpl/shared_ptr.h>
#include <Commons/ThreadPool.h>

namespace WrtDeviceApis {
namespace Commons {
template<class TemplateEvent>
class SignalEventCall : public DPL::Event::AbstractEventCall
{
    DPL::SharedPtr<TemplateEvent> m_event;

  public:

    SignalEventCall(const DPL::SharedPtr<TemplateEvent> &event) : m_event(event)
    {}
    virtual void Call()
    {
        _D("signaling in SignalEventCall");
        m_event->signalSynchronousEventFlag();
    }
};

template<class TemplateEvent>
class EventReceiver :
    protected DPL::Event::Controller<
        typename DPL::TypeListDecl<DPL::SharedPtr<TemplateEvent> >::Type>
{
    DPL::Event::ThreadEventDispatcher m_threadDispatcher;

  protected:

    EventReceiver(ThreadEnum::Enumeration threadType)
    {
        DPL::Thread *thread =
            ThreadPool::getInstance().getThreadRef(threadType);
        DPL::Event::ControllerEventHandler<DPL::SharedPtr<TemplateEvent> >::
            Touch();
        DPL::Event::ControllerEventHandler<DPL::SharedPtr<TemplateEvent> >::
            SwitchToThread(thread);
    }

    void signalEventByDispatcher(const DPL::SharedPtr<TemplateEvent> &event)
    {
        _D("called");
        Try {
            DPL::Event::AbstractEventDispatcher *dispatcher =
                ThreadPool::getInstance().getDispatcher(m_threadDispatcher);
            dispatcher->AddEventCall(new SignalEventCall<TemplateEvent>(event));
        }
        Catch(DPL::Thread::Exception::UnmanagedThread) {
            // if called on unmanaged thread,
            // call signalSynchronousEventFlag() directly
            _E("signalSynchronousEventFlag() is called"
                     "by unmanaged thread");
            event->signalSynchronousEventFlag();
        }
    }

    virtual ~EventReceiver()
    {
        DPL::Event::ControllerEventHandler<DPL::SharedPtr<TemplateEvent> >::
            SwitchToThread(NULL);
    }
};

template<class TemplateEvent>
class EventRequestReceiver : private EventReceiver<TemplateEvent>
{
  public:
    EventRequestReceiver(ThreadEnum::Enumeration threadType) : EventReceiver<
            TemplateEvent>(threadType)
    {}
    virtual void OnRequestReceived(const DPL::SharedPtr<TemplateEvent> &)
    { LogError("Pure virtual function call"); } //TODO: temporary code: DPL::SharedPtr -> std::shared_ptr

    virtual void OnRequestReceived(const std::shared_ptr<TemplateEvent> &)
    { LogError("Pure virtual function call"); }

    /*
     *
     * @argument delaySeconds - event will be received not sooner than after
     * delay (in seconds)
     */
    void PostRequest(const DPL::SharedPtr<TemplateEvent> &event,
                     double delaySeconds = 0.0)
    {
        _D("called");
        {
            DPL::Mutex::ScopedLock lock(&event->m_stateMutex);
            assert(TemplateEvent::STATE_INITIAL == event->m_state);
            event->m_state = TemplateEvent::STATE_REQUEST_SEND;
        }

        if (TemplateEvent::HANDLING_SYNCHRONOUS == event->getHandlingType() &&
            !event->m_synchronousEventFlag)
        {
            event->m_synchronousEventFlag = new DPL::WaitableEvent();
        }

        if (0.0 == delaySeconds) {
            DPL::Event::ControllerEventHandler<DPL::SharedPtr<TemplateEvent> >
                ::
                PostEvent(event);
        } else {
            DPL::Event::ControllerEventHandler<DPL::SharedPtr<TemplateEvent> >
                ::
                PostTimedEvent(event, delaySeconds);
        }

        switch (event->getHandlingType()) {
        case TemplateEvent::HANDLING_NOT_SET:
            assert(0);
            break;
        case TemplateEvent::HANDLING_SYNCHRONOUS:
            event->waitForAnswer();
            break;
        }
    }

    void OnEventReceived(const DPL::SharedPtr<TemplateEvent> &event)
    {
        _D("called");
        {
            DPL::Mutex::ScopedLock lock(&event->m_stateMutex);
            if (event->m_cancelled) {
                event->handleCancel();
                event->m_cancelAllowed = true;
                event->signalCancelStatusFlag();
                event->signalFinishedFlag();
                return;
            } else {
                assert(
                    TemplateEvent::STATE_REQUEST_SEND == event->m_state &&
                    "Wrong state!");
            }
            event->m_state = TemplateEvent::STATE_REQUEST_RECEIVED;
        }

        OnRequestReceived(event);
        event->signalCancelStatusFlag();
        //After Controller ends processing it should call it to signal that work
        // is done
        {
            DPL::Mutex::ScopedLock lock(&event->m_stateMutex);

            if (event->m_cancelled) {
                //if cancel was not handled in OnRequestReceived when we should
                //process as if it was not cancelled at all.
                if (event->m_cancelAllowed) {
                    event->handleCancel();
                    event->signalFinishedFlag();
                    return;
                }
            }
            //when event is not in manual answer mode we will answer now
            if (TemplateEvent::HANDLING_ASYNCHRONOUS_MANUAL_ANSWER !=
                event->m_handlingType &&
                TemplateEvent::HANDLING_SYNCHRONOUS_MANUAL_ANSWER !=
                event->m_handlingType)
            {
                event->m_state = TemplateEvent::STATE_ANSWER_SEND;
            }
        }

        switch (event->m_handlingType) {
        case TemplateEvent::HANDLING_NOT_SET:
            assert(0);
            break;
        case TemplateEvent::HANDLING_SYNCHRONOUS:
            //event->Signal();
            this->signalEventByDispatcher(event);
            break;
        case TemplateEvent::HANDLING_ASYNCHRONOUS:
            ///TODO check - shouldn't it be in signalEventByDispatcher?
            if (NULL != event->m_remoteController) {
                event->m_remoteController->PostAnswer(event);
            }
            //event->Signal();
            this->signalEventByDispatcher(event);
            break;
        //when event is in manual answer mode we do nothing - the answer will be
        // send explicit from the code
        case TemplateEvent::HANDLING_SYNCHRONOUS_MANUAL_ANSWER:
        case TemplateEvent::HANDLING_ASYNCHRONOUS_MANUAL_ANSWER:
            break;
        }
    }

    virtual void ManualAnswer(const DPL::SharedPtr<TemplateEvent> &event)
    {
        _D("called");
        assert(
            event->m_handlingType ==
            TemplateEvent::HANDLING_ASYNCHRONOUS_MANUAL_ANSWER ||
            event->m_handlingType ==
            TemplateEvent::HANDLING_SYNCHRONOUS_MANUAL_ANSWER);
        {
            DPL::Mutex::ScopedLock lock(&event->m_stateMutex);
            if (event->m_cancelled) {
                //if cancel was not handled in OnRequestReceived when we should
                //process as if it was not cancelled at all.
                if (event->m_cancelAllowed) {
                    event->handleCancel();
                    event->signalCancelStatusFlag();
                    event->signalFinishedFlag();
                    return;
                }
            }
            event->m_state = TemplateEvent::STATE_ANSWER_SEND;
        }
        switch (event->m_handlingType) {
        case TemplateEvent::HANDLING_SYNCHRONOUS_MANUAL_ANSWER:
            //event->Signal();
            this->signalEventByDispatcher(event);
            break;
        case TemplateEvent::HANDLING_ASYNCHRONOUS_MANUAL_ANSWER:
            //event->Signal();
            if (NULL != event->m_remoteController) {
                event->m_remoteController->PostAnswer(event);
            }
            this->signalEventByDispatcher(event);
            break;
        default:
            break;
        }
    }
};

template<class TemplateEvent>
class EventAnswerReceiver : private EventReceiver<TemplateEvent>
{
  public:
    EventAnswerReceiver(ThreadEnum::Enumeration threadType) : EventReceiver<
            TemplateEvent>(threadType)
    {}

    virtual void OnAnswerReceived(const DPL::SharedPtr<TemplateEvent> &)
    { LogError("Pure virtual function call"); } //TODO: temporary code: DPL::SharedPtr -> std::shared_ptr

    virtual void OnAnswerReceived(const std::shared_ptr<TemplateEvent> &)
    { LogError("Pure virtual function call"); }

    //it should be hidden outside, but I can't do it! I can't! :|
    void PostAnswer(const DPL::SharedPtr<TemplateEvent> &event)
    {
        LogDebug(__FUNCTION__);
        event->signalCancelStatusFlag();
        DPL::Event::ControllerEventHandler<DPL::SharedPtr<TemplateEvent> >::
            PostEvent(
            event);
    }

    void OnEventReceived(const DPL::SharedPtr<TemplateEvent> &event)
    {
        _D("EventAnswerReceiver: answer received");
        //check if it can be processed and set the state
        {
            DPL::Mutex::ScopedLock lock(&event->m_stateMutex);

            //in case someone changed it to synchronous call, we don't process
            // it
            if (TemplateEvent::STATE_CHANGED_TO_SYNCHRONOUS ==
                event->m_state || TemplateEvent::STATE_ENDED ==
                event->m_state)
            {
                return;
            }
            //we should get cancelled or answer_send state here
            assert(
                TemplateEvent::STATE_ANSWER_SEND == event->m_state &&
                "Wrong state!");

            if (event->m_cancelled && event->m_cancelAllowed) {
                event->handleCancel();
                event->signalFinishedFlag();
                return;
            }
            event->m_state = TemplateEvent::STATE_ANSWER_RECEIVED;
        }

        OnAnswerReceived(event);

        {
            DPL::Mutex::ScopedLock lock(&event->m_stateMutex);
            assert(TemplateEvent::STATE_ANSWER_RECEIVED == event->m_state);
            event->m_state = TemplateEvent::STATE_ENDED;
            delete event->m_cancelStatusFlag;
            event->m_cancelStatusFlag = NULL;
            //if someone is waiting
            event->signalFinishedFlag();
        }
    }
};
}
} // WrtDeviceApisCommon

#endif /* WRTDEVICEAPIS_COMMONS_EVENT_RECEIVER_H_ */
