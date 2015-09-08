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
 * @file       IFrameSupport.cpp
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */

#include <dpl/foreach.h>
#include <dpl/assert.h>
#include <dpl/log/log.h>
#include <js_iframe_support.h>

namespace WrtPlugins {
namespace W3C {
IFrameSupport::IFramesContainer IFrameSupport::m_iframesObject =
    IFrameSupport::IFramesContainer();

void IFrameSupport::RegisterWidget(java_script_context_t /*global_context*/,
                                   js_object_instance_t iframe,
                                   js_object_instance_t object)
{
    LogDebug("New widget instance registered");
    LogDebug("iframe: " << iframe);
    LogDebug("object: " << object);
    getIFrameData(static_cast<JSObjectRef>(iframe))->widget =
        static_cast<JSObjectRef>(object);
}

void IFrameSupport::RegisterAddEventListener(
    java_script_context_t /*global_context*/,
    js_object_instance_t iframe,
    js_object_instance_t object)
{
    LogDebug("New addEventListener instance registered");
    LogDebug("iframe: " << iframe);
    LogDebug("object: " << object);
    getIFrameData(static_cast<JSObjectRef>(iframe))->addEventListener
        = static_cast<JSObjectRef>(object);
}

JSObjectRef IFrameSupport::getIFrameObjectForWidget(JSObjectRef widgetObject)
{
    FOREACH(it, m_iframesObject)
    {
        if (it->second->widget == widgetObject) {
            LogDebug("iframe found");
            return it->first;
        }
    }
    LogDebug("Iframe not found");
    return NULL;
}

IFrameDataPtr IFrameSupport::getIFrameData(JSObjectRef iframe)
{
    auto it = m_iframesObject.find(iframe);
    if (it != m_iframesObject.end()) {
        return it->second;
    }

    return m_iframesObject[iframe] = IFrameDataPtr(new IFrameData());
}
}
}
