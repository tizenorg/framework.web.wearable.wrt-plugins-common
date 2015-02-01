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
# @author   Tae-Jeong Lee (taejeong.lee@samsung.com)
# @version  1.0
#

pkg_search_module(webkit2 REQUIRED ewebkit2)
pkg_search_module(dpl REQUIRED dpl-efl)

SET(TARGET_NAME wrt-dispatch-event)

SET(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/dispatch_event_support.cpp
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${webkit2_INCLUDE_DIRS}
    ${dpl_INCLUDE_DIRS}
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})

TARGET_LINK_LIBRARIES(${TARGET_NAME}
    ${webkit2_LIBRARIES}
    ${dpl_LIBRARIES}
)

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    COMPILE_FLAGS -fPIC
    LINK_FLAGS "-Wl,--as-needed -Wl,--hash-style=both"
)

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    SOVERSION ${CMAKE_PROJECT_API_VERSION}
    VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME}
    DESTINATION ${DESTINATION_LIB_PREFIX}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

INSTALL(FILES dispatch_event_support.h DESTINATION include/wrt-dispatch-event)