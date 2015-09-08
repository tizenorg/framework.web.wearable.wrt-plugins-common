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
 * ControllersFactory.h
 *
 *  Created on: 2010-06-28
 *      Author: kmajewski
 */

#ifndef WRTDEVICEAPIS_COMMONS_THREADPOOL_H_
#define WRTDEVICEAPIS_COMMONS_THREADPOOL_H_

#include <map>
#include <dpl/thread.h>
#include <dpl/event/main_event_dispatcher.h>
#include <dpl/event/thread_event_dispatcher.h>
#include <dpl/event/event_support.h>
#include <dpl/mutex.h>

namespace WrtDeviceApis {
namespace Commons {
class ThreadEnum
{
  private:
    ThreadEnum()
    {}

  public:
    enum Enumeration
    {
        NULL_THREAD = 0,
        CAMERA_THREAD,
        CALENDAR_THREAD,
        TELEPHONY_THREAD,
        APPLAUNCHER_THREAD,
        MESSAGING_THREAD,
        FILESYSTEM_THREAD,
        UI_THREAD,
        APPCONFIG_THREAD,
        GALLERY_THREAD,
        CONTACT_THREAD,
        BONDI_THREAD,
        GEOLOCATION_THREAD,
        DEVICESTATUS_THREAD,
        PROFILE_THREAD,
        HAPTICS_THREAD,
        ACCELEROMETER_THREAD,
        ORIENTATION_THREAD,
        TASK_THREAD,
        POWER_THREAD,
        PLUGIN_TEMPLETE_THREAD,
        BLUETOOTH_THREAD,
        APPLICATION_THREAD,
        GYROSCOPE_THREAD,
        CLOCK_THREAD,
        SYSTEMINFO_THREAD,
        CALLHISTORY_THREAD,
        ACCOUNT_THREAD,
        NFC_THREAD,
        MEDIACONTENT_THREAD,
        SE_THREAD,
        DOWNLOAD_THREAD,
        PUSH_THREAD,
        SYNC_THREAD,
        //....
        size
    };
};

class ThreadPool : private DPL::Noncopyable
{
  private:
    typedef std::map<ThreadEnum::Enumeration, DPL::Thread*> ThreadHandleMap;
    ThreadHandleMap m_threadHandlers;
    DPL::Mutex m_threadHandlersMutex;

    DPL::Thread *getThreadHandleCreateIfNotExists(ThreadEnum::Enumeration type);

    //DPL::MainEventDispatcher m_mainEventDispatcher;

    ThreadPool();

  public:
    ~ThreadPool();

    /*
     * @throws:  WrtDeviceApis::Commons::InvalidArgumentException
     */
    DPL::Thread * getThreadRef(ThreadEnum::Enumeration type);

    /*
     * Get proper dispatcher for current calling thread
     */
    DPL::Event::AbstractEventDispatcher *getDispatcher(
        DPL::Event::ThreadEventDispatcher &callingThreadDispatcher);

    static ThreadPool& getInstance();
};
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_THREADPOOL_H_
