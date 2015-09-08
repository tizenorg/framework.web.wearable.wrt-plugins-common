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

MACRO(include_config_file INCLUDED_CONFIG_FILE_PATH)
  GET_FILENAME_COMPONENT(CURRENT_CONFIG_FILE_PATH ${CMAKE_CURRENT_LIST_FILE} PATH)
  INCLUDE(${CURRENT_CONFIG_FILE_PATH}/${INCLUDED_CONFIG_FILE_PATH}/${CONFIG_FILE_NAME} OPTIONAL)
ENDMACRO()

# Gets actual path for CMake configuration file (*.cmake).
# By default, if no argument is passed, current path will be set to variable
# named CURRENT_PATH.
# @param [Optional] Name of the variable to set with current path.
MACRO(get_current_path)
  SET(VARIABLE "CURRENT_PATH")
  IF(${ARGC} GREATER 0)
    SET(VARIABLE ${ARGV0})
  ENDIF()
  GET_FILENAME_COMPONENT(${VARIABLE} ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDMACRO()

SET(HOST_ARCH $ENV{DEB_HOST_ARCH})
IF("${HOST_ARCH}" MATCHES "armel")
    SET(HOST_ARCH "armel")
ENDIF()

#target names
SET(TARGET_PLUGIN_LOADING_LIB "wrt-plugin-loading")
SET(TARGET_COMMONS "wrt-plugins-commons")
SET(TARGET_COMMONS_JAVASCRIPT "wrt-plugins-commons-javascript")
SET(TARGET_JS_OVERLAY "wrt-plugins-js-overlay")
SET(TARGET_POPUP_ACE "wrt-popup-ace-runtime")
SET(TARGET_POPUP_ACE_RUNNER_LIB "wrt-popup-ace-runner")
SET(TARGET_POPUP_WRT "wrt-popup-wrt-runtime")
SET(TARGET_POPUP_WRT_RUNNER_LIB "wrt-popup-wrt-runner")
SET(TARGET_PLUGINS_API_SUPPORT "wrt-plugins-api-support")
SET(TARGET_PLUGINS_IPC_MESSAGE_LIB "wrt-plugins-ipc-message")

SET(PLUGIN_LOADING_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/plugin-loading)
SET(PLUGINS_API_SUPPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/plugins-api-support)
SET(PLUGINS_IPC_MESSAGE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/plugins-ipc-message)

ADD_SUBDIRECTORY(plugins-api-support)
ADD_SUBDIRECTORY(plugin-loading)
ADD_SUBDIRECTORY(js-overlay)
ADD_SUBDIRECTORY(Commons)
ADD_SUBDIRECTORY(CommonsJavaScript)
ADD_SUBDIRECTORY(dispatch-event)
ADD_SUBDIRECTORY(modules)
ADD_SUBDIRECTORY(standards)
ADD_SUBDIRECTORY(wrt-popup)
ADD_SUBDIRECTORY(plugins-installer)
ADD_SUBDIRECTORY(plugins-ipc-message)
