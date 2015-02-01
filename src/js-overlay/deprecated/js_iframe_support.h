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
 * @file       IFrameSupport.h
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef _WRT_PLUGINS_SRC_W3C_WIDGET_IFRAME_SUPPORT_H_
#define _WRT_PLUGINS_SRC_W3C_WIDGET_IFRAME_SUPPORT_H_

#include <map>
#include <memory>
#include <JavaScriptCore/JavaScript.h>
#include <wrt_plugin_export.h>

namespace WrtPlugins {
namespace W3C {
struct IFrameData
{
    IFrameData() : widget(0), addEventListener(0){}

    JSObjectRef widget;
    JSObjectRef addEventListener;
};
typedef std::shared_ptr<IFrameData> IFrameDataPtr;

class IFrameSupport
{
  public:
    static void RegisterWidget(java_script_context_t global_context,
                               js_object_instance_t iframe,
                               js_object_instance_t object);

    static void RegisterAddEventListener(java_script_context_t global_context,
                                         js_object_instance_t iframe,
                                         js_object_instance_t object);

    static JSObjectRef getIFrameObjectForWidget(JSObjectRef widgetObject);

  private:
    //map iframe:objects
    typedef std::map<JSObjectRef, IFrameDataPtr> IFramesContainer;

    static IFramesContainer m_iframesObject;

  private:
    static IFrameDataPtr getIFrameData(JSObjectRef iframe);
};
}
}

#endif
