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
# @author   Janusz Kozerski (j.kozerski@samsung.com)
#
# @version  1.0
#

PKG_CHECK_MODULES(WRT_POPUP_DEP
    dpl-efl
    elementary
    security-client
    security-popup-validation
   REQUIRED)

SET(WRT_POPUP_SRC_DIR ${PROJECT_SOURCE_DIR}/src/wrt-popup/ace/popup-bin)

SET(WRT_POPUP_SOURCES
    ${WRT_POPUP_SRC_DIR}/Popup.cpp
)

ADD_DEFINITIONS(${WRT_POPUP_DEP_CFLAGS})

INCLUDE_DIRECTORIES(
    ${WRT_POPUP_SRC_DIR}
    ${WRT_POPUP_DEP_INCLUDE_DIRS}
)

ADD_EXECUTABLE(${TARGET_POPUP_ACE}
    ${WRT_POPUP_SOURCES}
)

ADD_DEFINITIONS("-fPIE")

SET_TARGET_PROPERTIES(${TARGET_POPUP_ACE} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")

SET_TARGET_PROPERTIES(${TARGET_POPUP_ACE} PROPERTIES
    COMPILE_FLAGS -fPIC
)

SET_TARGET_PROPERTIES(${TARGET_POPUP_ACE} PROPERTIES
    LINK_FLAGS "-Wl,--as-needed -pie"
    SOVERSION ${PROJECT_API_VERSION}
    VERSION ${PROJECT_VERSION}
)

TARGET_LINK_LIBRARIES(${TARGET_POPUP_ACE}
    ${WRT_POPUP_DEP_LIBRARIES}
    ${TARGET_POPUP_ACE_RUNNER_LIB}
    ${WRT_POPUP_DEP_LDFLAGS}
)

INSTALL(TARGETS ${TARGET_POPUP_ACE}
    DESTINATION bin
)
