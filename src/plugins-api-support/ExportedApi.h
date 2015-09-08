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
 *
 * @file       ExportedApi.h
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */
#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_EXPORTED_API_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_EXPORTED_API_H_

#include <PluginRegistration.h>

/**
 * This file provides definition of entry point to the plugin's shared library
 * used by wrt.
 *
 * Each plugin have to provide 1 symbol which is get by dlsym.
 * The name of required symbol is specified by 'GetExportedSymbolName' function
 * The type of this symbol is pointer to ExportedApi struct
 *
 * To allow access to your plugin, you have to:
 *
 * 1)define 3 functions:
 *  - Register,
 *  - Unregister,
 *  - GetProvidedFeatures
 *  (names are not important)
 *
 * 2)define global struct named "dll_api" and initialize it with above functions
 *   *Example:
 *    ExportedApi dll_api = {Register, Unregister, GetProvidedFeatures};
 *
 *
 * Detailed Example how the file with api may looks like file:
 *
 * #include <Commons/Exception.h>
 * #include <Commons/WrtAccess/WrtAccess.h>
 *
 * #include <Plugin.h>
 * #include <ObjectFactory.h>
 * #include <PluginRegistration.h>
 * #include <ExportedApi.h>
 *
 * #include "JSTest.h"
 * #include "plugin_config.h"
 *
 * #define OBJECT_WIDGET "widget"
 * #define OBJECT_TEST "__test"
 *
 *  using namespace WrtPlugins::W3C;
 *  using namespace WrtDeviceApis;
 *  using namespace WrtDeviceApis::Commons;
 *  using namespace WrtPluginsApi;
 *
 *  namespace W3CTest
 *  {
 *
 *  void on_widget_start_callback(int widgetId)
 *  {
 *
 *  }
 *
 *  void on_widget_stop_callback(int widgetId)
 *  {
 *  }
 *
 *  }
 *
 *  void Register(PluginRegistration& r)
 *  {
 *      Plugin* plugin = new Plugin();
 *
 *      auto test = ObjectFactory::createMainObject(
 *                          OBJECT_TEST,
 *                          WrtPlugins::W3C::JSTest::getClassRef,
 *                          OBJECT_WIDGET);
 *
 *      plugin->AddObject(test);
 *
 *      r.Connect<OnWidgetStart>(W3CTest::on_widget_start_callback);
 *
 *      r.Connect<OnWidgetStop>(W3CTest::on_widget_stop_callback);
 *
 *      r.AddPlugin(*plugin);
 *  }
 *
 *  void Unregister(PluginRegistration& r, Plugin* plugin)
 *  {
 *      r.DisconnectAll();
 *      delete plugin;
 *  }
 *
 *  void GetProvidedFeatures(feature_mapping_interface_t *mapping)
 *  {
 *      WrtPlugins::W3C::WidgetTestDeclarations::getMappingInterface(mapping);
 *  }
 *
 *  ExportedApi dll_api={Register, Unregister, GetProvidedFeatures};
 *
 * #undef OBJECT_WIDGET
 * #undef OBJECT_TEST
 *
 *
 * */

//forward declaration
struct feature_mapping_interface_s;
typedef struct feature_mapping_interface_s feature_mapping_interface_t;

extern "C" struct ExportedApi
{
    /*
     * This function is invoked when library is loaded
     * */
    void (*Register)(WrtPluginsApi::PluginRegistration&);

    /*
     * This function is invoked when library is unloaded
     * */
    void (*Unregister)(WrtPluginsApi::PluginRegistration&,
                       WrtPluginsApi::Plugin* plugin);

    /*
     * This function is invoked by wrt-plugins-installer to obtain
     * info about features,functions,objects provided by plugin
     * */
    void (*GetProvidedFeatures)(feature_mapping_interface_t*);
};

constexpr const char* GetExportedSymbolName()
{
    return "dll_api";
}

#endif
