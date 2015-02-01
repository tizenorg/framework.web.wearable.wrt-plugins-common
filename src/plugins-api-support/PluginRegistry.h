/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file    PluginRegistry.h
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */
#ifndef _WRT_PLUGINS_COMMON_PLUGIN_LOADING_PLUGIN_REGISTRY_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_LOADING_PLUGIN_REGISTRY_H_

#include <map>
#include <string>
#include "SignalsSupport.h"
#include "Plugin.h"
#include <dpl/exception.h>

namespace WrtPluginsApi {
typedef std::list<Plugin*> PluginsList;
typedef std::shared_ptr<PluginsList> PluginsListPtr;
typedef std::map< std::string, PluginsListPtr> PluginsSet;

class PluginRegistry : public SignalsSupport
{
  public:
    DECLARE_EXCEPTION_TYPE(DPL::Exception, Base)
    DECLARE_EXCEPTION_TYPE(Base, PluginNotFound)

    template <typename T, typename ... Args>
    void Call(Args ... args)
    {
        Invoke<T>(args ...);
    }

    template <typename T, typename ... Args>
    void CallGroup(const typename CallbackSupport<T>::GroupType& type,
                   Args ... args)
    {
        InvokeGroup<T>(type, args ...);
    }

    void AddPlugin(const std::string& libraryName, Plugin& plugin);

    /*
     * @throw PluginNotFound
     * */
    Plugin* GetPlugin(const std::string& libraryName);

    void RemovePlugin(const std::string& libraryName, Plugin& plugin);

    void UnloadAll();

    ~PluginRegistry();

  private:
    bool LoadFromFile(const std::string& libraryName);

    typedef void* Symbol;

    std::map<std::string, Plugin*> m_plugins;
    std::map<std::string, void*> m_libraries;
};

typedef std::shared_ptr<PluginRegistry> PluginRegistryPtr;
}

#endif
