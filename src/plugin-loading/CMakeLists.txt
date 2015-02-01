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
# @author   Lukasz Marek (l.marek@samsung.com)
# @version  1.0
#

pkg_search_module(plugin-types REQUIRED wrt-plugins-types)
pkg_search_module(dpl REQUIRED dpl-efl)
pkg_search_module(dpl-event REQUIRED dpl-event-efl)
pkg_search_module(dpl-wrt-dao-ro REQUIRED dpl-wrt-dao-ro)
pkg_search_module(webkit2 REQUIRED ewebkit2)

set(PLUGIN_LOADING_SOURCES
    ${PLUGIN_LOADING_DIRS}/plugin_logic.cpp
    ${PLUGIN_LOADING_DIRS}/plugin.cpp
    ${PLUGIN_LOADING_DIRS}/plugin_model.cpp
    ${PLUGIN_LOADING_DIRS}/explorer.cpp
    ${PLUGIN_LOADING_DIRS}/plugin_iframe_support.cpp
    ${PLUGIN_LOADING_DIRS}/plugin_container_support.cpp
    ${PLUGIN_LOADING_DIRS}/plugin_property_support.cpp
    ${PLUGIN_LOADING_DIRS}/javascript_interface.cpp
    ${PLUGIN_LOADING_DIRS}/js_page_session.cpp
)

INCLUDE_DIRECTORIES(
    ${PLUGIN_MODULE_SRC_DIR}
    ${PLUGIN_LOADING_DIRS}
    ${webkit2_INCLUDE_DIRS}
    ${plugin-types_INCLUDE_DIRS}
)

ADD_LIBRARY(${TARGET_PLUGIN_LOADING_LIB} SHARED
    ${PLUGIN_LOADING_SOURCES}
)

SET_TARGET_PROPERTIES(${TARGET_PLUGIN_LOADING_LIB} PROPERTIES
    COMPILE_FLAGS -fPIC
    LINK_FLAGS "-Wl,--as-needed -Wl,--hash-style=both"
)

SET_TARGET_PROPERTIES(${TARGET_PLUGIN_LOADING_LIB} PROPERTIES
    SOVERSION ${CMAKE_PROJECT_API_VERSION}
    VERSION ${CMAKE_PROJECT_VERSION}
)

target_link_libraries(${TARGET_PLUGIN_LOADING_LIB}
    ${dpl_LIBRARIES}
    ${dpl-event_LIBRARIES}
    ${dpl-wrt-dao-ro_LIBRARIES}
    ${TARGET_COMMONS}
    ${TARGET_JS_OVERLAY}
    wrt-plugins-tizen-service-event # static lib
    wrt-plugins-softkeyboardchange-event       # static lib
)

INSTALL(TARGETS ${TARGET_PLUGIN_LOADING_LIB}
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# For wrt-plugins
INSTALL(FILES
    ${PLUGIN_LOADING_DIRS}/plugin_logic.h
    ${PLUGIN_LOADING_DIRS}/js_types.h
    DESTINATION include/wrt-plugin-loading)
