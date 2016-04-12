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
 * @file       AddEventListenerSupport.cpp
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @author     Yunchan Cho (yunchan.cho@samsung.com)
 * @version    0.1
 * @brief
 */

#include <string>
#include <dpl/log/log.h>
#include <dpl/foreach.h>
#include <CommonsJavaScript/Converter.h>
#include <js_overlay_addEventListener.h>
#include <js_overlay_types.h>
#include <JSStorageEvent.h>
#include <CommonsJavaScript/JSLifeManager.h>

namespace WrtPlugins {
namespace W3C {

const std::string storageEventName = "storage";
const std::string appServiceEventName = "appservice";
const std::string softKeyboardChangeEventName = "softkeyboardchange";

AddEventListenerSupport::IFramesListeners
AddEventListenerSupport::m_listeners =
    AddEventListenerSupport::IFramesListeners();

JSValueRef AddEventListenerSupport::AddEventListener(
        JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* /*exception*/)
{
    LogDebug("Add event listener invoked");
    LogDebug("This(iframe?):" << thisObject);
    LogDebug("object:" << object);

    JSContextRef global_context = JSContextGetGlobalContext(context);
    LogDebug("context:" << context << "(global:" << global_context << ")");

    if (argumentCount < 2 || !JSValueIsString(context, arguments[0])) {
        LogError("Wrong arguments");
        return JSValueMakeUndefined(context);
    }

    std::string eventName = WrtDeviceApis::CommonsJavaScript::
            Converter(context).toString(arguments[0]);
    LogDebug("Event name: " << eventName);

    if (eventName != storageEventName &&
        eventName != appServiceEventName &&
        eventName != softKeyboardChangeEventName)
    {
        LogDebug("Event type not supported");
        return JSValueMakeUndefined(context);
    }

    JSObjectRef objectCb = JSValueToObject(context, arguments[1], NULL);
    if (!JSObjectIsFunction(context, objectCb)) {
        LogError("JS object is not a function");
        return JSValueMakeUndefined(context);
    }
    JSValueSafeProtect(global_context, arguments[1]);
    //add object to Listeners
    //set event information according to each event type
    CallbackData data;
    data.object = objectCb;
    data.thisObject = thisObject;
    data.context = global_context;

    if (eventName == storageEventName) {
        data.eventType = StorageCustomEvent;
    } else if (eventName == appServiceEventName) {
        data.eventType = ServiceCustomEvent;
    } else if (eventName == softKeyboardChangeEventName) {
        data.eventType = SoftKeyboardChangeCustomEvent;
    }

    getIFrameListeners(thisObject)->push_back(data);

    return JSValueMakeUndefined(context);
}

void AddEventListenerSupport::RemoveIFrame(JSObjectPtr iframe)
{
    auto it = m_listeners.find(iframe->getObject());
    if (it == m_listeners.end()) {
        LogWarning("Iframe not registered");
        return;
    }

    FOREACH(listener, *it->second)
    {
        JSValueSafeUnprotect(listener->context, listener->object);
    }
    m_listeners.erase(it);
    LogDebug("Iframe removed from add event listener object");
}

void AddEventListenerSupport::CallStorageListenersFromDifferentIFrames(
        JSObjectRef iframe,
        const WrtDeviceApis::StorageEvent::Api::IStorageEventPtr& event)
{
    LogDebug("Invoked callbacks");

    FOREACH(it, m_listeners)
    {
        if (it->first == iframe) {
            continue;
        }

        FOREACH(listener, *it->second)
        {
            LogDebug("Call");

            auto eventPriv =
                new JSStorageEventPrivateObject(listener->context, event);

            JSObjectRef eventObject =
                JSObjectMake(listener->context, JSStorageEvent::getClassRef(), eventPriv);
            const size_t argc = 1;
            JSValueRef argv[argc] = { eventObject };

            JSValueSafeProtect(listener->context, eventObject);

            if (listener->eventType == StorageCustomEvent)
            {
                JSObjectCallAsFunction(
                    listener->context,
                    listener->object,
                    NULL,
                    argc,
                    argv,
                    NULL);
            }

            JSValueSafeUnprotect(listener->context, eventObject);
        }
    }

    LogDebug("Done");
}

void AddEventListenerSupport::CallCustomEventListenersFromIFrame(
        JSObjectRef iframe,
        CustomEventType eventType,
        JSObjectRef eventObject)
{
    LogDebug("Invoked callbacks");

    IFramesListeners::iterator it = m_listeners.find(iframe);
    if (it == m_listeners.end()) {
        LogDebug("This frame object not existed");
        return;
    }

    const size_t argc = 1;
    JSValueRef argv[argc] = { eventObject };

    FOREACH(listener, *it->second)
    {
        if (listener->eventType == eventType) {
            LogDebug("Call");
            JSObjectCallAsFunction(
                listener->context,
                listener->object,
                iframe,
                argc,
                argv,
                NULL);
        }
    }
    LogDebug("Done");
}

AddEventListenerSupport::ListenersPtr
AddEventListenerSupport::getIFrameListeners(JSObjectRef iframe)
{
    auto it = m_listeners.find(iframe);
    if (it != m_listeners.end()) {
        return it->second;
    }

    return m_listeners[iframe] = ListenersPtr(new Listeners());
}
}
}
