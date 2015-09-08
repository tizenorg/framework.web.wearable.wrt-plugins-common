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
MACRO(install_header_file HEADER_FILE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADERS_NON_JS})
ENDMACRO()

MACRO(install_wrtaccess_header_file HEADER_FILE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADERS_NON_JS}/WrtAccess)
ENDMACRO()

MACRO(install_widget_interface_header_file HEADER_FILE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADERS_NON_JS}/WidgetInterface)
ENDMACRO()

INCLUDE_CONFIG_FILE(WrtAccess)

PKG_SEARCH_MODULE(plugin-types REQUIRED wrt-plugins-types)
PKG_SEARCH_MODULE(ace-client REQUIRED security-client)
PKG_SEARCH_MODULE(dpl-event REQUIRED dpl-event-efl)
PKG_SEARCH_MODULE(icu REQUIRED icu-i18n)
PKG_SEARCH_MODULE(pcrecpp REQUIRED libpcrecpp)

SET(WRT_COMMONS_DEPENDECIES_INCLUDES
    ${wrt-plugin-api_INCLUDE_DIRS}
    ${ace-client_INCLUDE_DIRS}
    ${plugin-types_INCLUDE_DIRS}
    ${icu_INCLUDE_DIRS}
)

INCLUDE_DIRECTORIES(${WRT_COMMONS_DEPENDECIES_INCLUDES})

SET(TARGET_NAME ${TARGET_COMMONS})

SET(SRCS
  ${SRCS_WRT_ACCESS}
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/FunctionDefinition.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Regex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/RegexUtils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/StringBuilder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/StringUtils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPool.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/JSObjectDeclaration.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/JSObject.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/TimeUtils.cpp
  #PARENT_SCOPE
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET_NAME}
  ${LIBS_COMMONS}
  ${ace-client_LIBRARIES}
  ${dpl-event_LIBRARIES}
  ${icu_LIBRARIES}
  ${pcrecpp_LIBRARIES}
  ${TARGET_POPUP_ACE_RUNNER_LIB}
)

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
 SOVERSION ${CMAKE_PROJECT_API_VERSION}
 VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX})

INSTALL_HEADER_FILE(FunctionDeclaration.h)
INSTALL_HEADER_FILE(FunctionDefinition.h)
INSTALL_HEADER_FILE(Base64.h)
INSTALL_HEADER_FILE(Deprecated.h)
INSTALL_HEADER_FILE(Dimension.h)
INSTALL_HEADER_FILE(Emitters.h)
INSTALL_HEADER_FILE(EventListener.h)
INSTALL_HEADER_FILE(EventReceiver.h)
INSTALL_HEADER_FILE(Exception.h)
INSTALL_HEADER_FILE(IEvent.h)
INSTALL_HEADER_FILE(IExternEventCanceler.h)
INSTALL_HEADER_FILE(ListenerEventEmitter.h)
INSTALL_HEADER_FILE(ListenerEvent.h)
INSTALL_HEADER_FILE(Range.h)
INSTALL_HEADER_FILE(Regex.h)
INSTALL_HEADER_FILE(RegexPatterns.h)
INSTALL_HEADER_FILE(RegexUtils.h)
INSTALL_HEADER_FILE(StaticAssert.h)
INSTALL_HEADER_FILE(StringBuilder.h)
INSTALL_HEADER_FILE(StringUtils.h)
INSTALL_HEADER_FILE(ThreadPool.h)
INSTALL_HEADER_FILE(TypesDeclaration.h)
INSTALL_HEADER_FILE(TypeTraits.h)
INSTALL_HEADER_FILE(plugin_initializer_def.h)
INSTALL_HEADER_FILE(JSObjectDeclaration.h)
INSTALL_HEADER_FILE(JSObject.h)
INSTALL_HEADER_FILE(TimeUtils.h)
INSTALL_WRTACCESS_HEADER_FILE(WrtAccess/WrtAccess.h)
