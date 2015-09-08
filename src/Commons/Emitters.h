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
 * @author       Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef WRTDEVICEAPIS_COMMONS_EMITTERS_H_
#define WRTDEVICEAPIS_COMMONS_EMITTERS_H_

#include <cstddef>
#include <map>
#include <memory>
#include <dpl/mutex.h>
#include <dpl/shared_ptr.h>
#include <Commons/ListenerEventEmitter.h>

namespace WrtDeviceApis {
namespace Commons {
/**
 * Manages listener events emitters.
 * Template parameter should be class that derives from @see ListenerEvent.
 */
template<class EmitterClass>
class Emitters
{
  public:
    typedef EmitterClass EmitterType;
    typedef DPL::SharedPtr<EmitterType>           EmitterPtrType;
    typedef typename EmitterType::IdType EmitterIdType;
    typedef typename EmitterType::EventType EventType;
    typedef typename EmitterType::EventPtrType EventPtrType;
    typedef std::auto_ptr<DPL::Mutex::ScopedLock> LockType;

  public:
    ~Emitters()
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        m_emitters.clear();
    }

    /**
     * Attaches emitter.
     * @param emitter Emitter.
     * @remarks Thread-safe. Do not use it in the scope of getLock() result.
     */
    void attach(const EmitterPtrType& emitter)
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        m_emitters[emitter->getId()] = emitter;
    }

    /**
     * Detaches emitter.
     * @param id Id of an emitter.
     * @return True when emitter was found and successfully detached,
     *         false otherwise.
     * @remarks Thread-safe. Do not use it in the scope of getLock() result.
     */
    bool detach(const EmitterIdType id)
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        return (m_emitters.erase(id) > 0);
    }

    /**
     * Emits event through all emitters.
     * @param event Event to emit.
     * @remarks Thread-safe. Do not use it in the scope of getLock() result.
     */
    void emit(const EventPtrType& event)
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        for (Iterator it = m_emitters.begin(); it != m_emitters.end(); ++it) {
            it->second->emit(event);
        }
    }

    /**
     * Emits event through those emitters that when passed to predicate result
     * in
     * returning true by it.
     * @param event Event to emit.
     * @param pred Predicate - a callable object (function, functor) that takes
     *             an argument of type EmitterPtrType and returns boolean value.
     */
    template<typename Predicate>
    void emitIf(const EventPtrType& event,
                Predicate pred)
    {
        DPL::Mutex::ScopedLock lock(&m_mtx);
        for (Iterator it = m_emitters.begin(); it != m_emitters.end(); ++it) {
            if (bool(pred(it->second))) {
                it->second->emit(event);
            }
        }
    }

    /**
     * Checks whether emitter of supplied id is attached.
     * @param emitter Emitter.
     * @return True when emitter has been found, false otherwise.
     * @remarks Can be used within scope of getLock() call.
     */
    bool isAttached(const EmitterIdType id) const
    {
        return (m_emitters.count(id) > 0);
    }

    /**
     * Returns number of attached emitters.
     * @return Number of emitters.
     * @remarks Can be used within scope of getLock() call.
     */
    std::size_t size() const
    {
        return m_emitters.size();
    }

    /**
     * Lock this object.
     * This lock will be automatically released when out of scope (unless
     * someone
     * copies it). Do not use in the same scope as other API of this class.
     * @return Lock object.
     * @remarks Provided to allow locking emitters in scope of some client
     *          code.
     */
    LockType getLock()
    {
        return LockType(new DPL::Mutex::ScopedLock(&m_mtx));
    }

  private:
    typedef std::map<EmitterIdType, EmitterPtrType> Map;
    typedef typename Map::iterator Iterator;
    typedef typename Map::const_iterator ConstIterator;

  private:
    DPL::Mutex m_mtx; ///< Synchronizes operation on this object.
    Map m_emitters; ///< Emitters container.
};
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_EMITTERS_H_
