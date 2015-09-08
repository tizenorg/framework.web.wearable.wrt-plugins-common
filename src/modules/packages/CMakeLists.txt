# Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

SET(TARGET_MODULE_FILESYSTEM "wrt-plugins-filesystem")
SET(TARGET_MODULE_WIDGET "wrt-plugins-widget")
SET(TARGET_MODULE_LOCALSTORAGE "wrt-plugins-localstorage")
SET(TARGET_MODULE_WIDGET_INTERFACE "wrt-plugins-widget-interface")
SET(TARGET_MODULE_STORAGEEVENT "wrt-plugins-storageevent")
SET(TARGET_MODULE_TIZEN_SERVICE_EVENT "wrt-plugins-tizen-service-event")
SET(TARGET_MODULE_SOFTKEYBOARD_CHANGE_EVENT "wrt-plugins-softkeyboardchange-event")
SET(TARGET_MODULE_WIDGETDB "wrt-plugins-widgetdb")
SET(TARGET_MODULE_PLUGIN_MANAGER "wrt-plugins-plugin-manager")

ADD_SUBDIRECTORY(Filesystem)
ADD_SUBDIRECTORY(Widget)
ADD_SUBDIRECTORY(LocalStorage)
ADD_SUBDIRECTORY(WidgetInterface)
ADD_SUBDIRECTORY(WidgetDB)
ADD_SUBDIRECTORY(PluginManager)
ADD_SUBDIRECTORY(StorageEvent)
ADD_SUBDIRECTORY(TizenServiceEvent)
ADD_SUBDIRECTORY(SoftKeyboardChangeEvent)
