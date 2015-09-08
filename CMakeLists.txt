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
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_PROJECT_NAME wrt-plugins-common)
SET(PACKAGE_NAME ${CMAKE_PROJECT_NAME})
PROJECT(${CMAKE_PROJECT_NAME})
STRING(REGEX MATCH "([^.]*)" CMAKE_PROJECT_API_VERSION "${CMAKE_PROJECT_VERSION}")

################################################################################
# Required platform modules

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(dpl REQUIRED dpl-efl>=0.2.21)

INCLUDE_DIRECTORIES(
  ${dpl_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/API
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Commons
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CommonsJavaScript
  ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin-loading
  ${CMAKE_CURRENT_SOURCE_DIR}/src/js-overlay
  ${CMAKE_CURRENT_SOURCE_DIR}/src/wrt-popup/ace/popup-runner
  ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins_ipc_message
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dispatch-event)

##############################################################################
# Build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT CMAKE_BUILD_TYPE)

# Compiler flags
SET(CMAKE_C_FLAGS_PROFILING    "-O2")
SET(CMAKE_CXX_FLAGS_PROFILING  "-O2 -std=c++0x")
SET(CMAKE_C_FLAGS_DEBUG        "-O0 -g")
SET(CMAKE_CXX_FLAGS_DEBUG      "-O0 -std=c++0x -g")
SET(CMAKE_C_FLAGS_RELEASE      "-Os")
SET(CMAKE_CXX_FLAGS_RELEASE    "-Os -std=c++0x -fvisibility-inlines-hidden")
SET(CMAKE_CXX_FLAGS_CCOV       "-O0 -std=c++0x -g --coverage")
####

OPTION(DPL_LOG "DPL logs status" ON)
SET(LOG_TAG "WRT_PLUGINS/COMMON")
OPTION(WITH_TESTS "Build tests" OFF)

IF(DPL_LOG AND NOT CMAKE_BUILD_TYPE MATCHES "profiling")
    MESSAGE(STATUS "Logging enabled for DPL")
    ADD_DEFINITIONS("-DDPL_LOGS_ENABLED")
ELSE(DPL_LOG AND NOT CMAKE_BUILD_TYPE MATCHES "profiling")
    MESSAGE(STATUS "Logging disabled for DPL")
ENDIF(DPL_LOG AND NOT CMAKE_BUILD_TYPE MATCHES "profiling")

MESSAGE(STATUS "WITH_TESTS: " ${WITH_TESTS})

ADD_DEFINITIONS("-fPIC")
ADD_DEFINITIONS("-fvisibility=default")         # mark all exported symbols as visible
ADD_DEFINITIONS("-fPIC")                        # If supported for the target machine, emit position-independent code, suitable for dynamic linking and avoiding
ADD_DEFINITIONS("-DEXPORT_API=")
ADD_DEFINITIONS("-Wall")
#ADD_DEFINITIONS("-Werror")
ADD_DEFINITIONS("-Wextra")
ADD_DEFINITIONS("-Wno-deprecated")
#ADD_DEFINITIONS("-DDEVPKG")
# GCC 4.7 changes the scheme of name lookup. Guide: http://gcc.gnu.org/gcc-4.7/porting_to.html
ADD_DEFINITIONS("-fpermissive")

##############################################################################
# CMake flags
IF (DEVPKG)
    SET(DEVPKG "ON")
ENDIF(DEVPKG)

################################################################################
# Miscellaneous variables

SET(CONFIG_FILE_NAME "config.cmake")
SET(LIBS_COMMONS ${dpl_LDFLAGS} )

SET(DESTINATION_LIB_PREFIX lib)
SET(DESTINATION_HEADER_PREFIX include/${PROJECT_NAME})

SET(DESTINATION_HEADERS_NON_JS ${DESTINATION_HEADER_PREFIX}/Commons)
SET(DESTINATION_HEADERS_JS ${DESTINATION_HEADER_PREFIX}/CommonsJavaScript)
SET(DESTINATION_HEADERS_JS_OVERLAY ${DESTINATION_HEADER_PREFIX}/js-overlay)
SET(DESTINATION_HEADERS_WRT_POPUP_RUNNER ${DESTINATION_HEADER_PREFIX}/popup-runner)

################################################################################
# Target platform

IF (NOT DEFINED PLATFORM)
  SET(PLATFORM "tizen")
ENDIF ()

################################################################################
# Schema of plugin's configuration file

SET(COMMON_CONFIG_DTD ${CMAKE_CURRENT_SOURCE_DIR}/config.dtd)
SET(COMMON_CONFIG_DTD_DST /usr/etc/wrt-plugins)
INSTALL(FILES ${COMMON_CONFIG_DTD} DESTINATION ${COMMON_CONFIG_DTD_DST})

################################################################################
# Macros used for including plugins from AL.


FUNCTION(add_subdir NAME)
  MESSAGE(STATUS "Building: ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}")
  ADD_SUBDIRECTORY(${NAME})
ENDFUNCTION()

################################################################################
# Subdirectories

ADD_SUBDIRECTORY(src)

IF(WITH_TESTS)
  ADD_SUBDIRECTORY(tests)
ENDIF(WITH_TESTS)

################################################################################
# PKGCONFIG

SET(PKGCONFIG_DIR ${PROJECT_SOURCE_DIR}/pkgconfigs)

MACRO(configure_and_install_pkg PKG_FILE)
    CONFIGURE_FILE(${PKGCONFIG_DIR}/${PKG_FILE}.in
               ${PKGCONFIG_DIR}/${PKG_FILE} @ONLY)
    INSTALL(FILES ${PKGCONFIG_DIR}/${PKG_FILE} DESTINATION lib/pkgconfig)
ENDMACRO(configure_and_install_pkg)

CONFIGURE_AND_INSTALL_PKG(wrt-plugins-commons-javascript.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugins-commons.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugins-filesystem.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugins-widget-interface.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugins-widgetdb.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugins-plugin-manager.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugin-loading.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugin-js-overlay.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-popup-runner.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-popup-ace-runner.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-popup-wrt-runner.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugins-api-support.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-plugins-ipc-message.pc)
CONFIGURE_AND_INSTALL_PKG(wrt-dispatch-event.pc)

################################################################################
# Cache

SET(PLATFORM "${PLATFORM}" CACHE STRING "Target platform" FORCE)
SET(CMAKE_CONFIG_FILE_NAME "${CMAKE_CONFIG_FILE_NAME}" CACHE
    STRING "CMake configuration file name." FORCE)

################################################################################
# SMACK rule

INSTALL(FILES ${CMAKE_SOURCE_DIR}/wrt-popup-ace-runtime.efl DESTINATION /etc/smack/accesses.d)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/wrt-popup-wrt-runtime.efl DESTINATION /etc/smack/accesses.d)

################################################################################
# Summary

MESSAGE(STATUS "PLATFORM = ${PLATFORM}")
