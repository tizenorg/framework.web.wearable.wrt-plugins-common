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
# Includes CMake configuration file (*.cmake), preserving appropriate paths.

ADD_DEFINITIONS("-DWRT_PLUGINS_COMMON_LOG")

macro(include_config_file INCLUDED_CONFIG_FILE_PATH)
  get_filename_component(CURRENT_CONFIG_FILE_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)
  include(${CURRENT_CONFIG_FILE_PATH}/${INCLUDED_CONFIG_FILE_PATH}/${CONFIG_FILE_NAME} OPTIONAL)
endmacro()

# Gets actual path for CMake configuration file (*.cmake).
# By default, if no argument is passed, current path will be set to variable
# named CURRENT_PATH.
# @param [Optional] Name of the variable to set with current path.
macro(get_current_path)
  set(VARIABLE "CURRENT_PATH")
  if (${ARGC} GREATER 0)
    set(VARIABLE ${ARGV0})
  endif ()
  get_filename_component(${VARIABLE} ${CMAKE_CURRENT_LIST_FILE} PATH)
endmacro()

set(HOST_ARCH $ENV{DEB_HOST_ARCH})
if("${HOST_ARCH}" MATCHES "armel")
    set(HOST_ARCH "armel")
endif()

#target names
set(TARGET_PLUGIN_LOADING_LIB "wrt-plugin-loading")
set(TARGET_COMMONS "wrt-plugins-commons")
set(TARGET_COMMONS_JAVASCRIPT "wrt-plugins-commons-javascript")
set(TARGET_JS_OVERLAY "wrt-plugins-js-overlay")
SET(TARGET_POPUP_ACE "wrt-popup-ace-runtime")
SET(TARGET_POPUP_ACE_RUNNER_LIB "wrt-popup-ace-runner")
SET(TARGET_POPUP_WRT "wrt-popup-wrt-runtime")
SET(TARGET_POPUP_WRT_RUNNER_LIB "wrt-popup-wrt-runner")
set(TARGET_PLUGINS_API_SUPPORT "wrt-plugins-api-support")
set(TARGET_PLUGINS_IPC_MESSAGE_LIB "wrt-plugins-ipc-message")

set(PLUGIN_LOADING_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/plugin-loading)
set(PLUGINS_API_SUPPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/plugins-api-support)
set(PLUGINS_IPC_MESSAGE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/plugins-ipc-message)

add_subdirectory(plugins-api-support)
add_subdirectory(plugin-loading)
add_subdirectory(js-overlay)
add_subdirectory(Commons)
add_subdirectory(CommonsJavaScript)
add_subdirectory(dispatch-event)
add_subdirectory(modules)
add_subdirectory(standards)
add_subdirectory(wrt-popup)
add_subdirectory(plugins-installer)
add_subdirectory(plugins-ipc-message)
