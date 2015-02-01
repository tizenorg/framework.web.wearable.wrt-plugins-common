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
cmake_minimum_required(VERSION 2.6)

SET(CMAKE_PROJECT_NAME wrt-plugins-common)
set(PACKAGE_NAME ${CMAKE_PROJECT_NAME})
project(${CMAKE_PROJECT_NAME})
STRING(REGEX MATCH "([^.]*)" CMAKE_PROJECT_API_VERSION "${CMAKE_PROJECT_VERSION}")

################################################################################
# Required platform modules

include(FindPkgConfig)

pkg_search_module(dpl REQUIRED dpl-efl>=0.2.21)

include_directories(
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

set(CONFIG_FILE_NAME "config.cmake")
set(LIBS_COMMONS ${dpl_LDFLAGS} )

set(DESTINATION_LIB_PREFIX lib)
set(DESTINATION_HEADER_PREFIX include/${PROJECT_NAME})

set(DESTINATION_HEADERS_NON_JS ${DESTINATION_HEADER_PREFIX}/Commons)
set(DESTINATION_HEADERS_JS ${DESTINATION_HEADER_PREFIX}/CommonsJavaScript)
set(DESTINATION_HEADERS_JS_OVERLAY ${DESTINATION_HEADER_PREFIX}/js-overlay)
set(DESTINATION_HEADERS_WRT_POPUP_RUNNER ${DESTINATION_HEADER_PREFIX}/popup-runner)

################################################################################
# Target platform

if (NOT DEFINED PLATFORM)
  set(PLATFORM "tizen")
endif ()

################################################################################
# Schema of plugin's configuration file

set(COMMON_CONFIG_DTD ${CMAKE_CURRENT_SOURCE_DIR}/config.dtd)
set(COMMON_CONFIG_DTD_DST /usr/etc/wrt-plugins)
INSTALL(FILES ${COMMON_CONFIG_DTD} DESTINATION ${COMMON_CONFIG_DTD_DST})

################################################################################
# Macros used for including plugins from AL.


function(add_subdir NAME)
  message(STATUS "Building: ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}")
  add_subdirectory(${NAME})
endfunction()

################################################################################
# Subdirectories

add_subdirectory(src)

IF(WITH_TESTS)
  add_subdirectory(tests)
ENDIF(WITH_TESTS)

################################################################################
# PKGCONFIG

set(PKGCONFIG_DIR ${PROJECT_SOURCE_DIR}/pkgconfigs)

macro(configure_and_install_pkg PKG_FILE)
    CONFIGURE_FILE(${PKGCONFIG_DIR}/${PKG_FILE}.in
               ${PKGCONFIG_DIR}/${PKG_FILE} @ONLY)
    INSTALL(FILES ${PKGCONFIG_DIR}/${PKG_FILE} DESTINATION lib/pkgconfig)
endmacro(configure_and_install_pkg)

configure_and_install_pkg(wrt-plugins-commons-javascript.pc)
configure_and_install_pkg(wrt-plugins-commons.pc)
configure_and_install_pkg(wrt-plugins-filesystem.pc)
configure_and_install_pkg(wrt-plugins-widget-interface.pc)
configure_and_install_pkg(wrt-plugins-widgetdb.pc)
configure_and_install_pkg(wrt-plugins-plugin-manager.pc)
configure_and_install_pkg(wrt-plugin-loading.pc)
configure_and_install_pkg(wrt-plugin-js-overlay.pc)
configure_and_install_pkg(wrt-popup-runner.pc)
configure_and_install_pkg(wrt-popup-ace-runner.pc)
configure_and_install_pkg(wrt-popup-wrt-runner.pc)
configure_and_install_pkg(wrt-plugins-api-support.pc)
configure_and_install_pkg(wrt-plugins-ipc-message.pc)
configure_and_install_pkg(wrt-dispatch-event.pc)

################################################################################
# Cache

set(PLATFORM "${PLATFORM}" CACHE STRING "Target platform" FORCE)
set(CMAKE_CONFIG_FILE_NAME "${CMAKE_CONFIG_FILE_NAME}" CACHE
    STRING "CMake configuration file name." FORCE)

################################################################################
# SMACK rule

INSTALL(FILES ${CMAKE_SOURCE_DIR}/wrt-popup-ace-runtime.efl DESTINATION /etc/smack/accesses.d)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/wrt-popup-wrt-runtime.efl DESTINATION /etc/smack/accesses.d)

################################################################################
# Summary

message(STATUS "PLATFORM = ${PLATFORM}")
