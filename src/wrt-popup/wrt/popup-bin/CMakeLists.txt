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

PKG_CHECK_MODULES(WRT_POPUP_DEPS
    ecore-x
    capi-appfw-application
    dpl-efl
    dpl-event-efl
    REQUIRED
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
    ${WRT_POPUP_DEPS_INCLUDE_DIRS}
    ${WRT_POPUP_COMMON_DIR}
)

SET(WRT_POPUP_SRCS
    ${WRT_POPUP_COMMON_SRCS}
    wrt-popup.cpp
    YesNoPopup.cpp
    YesNoCheckPopup.cpp
    InfoPopup.cpp
    renderer/evas_object.cpp
    renderer/popup_controller.cpp
    renderer/popup_manager.cpp
    renderer/popup_renderer.cpp
)

ADD_EXECUTABLE(${TARGET_POPUP_WRT}
    ${WRT_POPUP_SRCS}
)

ADD_DEFINITIONS("-fPIE")

TARGET_LINK_LIBRARIES(${TARGET_POPUP_WRT}
    ${WRT_POPUP_DEPS_LIBRARIES}
)

SET_TARGET_PROPERTIES(${TARGET_POPUP_WRT} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")

SET_TARGET_PROPERTIES(${TARGET_POPUP_WRT} PROPERTIES
    LINK_FLAGS "-Wl,--as-needed -pie -Wl,--hash-style=both"
    BUILD_WITH_INSTALL_RPATH ON
    INSTALL_RPATH_USE_LINK_PATH ON
)

INSTALL(TARGETS ${TARGET_POPUP_WRT} DESTINATION bin)
