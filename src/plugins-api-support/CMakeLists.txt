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

# @file     CMakeLists.txt
# @author   Grzegorz Krawczyk (g.krawczyk@samsung.com)
# @version  1.0
#

PKG_SEARCH_MODULE(dpl REQUIRED dpl-efl)

SET(PLUGINS_API_SUPPORT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ObjectFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginRegistration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginRegistry.cpp
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${dpl_INCLUDE_DIRS}
)

ADD_LIBRARY(${TARGET_PLUGINS_API_SUPPORT} SHARED
    ${PLUGINS_API_SUPPORT_SOURCES}
)

SET_TARGET_PROPERTIES(${TARGET_PLUGINS_API_SUPPORT} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")

SET_TARGET_PROPERTIES(${TARGET_PLUGIN_API_SUPPORT} PROPERTIES
    COMPILE_FLAGS -fPIC
    LINK_FLAGS "-Wl,--as-needed -Wl,--hash-style=both"
)

SET_TARGET_PROPERTIES(${TARGET_PLUGINS_API_SUPPORT} PROPERTIES
    SOVERSION ${CMAKE_PROJECT_API_VERSION}
    VERSION ${CMAKE_PROJECT_VERSION}
)

TARGET_LINK_LIBRARIES(${TARGET_PLUGINS_API_SUPPORT}
    ${dpl_LIBRARIES}
)

INSTALL(TARGETS ${TARGET_PLUGINS_API_SUPPORT}
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

INSTALL(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ExportedApi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ObjectFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CallbackSupport.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tuple.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginSignals.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SignalSignature.h
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginRegistration.h
    DESTINATION include/wrt-plugins-api-support)
