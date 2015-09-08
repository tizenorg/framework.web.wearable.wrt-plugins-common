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
#include "PluginRegistry.h"
#include "PluginRegistration.h"
#include "PluginRegistrationImpl.h"
#include "ExportedApi.h"

#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>

#include <dpl/log/log.h>
#include <dpl/log/secure_log.h>
#include <dpl/foreach.h>
#include <dpl/scope_guard.h>

namespace WrtPluginsApi {
void PluginRegistry::AddPlugin(const std::string& libraryName,
                               Plugin& plugin)
{
    LogDebug("Adding plugin for library: " << libraryName);

    auto libraryIt = m_plugins.find(libraryName);
    if (m_plugins.end() == libraryIt) {
        m_plugins[libraryName] = &plugin;
    }
}

Plugin* PluginRegistry::GetPlugin(const std::string& libraryName)
{
    auto it = m_plugins.find(libraryName);
    if (it == m_plugins.end()) {
        if (!LoadFromFile(libraryName)) {
            LogError("Failed to load lib" << libraryName);
            ThrowMsg(PluginNotFound, "Failed to load plugin");
        }

        return m_plugins[libraryName];
    }

    return it->second;
}

void PluginRegistry::RemovePlugin(const std::string& libraryName,
                                  Plugin& plugin)
{
    auto it = m_plugins.find(libraryName);
    if (it != m_plugins.end()) {
        if (&plugin == it->second) {
            m_plugins.erase(it);
        }
    }
}

void PluginRegistry::UnloadAll()
{
    LogDebug("Unload all plugins");

    typedef void (*UnregisterFunction)(PluginRegistration&, Plugin&);

    FOREACH(libraryIt, m_libraries)
    {
        auto pluginIt = m_plugins.find(libraryIt->first);
        if (m_plugins.end() != pluginIt) {
            void* handle = dlopen(libraryIt->first.c_str(), RTLD_NOW);
            if (!handle) {
                const char* error = (const char*)dlerror();
                LogError("Error: " << (error != NULL ? error : "unknown"));
                continue;
            }
            DPL_SCOPE_EXIT(handle) {
                if (dlclose(handle) != 0) {
                    const char* error = dlerror();
                    if (error != NULL)
                    {
                        std::string errstr{error};
                        _E("%s", errstr.c_str());
                    }
                    else
                    {
                        _E("unknown error while closing plug-in library");
                    }
                }
            };

            ExportedApi* entryPoint =
                static_cast<ExportedApi*>
                (dlsym(handle, GetExportedSymbolName()));
            if (NULL == entryPoint) {
                const char* error = (const char*)dlerror();
                LogError("Error: " << (error != NULL ? error : "unknown"));
                continue;
            }
            if (entryPoint->Unregister == NULL) {
                LogError("Error Unregister function not set");
                continue;
            }

            PluginRegistration registration(
                new PluginRegistration::Impl(*this, libraryIt->first));

            entryPoint->Unregister(registration, (pluginIt->second));

            m_plugins.erase(pluginIt);
        }
        dlclose(libraryIt->second);
    }
}

bool PluginRegistry::LoadFromFile(const std::string& libraryName)
{
    void* handle = dlopen(libraryName.c_str(), RTLD_NOW);
    if (!handle) {
        const char* error = (const char*)dlerror();
        LogError("Error: " << (error != NULL ? error : "unknown"));
        return false;
    }
    m_libraries[libraryName] = handle;

    ExportedApi* entryPoint =
        static_cast<ExportedApi*>(dlsym(handle, GetExportedSymbolName()));
    if (NULL == entryPoint) {
        const char* error = (const char*)dlerror();
        LogError("Error: " << (error != NULL ? error : "unknown"));
        return false;
    }

    if (entryPoint->Register == NULL) {
        LogError("Error Register function not set");
        return false;
    }
    if (entryPoint->Unregister == NULL) {
        LogError("Error Unregister function not set");
        return false;
    }

    PluginRegistration registration(
        new PluginRegistration::Impl(*this, libraryName));
    entryPoint->Register(registration);

    return true;
}

PluginRegistry::~PluginRegistry()
{
    //TODO discuss ... when the unload should be called
    //    UnloadAll();
}
}
