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
 * @file        plugin_logic.h
 * @author      Piotr Fatyga (p.fatyga@samsung.com)
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of plugin and feature
 *              loading routines
 * @brief       This code is intended to work behind view controller
 */
#ifndef PLUGIN_LOGIC_H
#define PLUGIN_LOGIC_H

#include <memory>
#include <vector>
#include <set>
#include <dpl/singleton.h>
#include <dpl/noncopyable.h>
#include <dpl/wrt-dao-ro/plugin_dao_read_only.h>
#include <js_overlay_types.h>
#include <js_types.h>
#include <Commons/Deprecated.h>

using namespace WrtPlugins::W3C;

class PluginLogic : DPL::Noncopyable
{
  public:
    // Widget session
    void initSession(int widgetHandle);
    void startSession(int widgetHandle,
                      JSGlobalContextRef view,
                      double scaleFactor,
                      const char* encodedBundle,
                      const char* theme);

    void stopSession(JSGlobalContextRef context);

    //TODO im not sure if it is needed. I think it should be hidden
    void performLibrariesUnload();
    bool loadPluginOnDemand(const WrtDB::DbPluginHandle &pluginHandle,
                            JavaScriptObject& parentObject,
                            JSGlobalContextRef context);

    WRT_PLUGINS_DEPRECATED void loadPluginsIntoIframes(
        JSGlobalContextRef context);

    void loadFrame(JSGlobalContextRef context);
    void unloadFrame(JSGlobalContextRef context);

    // reset custom window properties. (Tizen requirements)
    WRT_PLUGINS_DEPRECATED void setCustomProperties(double scaleFactor,
                                                    const char* encodedBundle,
                                                    const char* theme);

    void setCustomProperties(JSGlobalContextRef ctx,
                             double scaleFactor,
                             const char* encodedBundle,
                             const char* theme);

    WRT_PLUGINS_DEPRECATED void dispatchJavaScriptEvent(
        CustomEventType eventType);

    void dispatchJavaScriptEvent(JSGlobalContextRef ctx,
                                 CustomEventType eventType,
                                 void* data);

    unsigned int windowHandle() const;
    void setWindowHandle(unsigned int handle);

  private:
    PluginLogic();
    ~PluginLogic();

    class Impl;
    std::unique_ptr<Impl> m_impl;

    friend class DPL::Singleton<PluginLogic>;
};

typedef DPL::Singleton<PluginLogic> PluginLogicSingleton;

#endif // PLUGIN_LOGIC_H
