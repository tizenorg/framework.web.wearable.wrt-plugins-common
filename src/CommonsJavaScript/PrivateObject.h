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
#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_PRIVATEOBJECT_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_PRIVATEOBJECT_H_

#include <JavaScriptCore/JavaScript.h>
#include <dpl/noncopyable.h>
#include <dpl/assert.h>
#include <Commons/Exception.h>
#include <Commons/TypeTraits.h>
#include <CommonsJavaScript/JSLifeManager.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
template<class T>
struct NoAcquire
{
    void acquire(T*)
    {}

  protected:
    ~NoAcquire()
    {}
};

template<class T>
struct AcquireByProtect
{
    void acquire(T* object)
    {
        Assert(object && "Object passed to protect can't be NULL.");
        JSValueSafeProtect(object->getContext(), object->getObject());
    }

  protected:
    ~AcquireByProtect()
    {}
};

template<class T>
struct NoRelease
{
    void release(T* object)
    {
        (void)object;
    }

  protected:
    ~NoRelease()
    {}
};

template<class T>
struct ReleaseByDelete
{
    void release(T* object)
    {
        delete object->getObject();
    }

  protected:
    ~ReleaseByDelete()
    {}
};

template<class T>
struct ReleaseByUnprotect
{
    void release(T* object)
    {
        Assert(object && "Object passed to unprotect can't be NULL.");
        JSValueSafeUnprotect(object->getContext(), object->getObject());
    }

  protected:
    ~ReleaseByUnprotect()
    {}
};

template<class T>
struct NoOwnership : protected NoAcquire<T>,
    protected NoRelease<T>
{
  protected:
    ~NoOwnership()
    {}
};

template<class T>
struct OwnershipByAcquisition : protected NoAcquire<T>,
    protected ReleaseByDelete<T>
{
  protected:
    ~OwnershipByAcquisition()
    {}
};

template<class T>
struct OwnershipByProtection : protected AcquireByProtect<T>,
    protected ReleaseByUnprotect<T>
{
  protected:
    ~OwnershipByProtection()
    {}
};

template<class PrivateClass,
         template <class> class OwnershipPolicy = OwnershipByAcquisition>
class PrivateObject : public DPL::Noncopyable,
    protected OwnershipPolicy<PrivateObject<PrivateClass, OwnershipPolicy> >
{
  public:
    typedef PrivateClass ObjectType;

  public:
    /**
     * Creates storage object for JS private data.
     * @param context JS (root/global) context.
     * @param object Object to store.
     * @throw NullPointerException When object is pointer and is set to NULL.
     */
    PrivateObject(JSContextRef context,
                  const PrivateClass& object) :
        m_context(context),
        m_object(object)
    {
        Assert(NULL != m_context && "Context is NULL.");
        Assert(!Commons::IsNull<PrivateClass>::value(
                   object) && "Object is NULL.");
        this->acquire(this);
    }

    /**
     * Destroys instance of the object.
     */
    virtual ~PrivateObject()
    {
        this->release(this);
    }

    /**
     * Gets stored JS context.
     * @return JavaScript context.
     */
    virtual JSContextRef getContext() const
    {
        return m_context;
    }

    /**
     * Gets stored object.
     * @return Stored object.
     */
    virtual PrivateClass getObject() const
    {
        return m_object;
    }

  protected:
    JSContextRef m_context; ///< JS context.
    PrivateClass m_object; ///< Stored object.
};

/**
 * Specialization for type void.
 */
template<>
class PrivateObject<void, NoOwnership> : private DPL::Noncopyable
{
  public:
    /**
     * Creates storage object for JS private data.
     * @param context JS (root/global) context.
     * @remarks Takes ownership over stored object.
     */
    explicit PrivateObject(JSContextRef context) : m_context(context)
    {
        Assert(NULL != m_context && "Context is NULL.");
    }

    /**
     * Destroys instance of the object.
     */
    virtual ~PrivateObject()
    {}

    /**
     * Gets stored JS context.
     * @return JavaScript context.
     */
    virtual JSContextRef getContext() const
    {
        return m_context;
    }

  protected:
    JSContextRef m_context;
};

template<class C>
struct PrivateObjectT
{
    typedef PrivateObject<C, NoOwnership> Type;
};

template<class C>
struct PrivateObjectT<C*>
{
    typedef PrivateObject<C*, OwnershipByAcquisition> Type;
};

template<>
struct PrivateObjectT<JSObjectRef>
{
    typedef PrivateObject<JSObjectRef, NoOwnership> Type;
};

template<>
struct PrivateObjectT<void>
{
    typedef PrivateObject<void, NoOwnership> Type;
};
} // CommonsJavaScript
} // WrtDeviceApis

#endif /* PRIVATEOBJECT_H_ */
