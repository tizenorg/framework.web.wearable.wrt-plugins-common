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
 * ControllersFactory.cpp
 *
 *  Created on: 2010-06-28
 *      Author: kmajewski
 */

#include <assert.h>

#include <dpl/log/log.h>
#include <dpl/thread.h>

#include "ThreadPool.h"
#include "Exception.h"

namespace WrtDeviceApis {
namespace Commons {
ThreadPool &ThreadPool::getInstance()
{
    static ThreadPool theInstance;
    return theInstance;
}

DPL::Event::AbstractEventDispatcher *ThreadPool::getDispatcher(
    DPL::Event::ThreadEventDispatcher &callingThreadDispatcher)
{
    DPL::Thread *currentThread = DPL::Thread::GetCurrentThread();
    //if we are in main thread
    if (NULL == currentThread) {
        return &DPL::Event::GetMainEventDispatcherInstance();
    }
    callingThreadDispatcher.SetThread(currentThread);
    return &callingThreadDispatcher;
}

ThreadPool::ThreadPool()
{}

ThreadPool::~ThreadPool()
{
    for (ThreadHandleMap::const_iterator it = m_threadHandlers.begin(); it
         != m_threadHandlers.end(); ++it)
    {
        delete (*it).second;
    }
}

DPL::Thread* ThreadPool::getThreadHandleCreateIfNotExists(
    ThreadEnum::Enumeration type)
{
    DPL::Mutex::ScopedLock lock(&m_threadHandlersMutex);
    ThreadHandleMap::iterator element = m_threadHandlers.find(type);

    //if element does not exists in the map
    if (m_threadHandlers.end() == element) {
        DPL::Thread* newThread = new DPL::Thread();
        m_threadHandlers.insert(
            m_threadHandlers.begin(),
            std::pair<ThreadEnum::Enumeration, DPL::Thread*>(type, newThread));
        newThread->Run();
        return newThread;
    }
    return (*element).second;
}

DPL::Thread *ThreadPool::getThreadRef(ThreadEnum::Enumeration type)
{
    DPL::Thread *thandle = NULL;
    if (type < 0 || type >= ThreadEnum::size) {
        Throw(InvalidArgumentException);
    }
    /* we could stay with
     * getThreadHandleCreateIfNotExists(type);
     * but by switch we can attach actually one thread to more than one
     * ThreadEnums
     */
    switch (type) {
    case ThreadEnum::NULL_THREAD:
        thandle = NULL;
        break;
    case ThreadEnum::CAMERA_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::CAMERA_THREAD);
        break;
    case ThreadEnum::CALENDAR_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::CALENDAR_THREAD);
        break;
    case ThreadEnum::TELEPHONY_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::TELEPHONY_THREAD);
        break;
    case ThreadEnum::APPLAUNCHER_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::APPLAUNCHER_THREAD);
        break;
    case ThreadEnum::APPCONFIG_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::APPCONFIG_THREAD);
        break;
    case ThreadEnum::MESSAGING_THREAD:
        thandle = getThreadHandleCreateIfNotExists(type);
        break;
    case ThreadEnum::FILESYSTEM_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::FILESYSTEM_THREAD);
        break;
    case ThreadEnum::GALLERY_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::GALLERY_THREAD);
        break;
    case ThreadEnum::CONTACT_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::CONTACT_THREAD);
        break;
    case ThreadEnum::BONDI_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::BONDI_THREAD);
        break;
    case ThreadEnum::GEOLOCATION_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::GEOLOCATION_THREAD);
        break;
    case ThreadEnum::DEVICESTATUS_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::DEVICESTATUS_THREAD);
        break;
    case ThreadEnum::PROFILE_THREAD:
        thandle = getThreadHandleCreateIfNotExists(type);
        break;
    case ThreadEnum::HAPTICS_THREAD:
        thandle = getThreadHandleCreateIfNotExists(type);
        break;
    case ThreadEnum::ACCELEROMETER_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::ACCELEROMETER_THREAD);
        break;
    case ThreadEnum::ORIENTATION_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::ORIENTATION_THREAD);
        break;
    case ThreadEnum::TASK_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::TASK_THREAD);
        break;
    case ThreadEnum::POWER_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::POWER_THREAD);
        break;
    case ThreadEnum::PLUGIN_TEMPLETE_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::PLUGIN_TEMPLETE_THREAD);
        break;
    case ThreadEnum::BLUETOOTH_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::BLUETOOTH_THREAD);
        break;
    case ThreadEnum::APPLICATION_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::APPLICATION_THREAD);
        break;
    case ThreadEnum::GYROSCOPE_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::GYROSCOPE_THREAD);
        break;
    case ThreadEnum::CLOCK_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::CLOCK_THREAD);
        break;
    case ThreadEnum::SYSTEMINFO_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::SYSTEMINFO_THREAD);
        break;
    case ThreadEnum::CALLHISTORY_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::CALLHISTORY_THREAD);
        break;
    case ThreadEnum::ACCOUNT_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::CALLHISTORY_THREAD);
        break;
    case ThreadEnum::NFC_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::NFC_THREAD);
        break;
    case ThreadEnum::MEDIACONTENT_THREAD:
        thandle = getThreadHandleCreateIfNotExists(
                ThreadEnum::MEDIACONTENT_THREAD);
        break;
    case ThreadEnum::SE_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::SE_THREAD);
        break;
    case ThreadEnum::DOWNLOAD_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::DOWNLOAD_THREAD);
        break;
    case ThreadEnum::PUSH_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::PUSH_THREAD);
        break;
    case ThreadEnum::SYNC_THREAD:
        thandle = getThreadHandleCreateIfNotExists(ThreadEnum::SYNC_THREAD);
        break;
    // .....
    default:
        LogError("no case statement for ThreadEnum");
        Throw(InvalidArgumentException);
    }
    return thandle;
}
}
} // WrtDeviceApisCommon
