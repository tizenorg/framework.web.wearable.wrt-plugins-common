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
 * @file        plugin.cpp
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of plugin
 */
#include "plugin.h"
#include <dpl/log/secure_log.h>
#include <dpl/assert.h>
#include <dlfcn.h>

Plugin::Plugin(const std::string &fileName,
               void *libHandle,
               on_widget_start_proc *apiOnWidgetStart,
               on_widget_init_proc *apiOnWidgetInit,
               on_widget_stop_proc *apiOnWidgetStop,
               on_frame_load_proc* apiOnFrameLoad,
               on_frame_unload_proc* apiOnFrameUnload,
               const ClassPtrList &apiClassList) :
    m_fileName(fileName),
    m_libHandle(libHandle),
    m_apiOnWidgetStart(apiOnWidgetStart),
    m_apiOnWidgetInit(apiOnWidgetInit),
    m_apiOnWidgetStop(apiOnWidgetStop),
    m_apiOnFrameLoad(apiOnFrameLoad),
    m_apiOnFrameUnload(apiOnFrameUnload),
    m_apiClassList(apiClassList)
{}

Plugin::~Plugin()
{
    _D("Unloading plugin library: %s", m_fileName.c_str());

    // Unload library
    if (dlclose(m_libHandle) != 0) {
        _E("Cannot close plugin handle");
    } else {
        _D("Library is unloaded");
    }
}

PluginPtr Plugin::LoadFromFile(const std::string& fileName)
{
    static bool logEnable = (getenv("WRT_LOAD_PLUGINS_LOG_ENABLE") != NULL);

    _D("LoadFromFile %s", fileName.c_str());

    void *dllHandle;
    dllHandle = dlopen(fileName.c_str(), RTLD_LAZY);

    _D("dlopen() done!");

    if (dllHandle == NULL) {
        const char* error = (const char*)dlerror();
        _E("Failed to load plugin: %s. Reason: %s",
           fileName.c_str(),
           (error != NULL ? error : "unknown"));
        PluginPtr empty;
        return empty;
    }

    // Load new plugin API
    on_widget_start_proc *onWidgetStartProcPtr = NULL;
    on_widget_stop_proc *onWidgetStopProcPtr = NULL;
    on_widget_init_proc *onWidgetInitProcPtr = NULL;
    on_frame_load_proc *onFrameLoadProcPtr = NULL;
    on_frame_unload_proc *onFrameUnloadProcPtr = NULL;

    const js_entity_definition_t *rawClassList = NULL;
    get_widget_entity_map_proc *getWidgetEntityMapProcPtr = NULL;

    onWidgetStartProcPtr =
        reinterpret_cast<on_widget_start_proc *>(
            dlsym(dllHandle, PLUGIN_WIDGET_START_PROC_NAME));
    onWidgetInitProcPtr =
        reinterpret_cast<on_widget_init_proc *>(
            dlsym(dllHandle, PLUGIN_WIDGET_INIT_PROC_NAME));
    onWidgetStopProcPtr =
        reinterpret_cast<on_widget_stop_proc *>(
            dlsym(dllHandle, PLUGIN_WIDGET_STOP_PROC_NAME));
    onFrameLoadProcPtr =
        reinterpret_cast<on_frame_load_proc *>(
            dlsym(dllHandle, PLUGIN_FRAME_LOAD_PROC_NAME));
    onFrameUnloadProcPtr =
        reinterpret_cast<on_frame_unload_proc *>(
            dlsym(dllHandle, PLUGIN_FRAME_UNLOAD_PROC_NAME));
    getWidgetEntityMapProcPtr =
        reinterpret_cast<get_widget_entity_map_proc *>(
            dlsym(dllHandle, PLUGIN_GET_CLASS_MAP_PROC_NAME));

    if (getWidgetEntityMapProcPtr) {
        rawClassList = (*getWidgetEntityMapProcPtr)();

        if (logEnable) {
            _D("rawClassList : %p by getWidgetClassMapProcPtr()",
               rawClassList);
        }
    } else {
        rawClassList =
            static_cast<const js_entity_definition_t *>(dlsym(dllHandle,
                                                              PLUGIN_CLASS_MAP_NAME));
        if (logEnable) { _D("rawClassList : %p", rawClassList); }
    }

    if (NULL == onWidgetStartProcPtr || NULL == onWidgetStopProcPtr ||
        /*NULL == onWidgetInitProcPtr ||*/ NULL == rawClassList)
    {
        if (logEnable) {
            _W("#####");
            _W("##### Warning: The following plugin does not support new plugin API.");
            _W("##### Old plugin API is deprecated. Please update it to new API");
            _W("#####");
            _W("##### Plugin: %s has got deprecated or invalid API", fileName.c_str());
            _W("#####");
        }

        // Will not load plugin
        dlclose(dllHandle);

        PluginPtr empty;
        return empty;
    }

    if (logEnable) {
        _D("#####");
        _D("##### Plugin: %s supports new plugin API", fileName.c_str());
        _D("#####");
        _D("##### $onWidgetStartProc: %p", onWidgetStartProcPtr);
        _D("##### $onWidgetInitProc: %p", onWidgetInitProcPtr);
        _D("##### $onWidgetStopProc %p", onWidgetStopProcPtr);
        _D("##### $onFrameLoadProc %p", onFrameLoadProcPtr);
        _D("##### $onFrameUnloadProc %p", onFrameUnloadProcPtr);
        _D("##### $classMap: %p", reinterpret_cast<const void *>(rawClassList));
        _D("##### ");
        _D("##### Class map:");
    }

    const js_entity_definition_t *rawEntityListIterator = rawClassList;
    ClassPtrList classList(new Plugin::ClassList());

    // Parse all class definitions
    while (rawEntityListIterator->parent_name != NULL &&
           rawEntityListIterator->object_name != NULL)
    {
        if (logEnable)
        {
            // Logging
            _D("#####");
            _D("##### [%s]", rawEntityListIterator->object_name);
            _D("##### Interface: %s", rawEntityListIterator->interface_name);
            _D("##### Parent: %s", rawEntityListIterator->parent_name);
        }

        // Register class
        classList->push_back(ClassPtr(new Class(rawEntityListIterator)));

        // Go to next class
        ++rawEntityListIterator;
    }

    // Load export table
    _D("Plugin successfuly loaded");

    // Insert to loaded modules list

    PluginPtr instance(new Plugin(fileName,
                                  dllHandle,
                                  onWidgetStartProcPtr,
                                  onWidgetInitProcPtr,
                                  onWidgetStopProcPtr,
                                  onFrameLoadProcPtr,
                                  onFrameUnloadProcPtr,
                                  classList));

    return instance;
}

std::string Plugin::GetFileName() const
{
    return m_fileName;
}

void Plugin::OnWidgetStart(int widgetId)
{
    if (NULL != m_apiOnWidgetStart) {
        (*m_apiOnWidgetStart)(widgetId);
        _D("Called!");
    }
}

void Plugin::OnWidgetInit(feature_mapping_interface_t* mapping)
{
    AssertMsg(NULL != mapping, "NULL mapping interface provided");
    if (NULL != m_apiOnWidgetInit) {
        (*m_apiOnWidgetInit)(mapping);
        _D("Called!");
    }
}

void Plugin::OnWidgetStop(int widgetId)
{
    if (NULL != m_apiOnWidgetStop) {
        (*m_apiOnWidgetStop)(widgetId);
        _D("Called!");
    }
}

void Plugin::OnFrameLoad(java_script_context_t context)
{
    if (NULL != m_apiOnFrameLoad) {
        (*m_apiOnFrameLoad)(context);
        _D("Called!");
    }
}

void Plugin::OnFrameUnload(java_script_context_t context)
{
    if (NULL != m_apiOnFrameUnload) {
        (*m_apiOnFrameUnload)(context);
        _D("Called!");
    }
}

const Plugin::ClassPtrList Plugin::GetClassList() const
{
    return m_apiClassList;
}
