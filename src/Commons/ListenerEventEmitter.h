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

#ifndef WRTDEVICEAPIS_COMMONS_LISTENER_EVENT_EMITTER_H_
#define WRTDEVICEAPIS_COMMONS_LISTENER_EVENT_EMITTER_H_

#include <stdint.h>
#include <dpl/noncopyable.h>
#include <dpl/shared_ptr.h>
#include <dpl/mutex.h>
#include <Commons/EventListener.h>
#include <Commons/ListenerEvent.h>

namespace WrtDeviceApis {
namespace Commons {
template<class EmitterClass>
class Emitters;

/**
 * Creates listener events in the abstraction layer in response to some
 * asynchronous action.
 * To enable passing events from abstract layer to layer that uses it, first
 * proper event emitter should be registered in abstract layer by the layer
 * that uses it. Then when some action happens in abstract layer event should be
 * created and passed to this emitter's emit() function. Then emitter passes
 * this event to proper listener.
 * Template parameter should be class that derives from @see ListenerEvent.
 */
template<class EventClass>
class ListenerEventEmitter : private DPL::Noncopyable
{
  public:
    typedef EventClass EventType;
    typedef DPL::SharedPtr<EventType> EventPtrType;
    typedef ListenerEventEmitter<EventType> Type;
    typedef typename ListenerEvent<EventType>::PrivateDataType
    EventPrivateDataType;
    typedef typename ListenerEvent<EventType>::PrivateDataTypePtr
    EventPrivateDataTypePtr;
    typedef EventListener<EventType> ListenerType;
    typedef uintptr_t IdType;

    /**
     * Empty (NULL) value of emitter's Id.
     */
    static const IdType emptyId;

  public:
    ListenerEventEmitter() : m_listener(NULL)
    {}

  public:
    virtual ~ListenerEventEmitter()
    {}

    /**
     * Sets event's private data.
     * Event's private data object has to implement @see IEventPrivateData
     * interface.
     * @param data Private data.
     * @remarks Practically private dat should be only set at object creation
     * and
     *          not chaged during this object lifetime.
     */
    virtual void setEventPrivateData(const EventPrivateDataTypePtr& data)
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        m_privateData = data;
    }

    /**
     * Gets event's private data.
     * @return Private data.
     */
    virtual EventPrivateDataTypePtr getEventPrivateData()
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        return m_privateData;
    }

    /**
     * Sets listener.
     * Object set as listener has to implement @see EventListener interface.
     * @param listener Listener object.
     * @remarks Doesn't take ownership over this object.
     *          It's suggested to use singletons to have one listener for all
     *          events (no dynamic allocation overhead).
     */
    virtual void setListener(ListenerType* listener)
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        m_listener = listener;
    }

    /**
     * Emits event.
     * @param event Event to emit.
     */
    virtual void emit(const EventPtrType& event)
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        EventPtrType copy(new EventType(*event.Get()));
        if (m_listener) {
            copy->setPrivateData(m_privateData);
            m_listener->postAnswer(copy);
        }
    }

    /**
     * Gets id.
     * @return Event's id.
     * @remarks Id is implemented as value of `this` pointer.
     */
    virtual IdType getId()
    {
        return reinterpret_cast<IdType>(this);
    }

  protected:
    DPL::Mutex m_mtx; ///< Mutex for thread-safety.
    ListenerType* m_listener; ///< Event listener object.
    EventPrivateDataTypePtr m_privateData; ///< Private data.
};

template<class EventClass>
const typename ListenerEventEmitter<EventClass>::IdType ListenerEventEmitter<
    EventClass>::emptyId = 0;
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_LISTENER_EVENT_EMITTER_H_
