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

#ifndef WRTDEVICEAPIS_COMMONS_IEVENT_H_
#define WRTDEVICEAPIS_COMMONS_IEVENT_H_

#include <assert.h>
#include <dpl/event/controller.h>
#include <dpl/mutex.h>
#include <dpl/shared_ptr.h>
#include <Commons/Exception.h>
#include <Commons/EventReceiver.h>

namespace WrtDeviceApis {
namespace Commons {
class IEventPrivateData
{
  public:
    virtual ~IEventPrivateData()
    {}
};

class IEventController
{
    DPL::SharedPtr<IEventPrivateData> m_privateData;

  public:
    virtual void waitTillProcessed() = 0;
    virtual void waitForAnswer() = 0;
    virtual bool cancelRequest()
    {
        return false;
    }
    virtual bool changeCallToSynchronous() = 0;
    virtual ~IEventController()
    {}

    void setPrivateData(
        const DPL::SharedPtr<IEventPrivateData> &privateData)
    {
        m_privateData = privateData;
    }
    const DPL::SharedPtr<IEventPrivateData>& getPrivateData()
    {
        return m_privateData;
    }
};
typedef DPL::SharedPtr<IEventController> IEventControllerPtr;

// CRTP pattern
template<class Super>
class IEvent : /*private DPL::WaitableEvent, */ public IEventController
{
  public:
    friend class EventRequestReceiver<Super>;
    friend class EventAnswerReceiver<Super>;
    friend class SignalEventCall<Super>;
    friend class EventReceiver<Super>;

    enum HandlingType
    {
        HANDLING_NOT_SET = -1,
        HANDLING_SYNCHRONOUS = 0,
        HANDLING_ASYNCHRONOUS = 1,
        HANDLING_SYNCHRONOUS_MANUAL_ANSWER = 2,
        HANDLING_ASYNCHRONOUS_MANUAL_ANSWER = 3
    };

    enum State
    {
        STATE_INITIAL,
        STATE_REQUEST_SEND,
        STATE_REQUEST_RECEIVED,
        STATE_ANSWER_SEND,
        STATE_ANSWER_RECEIVED,
        STATE_ENDED,
        STATE_CHANGED_TO_SYNCHRONOUS
    };

  private:
    void handleCancel()
    {
        clearOnCancel();
        m_state = STATE_ENDED;
        m_exceptionCode = Commons::ExceptionCodes::EventCancelledException;
        //if someone is waiting
        signalSynchronousEventFlag();
    }

  protected:
    DPL::Mutex m_stateMutex;
    State m_state;
    HandlingType m_handlingType;
    EventAnswerReceiver< Super > *m_remoteController;
    Commons::ExceptionCodes::Enumeration m_exceptionCode;
    DPL::WaitableEvent *m_synchronousEventFlag;
    DPL::WaitableEvent *m_finishedFlag;
    DPL::WaitableEvent *m_cancelStatusFlag;
    bool m_cancelSignalAhead;
    bool m_cancelled;
    bool m_cancelAllowed;

    IEvent() :
        m_state(STATE_INITIAL),
        m_handlingType(HANDLING_NOT_SET),
        m_remoteController(NULL),
        m_exceptionCode(Commons::ExceptionCodes::None),
        m_synchronousEventFlag(NULL),
        m_finishedFlag(NULL),
        m_cancelStatusFlag(NULL),
        m_cancelSignalAhead(false),
        m_cancelled(false),
        m_cancelAllowed(false)
    {}

    virtual void waitForAnswer()
    {
        assert(HANDLING_SYNCHRONOUS == m_handlingType);
        DPL::WaitForSingleHandle(m_synchronousEventFlag->GetHandle());
        {
            DPL::Mutex::ScopedLock lock(&m_stateMutex);
            m_state = STATE_ENDED;
        }
        signalFinishedFlag();
        LogDebug("deleting m_processEvent");
        delete m_synchronousEventFlag;
        m_synchronousEventFlag = NULL;
    }

    void signalFinishedFlag()
    {
        if (m_finishedFlag) {
            m_finishedFlag->Signal();
        }
    }

    DPL::WaitableEvent &getFinishedFlag()
    {
        if (!m_finishedFlag) {
            m_finishedFlag = new DPL::WaitableEvent();
        }
        return *m_finishedFlag;
    }

    void signalCancelStatusFlag()
    {
        LogDebug("signaling cancel");
        DPL::Mutex::ScopedLock lock(&m_stateMutex);
        m_cancelSignalAhead = true;
        if (m_cancelStatusFlag) {
            m_cancelStatusFlag->Signal();
        }
    }

    DPL::WaitableEvent &getCancelStatusFlag()
    {
        if (!m_cancelStatusFlag) {
            m_cancelStatusFlag = new DPL::WaitableEvent();
        }
        return *m_cancelStatusFlag;
    }

    void signalSynchronousEventFlag()
    {
        if (m_synchronousEventFlag) {
            m_synchronousEventFlag->Signal();
        }
    }

  public:

    /*
     * Gets the answer receiver pointer.
     */
    EventAnswerReceiver< Super > * getAnswerReceiverRef() const
    {
        return m_remoteController;
    }

    virtual ~IEvent()
    {
        delete m_cancelStatusFlag;
        delete m_finishedFlag;
        delete m_synchronousEventFlag;
    }

    virtual bool changeCallToSynchronous()
    {
        return setForSynchronousCall();
    }

    virtual void waitTillProcessed()
    {
        DPL::WaitForSingleHandle(getFinishedFlag().GetHandle());
        delete m_finishedFlag;
        m_finishedFlag = NULL;
    }

    virtual void clearOnCancel()
    {}

    Commons::ExceptionCodes::Enumeration getExceptionCode() const
    {
        return m_exceptionCode;
    }
    void setExceptionCode(Commons::ExceptionCodes::Enumeration exceptionCode)
    {
        m_exceptionCode = exceptionCode;
    }

    short getHandlingType() const
    {
        return m_handlingType;
    }

    virtual bool setForSynchronousCall()
    {
        DPL::Mutex::ScopedLock lock(&m_stateMutex);
        if (m_cancelled) {
            return false;
        }
        switch (m_state) {
        case STATE_ANSWER_SEND:
            m_state = STATE_CHANGED_TO_SYNCHRONOUS;
            break;
        case STATE_ANSWER_RECEIVED:
        case STATE_ENDED:
            return false;
        default:
            break;
        }
        m_handlingType = HANDLING_SYNCHRONOUS;
        return true;
    }

    virtual bool setForAsynchronousCall(
        EventAnswerReceiver< Super > *remoteController)
    {
        DPL::Mutex::ScopedLock lock(&m_stateMutex);
        if (m_cancelled) {
            return false;
        }
        switch (m_state) {
        case STATE_ANSWER_SEND:
        case STATE_ANSWER_RECEIVED:
        case STATE_ENDED:
            return false;
        default:
            break;
        }
        m_handlingType = HANDLING_ASYNCHRONOUS;
        m_remoteController = remoteController;
        return true;
    }

    /*
     * Normally, after invoking OnRequestReceived in RequestReceiver, the answer
     * is being send automatically (after flow leaves OnRequestReceived).
     * After calling this function, the answer is not being send automatically,
     * you need to call ManualAnswer to send event back.
     * It works both in asynchronous and synchronous handling type.
     */
    virtual bool switchToManualAnswer()
    {
        assert(
            m_handlingType == HANDLING_ASYNCHRONOUS || m_handlingType ==
            HANDLING_SYNCHRONOUS);

        DPL::Mutex::ScopedLock lock(&m_stateMutex);
        if (m_cancelled) {
            return false;
        }
        switch (m_state) {
        case STATE_ANSWER_SEND:
        case STATE_ANSWER_RECEIVED:
        case STATE_ENDED:
            return false;
        default:
            break;
        }

        switch (m_handlingType) {
        case HANDLING_ASYNCHRONOUS:
            m_handlingType = HANDLING_ASYNCHRONOUS_MANUAL_ANSWER;
            break;
        case HANDLING_SYNCHRONOUS:
            m_handlingType = HANDLING_SYNCHRONOUS_MANUAL_ANSWER;
            break;
        default:
            break;
        }
        return true;
    }

    bool checkCancelled()
    {
        //DPL::Mutex::ScopedLock lock(&m_stateMutex);
        return m_cancelled;
    }

    void tryCancelled()
    {
        //DPL::Mutex::ScopedLock lock(&m_stateMutex);
        if (m_cancelled) {
            Throw(Commons::EventCancelledException);
        }
    }

    bool getCancelAllowed() const
    {
        return m_cancelAllowed;
    }

    void setCancelAllowed(bool cancelAllowed)
    {
        m_cancelAllowed = cancelAllowed;
    }

    bool cancelRequest()
    {
        LogDebug("trying to cancel");
        assert(HANDLING_ASYNCHRONOUS == m_handlingType ||
               HANDLING_ASYNCHRONOUS_MANUAL_ANSWER == m_handlingType);
        bool signaled = false;
        {
            DPL::Mutex::ScopedLock lock(&m_stateMutex);
            if (m_cancelled) {
                return false;
            }
            switch (m_state) {
            case STATE_INITIAL:
                assert(0);
            case STATE_ANSWER_SEND:
                LogDebug("cancelling");
                m_cancelled = true;
                delete m_cancelStatusFlag;
                m_cancelStatusFlag = NULL;
                return m_cancelAllowed;
            case STATE_ANSWER_RECEIVED:
            case STATE_ENDED:
                return false;
            default:
                break;
            }
            LogDebug("cancelling");
            m_cancelled = true;
            signaled = m_cancelSignalAhead;
            if (!signaled) {
                //create waitable handle
                getCancelStatusFlag();
            }
        }
        LogDebug("waiting for cancel flag");
        if (!signaled) {
            DPL::WaitForSingleHandle(getCancelStatusFlag().GetHandle());
        }
        delete m_cancelStatusFlag;
        m_cancelStatusFlag = NULL;
        return m_cancelAllowed;
    }
};
}
} // WrtDeviceApisCommon

#endif /* WRTDEVICEAPIS_COMMONS_IEVENT_H_ */
