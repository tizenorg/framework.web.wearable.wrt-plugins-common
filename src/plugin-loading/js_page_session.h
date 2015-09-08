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
 * @file        js_page_session.h
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version     1.0
 */
#ifndef WRT_PLUGIN_JS_PAGE_SESSION_H_
#define WRT_PLUGIN_JS_PAGE_SESSION_H_

#include <memory>
#include <vector>
#include <dpl/wrt-dao-ro/plugin_dao_read_only.h>
#include <js_overlay_types.h>
#include "js_types.h"

using namespace WrtPlugins::W3C;

class JSPageSession
{
  public:
    // Widget session
    void startSession(int widgetHandle,
                      JSGlobalContextRef view,
                      double scaleFactor,
                      const char* encodedBundle,
                      const char* theme);

    void stopSession();

    //TODO im not sure if it is needed. I think it should be hidden
    void performLibrariesUnload();
    bool loadPluginOnDemand(const WrtDB::DbPluginHandle &pluginHandle,
                            JavaScriptObject& parentObject,
                            JSGlobalContextRef context);

    void loadFrame(JSGlobalContextRef context);
    void unloadFrame(JSGlobalContextRef context);

    // reset custom window properties. (Tizen requirements)
    void setCustomProperties(double scaleFactor,
                             const char* encodedBundle,
                             const char* theme);

    void dispatchJavaScriptEvent(CustomEventType eventType, void* data);

    JSPageSession(const PluginContainerSupportPtr& containerSupport);

    virtual ~JSPageSession();

  private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

typedef std::shared_ptr<JSPageSession> JSPageSessionPtr;

#endif // PLUGIN_LOGIC_H
