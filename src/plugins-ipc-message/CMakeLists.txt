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
# @author   Jihoon Chung (jihoon.chung@samsung.com)
# @version  1.0
#

pkg_search_module(dpl REQUIRED dpl-efl)
pkg_search_module(webkit2 REQUIRED ewebkit2)

set(PLUGINS_IPC_MESSAGE_SOURCES
    ${PLUGINS_IPC_MESSAGE_DIRS}/ipc_message_support.cpp
)

INCLUDE_DIRECTORIES(
    ${PLUGINS_IPC_MESSAGE_DIRS}
    ${webkit2_INCLUDE_DIRS}
)

ADD_LIBRARY(${TARGET_PLUGINS_IPC_MESSAGE_LIB} SHARED
    ${PLUGINS_IPC_MESSAGE_SOURCES}
)

SET_TARGET_PROPERTIES(${TARGET_PLUGINS_IPC_MESSAGE_LIB} PROPERTIES
    COMPILE_FLAGS -fPIC
    LINK_FLAGS "-Wl,--as-needed -Wl,--hash-style=both"
)

SET_TARGET_PROPERTIES(${TARGET_PLUGINS_IPC_MESSAGE_LIB} PROPERTIES
    SOVERSION ${CMAKE_PROJECT_API_VERSION}
    VERSION ${CMAKE_PROJECT_VERSION}
)

target_link_libraries(${TARGET_PLUGINS_IPC_MESSAGE_LIB}
    ${dpl_LIBRARIES}
    ${webkit2_LIBRARIES}
)

INSTALL(TARGETS ${TARGET_PLUGINS_IPC_MESSAGE_LIB}
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

INSTALL(FILES
    ipc_message_support.h
    DESTINATION include/plugins-ipc-message)
