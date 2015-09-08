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
 *
 * @file       js_overlay_addEventListener.h
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @author     Yunchan Cho (yunchan.cho@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef _WRT_PLUGINS_SRC_W3C_WIDGET_ADDEVENTLISTENERSUPPORT_H_
#define _WRT_PLUGINS_SRC_W3C_WIDGET_ADDEVENTLISTENERSUPPORT_H_

#include <map>
#include <list>
#include <memory>
#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/JSContextRef.h>
#include <StorageEvent/IStorageEvent.h>
#include <TizenServiceEvent/ITizenServiceEvent.h>
#include <js_overlay_types.h>
#include <JSObject.h>

// import temporarily from JSContextRefPrivate.h
extern "C" JSGlobalContextRef JSContextGetGlobalContext(JSContextRef ctx);

namespace WrtPlugins {
namespace W3C {
class AddEventListenerSupport
{
  public:
    static JSValueRef AddEventListener(JSContextRef context,
                                       JSObjectRef object,
                                       JSObjectRef thisObject,
                                       size_t argumentCount,
                                       const JSValueRef arguments[],
                                       JSValueRef* exception);

    static void RemoveIFrame(JSObjectPtr iframe);

    static void CallStorageListenersFromDifferentIFrames(
        JSObjectRef iframe,
        const WrtDeviceApis::StorageEvent::Api::IStorageEventPtr& event);

    static void CallCustomEventListenersFromIFrame(
        JSObjectRef iframe,
        CustomEventType eventType,
        JSObjectRef eventObject);

  private:

    struct CallbackData {
        CustomEventType eventType;
        JSObjectRef object;
        JSObjectRef thisObject;
        JSContextRef context;
    };

    typedef std::list<CallbackData> Listeners;
    typedef std::shared_ptr<Listeners> ListenersPtr;

    //first-iframe, 2nd - listeners list
    typedef std::map<JSObjectRef, ListenersPtr> IFramesListeners;

    static IFramesListeners m_listeners;

  private:
    static ListenersPtr getIFrameListeners(JSObjectRef iframe);
};
}
}

#endif
