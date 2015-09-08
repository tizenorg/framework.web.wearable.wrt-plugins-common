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
/*
 * @author      Karol Majewski (k.majewski@samsung.com)
 * @version     0.1
 * @brief       This is a file that you provides interface for wrt-engine while
 * loading and using plugin
 *              If you are a plugin developer you need to plugin_initializer.cpp
 * in your module and provide implementation for macros below
 */

#ifndef WRTDEVICEAPIS_COMMONS_PLUGIN_INITIALIZER_DEF_H_
#define WRTDEVICEAPIS_COMMONS_PLUGIN_INITIALIZER_DEF_H_

#include <wrt_plugin_export.h>

typedef java_script_context_t JavaScriptContext;

#define EXPORT_SYMBOL __attribute__((__visibility__("default")))

//DEFINES FOR GLOBAL OBJECTS AVAILABLE IN JAVASCRIPT
/**
 * each object which declare this object as parent
 * will ba available as global object in javascript
 */
#define WRT_JS_EXTENSION_OBJECT_GLOBAL  "GLOBAL_OBJECT"

/**
 * global object bondi
 * */
#define WRT_JS_EXTENSION_OBJECT_BONDI "bondi"

/**
 * global object Widget
 * */
#define WRT_JS_EXTENSION_OBJECT_WIDGET "Widget"

/**
 * global object deviceapis
 * */
#define WRT_JS_EXTENSION_OBJECT_DEVICEAPIS "deviceapis"

/**
 * global object tizen
 * */
#define WRT_JS_EXTENSION_OBJECT_TIZEN "tizen"

//HAVE TO BE IMPLEMENTED IN EVERY PLUGIN
/*
 * You have to(!) call this macro in your plugin_initializer.cpp(!) file
 * providing callback that will be called while loading each widget (for every
 * loaded widget this function will be called)
 *  Example:
 *       plugin_initializer.cpp
 *        void on_widget_start_callback(int widgetId, JSContextRef context,
 * const engine_interface_t *interface)
 *        {
 *          //...
 *        }
 *        PLUGIN_ON_WIDGET_START(on_widget_start_callback)
 */
#define PLUGIN_ON_WIDGET_START(CALLBACK_NAME) extern "C" const \
    on_widget_start_proc PLUGIN_WIDGET_START_PROC EXPORT_SYMBOL = CALLBACK_NAME;

/*
 *  You have to(!) call this macro in your plugin_initializer.cpp(!) file
 *  providing callback that will be called while loading each widget
 * (for every loaded widget this function will be called)
 *  Example:
 *       plugin_initializer.cpp
 *        void on_widget_init_callback(feature_mapping_interface_t *mapping)
 *        {
 *          //...
 *        }
 *        PLUGIN_ON_WIDGET_INIT(on_widget_init_callback)
 */
#define PLUGIN_ON_WIDGET_INIT(CALLBACK_NAME) extern "C" \
    const on_widget_init_proc PLUGIN_WIDGET_INIT_PROC EXPORT_SYMBOL = \
        CALLBACK_NAME;

/*
 *  You have to(!) call this macro in your plugin_initializer.cpp(!) file
 * providing callback that will be called while unloading each widget (for
 * every unloaded widget this function will be called)
 *  Example:
 *        void on_widget_stop_callback(int widgetId)
 *        {
 *          //...
 *        }
 *        PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
 */
#define PLUGIN_ON_WIDGET_STOP(CALLBACK_NAME) extern "C" const \
    on_widget_stop_proc PLUGIN_WIDGET_STOP_PROC EXPORT_SYMBOL = CALLBACK_NAME;

/*
 *  You have to(!) call this macro in your plugin_initializer.cpp(!) file
 * providing callback that will be called while unloading each page (for every
 * loaded page, including nested page, this function will be called)
 *  Example:
 *        void on_frame_load_callback(java_script_context_t context)
 *        {
 *          //...
 *        }
 *        PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
 */
#define PLUGIN_ON_FRAME_LOAD(CALLBACK_NAME) extern "C" const on_frame_load_proc \
    PLUGIN_FRAME_LOAD_PROC EXPORT_SYMBOL = CALLBACK_NAME;

/*
 *  You have to(!) call this macro in your plugin_initializer.cpp(!) file
 * providing callback that will be called while ununloading each page (for
 * every unloaded page, including nested page, this function will be called)
 *  Example:
 *        void on_frame_unload_callback(java_script_context_t context)
 *        {
 *          //...
 *        }
 *        PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)
 */
#define PLUGIN_ON_FRAME_UNLOAD(CALLBACK_NAME) extern "C" const \
    on_frame_unload_proc PLUGIN_FRAME_UNLOAD_PROC EXPORT_SYMBOL = CALLBACK_NAME;

/*
 * You have to(!) define an array of structures in your
 * plugin_initializer.cpp(!) file describing a JS class (class_definition) and
 * it's parent class name (parent_name).
 * JS class will be bind to a parent class name (parent_name.jsclass_name).
 * Example:
 *    plugin_initializer.cpp
 *      PLUGIN_CLASS_MAP_BEGIN
 *          PLUGIN_CLASS_MAP_ADD_CLASS(
 *              "bondi",
 *              WrtPluginBondi::JSICameraManager::getClassInfo())
 *          PLUGIN_CLASS_MAP_ADD_CLASS(
 *              "bondi",
 *              WrtPluginBondi::JSICameraAnotherClass::getClassInfo())
 *      PLUGIN_CLASS_MAP_END
 *
 */
#define PLUGIN_CLASS_MAP_BEGIN extern "C" const js_entity_definition_t \
    PLUGIN_CLASS_MAP[] EXPORT_SYMBOL = {

#define PLUGIN_CLASS_MAP_ADD_INTERFACE(PARENTNAME, \
                                       INTERFACENAME, \
                                       JSPRODUCTCLASSTEMPLATE, \
                                       PRODUCTCONSTRUCTORCB, \
                                       PRIVDATA) \
    { PARENTNAME, INTERFACENAME, "", JSPRODUCTCLASSTEMPLATE, \
      PRODUCTCONSTRUCTORCB, PRIVDATA },

#define PLUGIN_CLASS_MAP_ADD_INTERFACE_PRODUCT(PARENTNAME, OBJECTNAME, \
                                               INTERFACENAME, PRIVDATA) \
    { PARENTNAME, OBJECTNAME, INTERFACENAME, NULL, NULL, PRIVDATA },

#define PLUGIN_CLASS_MAP_ADD_CLASS(PARENTNAME, CLASSNAME, JSCLASSTEMPLATE, \
                                   PRIVDATA) \
    { PARENTNAME, CLASSNAME, "", JSCLASSTEMPLATE, NULL, PRIVDATA },

#define PLUGIN_CLASS_MAP_END { NULL, NULL, NULL, NULL, NULL, NULL } };

#define PLUGIN_CLASS_MAP_BEGIN_STATIC static const js_entity_definition_t \
    PLUGIN_CLASS_MAP[] = {

#define PLUGIN_GET_CLASS_MAP(CALLBACK_NAME) extern "C" const \
    get_widget_entity_map_proc \
    PLUGIN_GET_CLASS_PROC_MAP EXPORT_SYMBOL = CALLBACK_NAME;

#endif // WRTDEVICEAPIS_COMMONS_PLUGIN_INITIALIZER_DEF_H_
