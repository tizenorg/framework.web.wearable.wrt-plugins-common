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
# @author   Andrzej Surdej (a.surdej@samsung.com)
# @version  1.0
#

SET(TARGET_PLUGINS_INSTALLER "wrt-plugins-installer")

SET(WRT_PLUGINS_INSTALLER_SRC_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
)

PKG_CHECK_MODULES(PLUGINS_INSTALLER_DEPS
    libxml-2.0
    cert-svc-vcore
    dpl-efl
    dpl-wrt-dao-rw
    libpcrecpp
    REQUIRED
)

INCLUDE_DIRECTORIES(
    ${WRT_PLUGINS_INSTALLER_SRC_DIR}
    ${PLUGINS_INSTALLER_INCLUDES}
    ${PLUGINS_INSTALLER_DEPS_INCLUDE_DIRS}
    ${PLUGINS_API_SUPPORT_DIRS}
)

SET(WRT_PLUGINS_INSTALLER_SOURCES
    ${WRT_PLUGINS_INSTALLER_SRC_DIR}/wrt_plugins_installer.cpp
    ${WRT_PLUGINS_INSTALLER_SRC_DIR}/plugin_installer.cpp
    ${WRT_PLUGINS_INSTALLER_SRC_DIR}/plugin_utils.cpp
    ${WRT_PLUGINS_INSTALLER_SRC_DIR}/plugin_objects.cpp
    ${WRT_PLUGINS_INSTALLER_SRC_DIR}/plugin_metafile_reader.cpp
)

ADD_DEFINITIONS(${PLUGINS_INSTALLER_DEPS_CFLAGS})
ADD_DEFINITIONS(${PLUGINS_INSTALLER_DEPS_CFLAGS_OTHERS})

ADD_EXECUTABLE(${TARGET_PLUGINS_INSTALLER}
    ${WRT_PLUGINS_INSTALLER_SOURCES}
)

TARGET_LINK_LIBRARIES(${TARGET_PLUGINS_INSTALLER}
    ${PLUGINS_INSTALLER_DEPS_LIBRARIES}
    ${TARGET_PLUGINS_API_SUPPORT} "-ldl"
)

SET_TARGET_PROPERTIES(${TARGET_PLUGINS_INSTALLER} PROPERTIES
    LINK_FLAGS "-Wl,--as-needed -Wl,--hash-style=both"
    BUILD_WITH_INSTALL_RPATH ON
    INSTALL_RPATH_USE_LINK_PATH ON
)

INSTALL(TARGETS ${TARGET_PLUGINS_INSTALLER} DESTINATION bin)
