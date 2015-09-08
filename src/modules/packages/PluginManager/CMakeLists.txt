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
#
# @file     CMakeLists.txt
# @author   Lukasz Marek (l.marek@samsung.com)
# @version  1.0
#

MACRO(install_header_file HEADER_FILE)
    INSTALL(FILES ${API_PLUGIN_MANAGER_PATH}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADER_PREFIX}/PluginManager)
ENDMACRO()

SET(TARGET_NAME ${TARGET_MODULE_PLUGIN_MANAGER})

PKG_SEARCH_MODULE(webkit2 REQUIRED ewebkit2)

INCLUDE_DIRECTORIES(
    ${INCLUDES_PLATFORM_IMPLEMENTATION_PLUGIN_MANAGER}
    ${webkit2_INCLUDE_DIRS}}
)

SET(SRCS
  ${SRCS_API_PLUGIN_MANAGER}
  ${SRCS_PLATFORM_IMPLEMENTATION_PLUGIN_MANAGER}
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET_NAME}
   ${LIBS_COMMONS}
   ${TARGET_PLUGIN_LOADING_LIB}
   ${TARGET_COMMONS}
   ${TARGET_COMMONS_JAVASCRIPT}
   ${LIBS_PLATFORM_IMPLEMENTATION_PLUGIN_MANAGER}
)

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
 SOVERSION ${CMAKE_PROJECT_API_VERSION}
 VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION
    ${DESTINATION_LIB_PREFIX})

INSTALL_HEADER_FILE(PluginManagerFactory.h)
INSTALL_HEADER_FILE(IPluginManager.h)
