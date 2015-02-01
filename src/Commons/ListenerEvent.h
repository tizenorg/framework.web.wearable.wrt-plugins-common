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
 * @author    Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef WRTDEVICEAPIS_COMMONS_LISTENER_EVENT_H_
#define WRTDEVICEAPIS_COMMONS_LISTENER_EVENT_H_

#include <dpl/mutex.h>
#include <dpl/shared_ptr.h>
#include <dpl/noncopyable.h>
#include <Commons/Exception.h>
#include <Commons/IEvent.h>

namespace WrtDeviceApis {
namespace Commons {
template<class EventClass>
class ListenerEventEmitter;

/**
 * This is base class for events that should act as signals between abstract
 * layer and layer that uses it (e.g. JS layer in WRT plugins).
 * Such event is created by specific event emitter from abstract layer
 * and passed asynchronously to object that acts as event listener.
 * Template parameter should be class of an event which derives from
 * this class (co called CRTP pattern).
 */
template<class Derived>
class ListenerEvent
{
    friend class ListenerEventEmitter<Derived>;

  public:
    typedef IEventPrivateData PrivateDataType;
    typedef DPL::SharedPtr<PrivateDataType> PrivateDataTypePtr;

  public:
    virtual ~ListenerEvent()
    {
        delete m_mtx;
    }

    /**
     * Gets exception code.
     * @return Exception code.
     */
    Commons::ExceptionCodes::Enumeration getExceptionCode() const
    {
        DPL::Mutex::ScopedLock lock(m_mtx);
        return m_code;
    }

    /**
     * Sets exception code.
     * @param code Exception code.
     * @throw EventWrongStateExeption When event has already been emitted.
     */
    void setExceptionCode(Commons::ExceptionCodes::Enumeration code)
    {
        DPL::Mutex::ScopedLock lock(m_mtx);
        m_code = code;
    }

    /**
     * Gets event's private data.
     * @return Private data (use DPL cast to proper object).
     */
    const PrivateDataTypePtr& getPrivateData() const
    {
        DPL::Mutex::ScopedLock lock(m_mtx);
        return m_privateData;
    }

  protected:
    ListenerEvent() :
        m_mtx(new DPL::Mutex()),
        m_code(Commons::ExceptionCodes::None)
    {}

    ListenerEvent(const ListenerEvent &ths) :
        m_mtx(new DPL::Mutex()),
        m_code(ths.m_code),
        m_privateData(ths.m_privateData)
    {}

    ListenerEvent& operator=(const ListenerEvent &other)
    {
        if (this != &other) {
            m_mtx = new DPL::Mutex();
            m_code = other.m_code;
            m_privateData = other.m_privateData;
        }
        return *this;
    }

    /**
     * Sets event's private data.
     * Event's private data object has to implement @see IEventPrivateData
     * interface.
     * @param data Private data.
     * @throw EventWrongStateExeption When event has already been emitted.
     */
    void setPrivateData(const PrivateDataTypePtr& data)
    {
        DPL::Mutex::ScopedLock lock(m_mtx);
        m_privateData = data;
    }

  protected:
    mutable DPL::Mutex *m_mtx;
    Commons::ExceptionCodes::Enumeration m_code; ///< Exception code.
    PrivateDataTypePtr m_privateData; ///< Private data.
};
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_LISTENER_EVENT_H_
