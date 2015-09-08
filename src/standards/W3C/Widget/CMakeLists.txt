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

ADD_DEFINITIONS("-DWRT_PLUGINS_WIDGET_LOG")

SET(TARGET_NAME "wrt-plugins-w3c-widget-interface")

PKG_SEARCH_MODULE(webkit2 REQUIRED ewebkit2)
PKG_SEARCH_MODULE(plugin-types REQUIRED wrt-plugins-types)

SET(SRCS
  ${SRCS_COMMONS}
  JSWidget.cpp
  JSPreferences.cpp
  plugin_initializer.cpp
  plugin_config.cpp
)

INCLUDE_DIRECTORIES(
  ${API_WIDGET_PATH}
  ${webkit2_INCLUDE_DIRS}
  ${plugin-types_INCLUDE_DIRS}
  ${PLUGINS_API_SUPPORT_DIRS}
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET_NAME}
  ${LIBS_COMMON}
  ${TARGET_COMMONS}
  ${TARGET_COMMONS_JAVASCRIPT}
  ${TARGET_JS_OVERLAY}
  ${TARGET_PLUGINS_API_SUPPORT}
  wrt-plugins-widget
  wrt-plugins-localstorage
  wrt-plugins-storageevent
  wrt-dispatch-event
)

# Override default tag
SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="WRT_PLUGINS/WIDGET")

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION /usr/lib/wrt-plugins/w3c-widget-interface)
