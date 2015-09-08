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
#
# @version  1.0
#

PKG_CHECK_MODULES(WRT_POPUP_RUNNER_LIB_DEP
    dpl-efl
    REQUIRED
)

SET(WRT_POPUP_RUNNER_LIB_SRC_DIR ${PROJECT_SOURCE_DIR}/src/wrt-popup/wrt/popup-runner)

SET(WRT_POPUP_RUNNER_LIB_SOURCES
    ${WRT_POPUP_COMMON_SRCS}
    ${WRT_POPUP_RUNNER_LIB_SRC_DIR}/PopupInvoker.cpp
)

ADD_DEFINITIONS(${WRT_POPUP_RUNNER_LIB_DEP_CFLAGS})

INCLUDE_DIRECTORIES(
    ${WRT_POPUP_RUNNER_LIB_SRC_DIR}
    ${WRT_POPUP_RUNNER_LIB_DEP_INCLUDE_DIRS}
    ${WRT_POPUP_COMMON_DIR}
)

ADD_LIBRARY(${TARGET_POPUP_WRT_RUNNER_LIB} SHARED
    ${WRT_POPUP_RUNNER_LIB_SOURCES}
)

SET_TARGET_PROPERTIES(${TARGET_POPUP_WRT_RUNNER_LIB} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")

SET_TARGET_PROPERTIES(${TARGET_POPUP_WRT_RUNNER_LIB} PROPERTIES
    COMPILE_FLAGS -fPIC
)

SET_TARGET_PROPERTIES(${TARGET_POPUP_WRT_RUNNER_LIB} PROPERTIES
    SOVERSION ${CMAKE_PROJECT_API_VERSION}
    VERSION ${CMAKE_PROJECT_VERSION}
)

TARGET_LINK_LIBRARIES(${TARGET_POPUP_WRT_RUNNER_LIB}
    ${WRT_POPUP_RUNNER_LIB_DEP_LIBRARIES}
    ${WRT_POPUP_RUNNER_LIB_DEP_LDFLAGS}
)

INSTALL(TARGETS ${TARGET_POPUP_WRT_RUNNER_LIB}
    DESTINATION lib
)

INSTALL(FILES ${WRT_POPUP_RUNNER_LIB_SRC_DIR}/PopupInvoker.h
    DESTINATION ${DESTINATION_HEADERS_WRT_POPUP_RUNNER}
)
