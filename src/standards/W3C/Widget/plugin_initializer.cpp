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
 * @file       plugin_initializer.cpp
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */

#include <dpl/log/secure_log.h>

#include <Commons/plugin_initializer_def.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <ExportedApi.h>
#include <ObjectFactory.h>
#include <Plugin.h>
#include <PluginRegistration.h>

#include "JSWidget.h"
#include "JSPreferences.h"

#include "plugin_config.h"

#define OBJECT_WIDGET "widget"
#define INTERFACE_WIDGET_NAME "Widget"

using namespace WrtPlugins::W3C;
using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtPluginsApi;

namespace Options {
class_definition_options_t WidgetOptions = {
    JS_CLASS,
    CREATE_INSTANCE,
    NONE_NOTICE,
    IGNORED,
    NULL,
    NULL,
    NULL
};

class_definition_options_t WidgetInterfaceOptions = {
    JS_INTERFACE,
    CREATE_INSTANCE,
    NONE_NOTICE,
    IGNORED, //ignored
    NULL,
    NULL,
    NULL
};
}

//TODO: remove this after switch to wrt-plugin-installer
void on_widget_init_callback(feature_mapping_interface_t *mapping)
{
    _D("[W3C\\widget] on_widget_init_callback");

    WrtPlugins::W3C::WidgetDeclarations::getMappingInterface(mapping);
}
//...

void on_widget_start_callback(int widgetId)
{
    _D("[W3C\\widget] on_widget_start_callback (%d)", widgetId);

    Try
    {
        WrtAccessSingleton::Instance().initialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        _E("Wrt wrapper registration failed");
        return;
    }
}

void on_widget_stop_callback(int widgetId)
{
    _D("[W3C\\widget] on_widget_stop_callback (%d)", widgetId);
    Try
    {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        _E("Wrt wrapper registration failed");
        return;
    }
}

void Register(PluginRegistration& r)
{
    Plugin* plugin = new Plugin();

    auto widgetObject = ObjectFactory::createObjectWithInterface(
            OBJECT_WIDGET,
            (js_class_template_getter) WrtPlugins::W3C::JSWidget::getClassRef,
            INTERFACE_WIDGET_NAME,
            reinterpret_cast<js_class_constructor_cb_t>(WrtPlugins::W3C::JSWidget::callAsConstructor));

    plugin->AddObject(widgetObject);

    r.Connect<OnWidgetStart>(on_widget_start_callback);
    r.Connect<OnWidgetStop>(on_widget_stop_callback);

    r.AddPlugin(*plugin);
}

void Unregister(PluginRegistration& r, Plugin* plugin)
{
    r.DisconnectAll();
    delete plugin;
}

void GetProvidedFeatures(feature_mapping_interface_t *mapping)
{
    WrtPlugins::W3C::WidgetDeclarations::getMappingInterface(mapping);
}

ExportedApi dll_api={Register, Unregister, GetProvidedFeatures};

//TODO: remove this after switch to wrt-plugin-installer
PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
PLUGIN_ON_WIDGET_INIT(on_widget_init_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_INTERFACE(
    WRT_JS_EXTENSION_OBJECT_GLOBAL,
    INTERFACE_WIDGET_NAME,
    (js_class_template_getter) WrtPlugins::W3C::JSWidget::getClassRef,
    reinterpret_cast<js_class_constructor_cb_t>(WrtPlugins::W3C::JSWidget::
                                                    callAsConstructor),
    &Options::WidgetInterfaceOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE_PRODUCT(
    WRT_JS_EXTENSION_OBJECT_GLOBAL,
    OBJECT_WIDGET,
    INTERFACE_WIDGET_NAME,
    &Options::WidgetOptions)

PLUGIN_CLASS_MAP_END
//...

#undef OBJECT_WIDGET
#undef INTERFACE_WIDGET_NAME
