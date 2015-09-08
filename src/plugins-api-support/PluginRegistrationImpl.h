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
 * @file   PluginRegistrationImpl.h
 * @author Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_PLUGIN_REGISTRATION_IMPL_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_PLUGIN_REGISTRATION_IMPL_H_

#include <string>
#include "SignalsSupport.h"
#include "Plugin.h"
#include <dpl/log/log.h>

namespace WrtPluginsApi {
class PluginRegistration::Impl
{
  public:
    Impl(SignalsSupport& registry, const std::string& libraryName) :
        m_registry(registry),
        m_libraryName(libraryName)
    {}

    void AddPlugin(Plugin& plugin)
    {
        m_registry.AddPlugin(m_libraryName, plugin);
    }

    template<typename T>
    void Connect(const typename T::Type& slot)
    {
        m_registry.Connect<T>(m_libraryName, slot);
    }

    void DisconnectAll()
    {
        m_registry.Disconnect(m_libraryName);
    }

  private:
    SignalsSupport& m_registry;
    std::string m_libraryName;
};
}

#endif
