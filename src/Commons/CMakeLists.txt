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
macro(install_header_file HEADER_FILE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADERS_NON_JS})
endmacro()

macro(install_wrtaccess_header_file HEADER_FILE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADERS_NON_JS}/WrtAccess)
endmacro()

macro(install_widget_interface_header_file HEADER_FILE)
    INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADERS_NON_JS}/WidgetInterface)
endmacro()

include_config_file(WrtAccess)

pkg_search_module(plugin-types REQUIRED wrt-plugins-types)
pkg_search_module(ace-client REQUIRED security-client)
pkg_search_module(dpl-event REQUIRED dpl-event-efl)
pkg_search_module(icu REQUIRED icu-i18n)

set(WRT_COMMONS_DEPENDECIES_INCLUDES
    ${wrt-plugin-api_INCLUDE_DIRS}
    ${ace-client_INCLUDE_DIRS}
    ${plugin-types_INCLUDE_DIRS}
    ${icu_INCLUDE_DIRS}
    )

include_directories(${WRT_COMMONS_DEPENDECIES_INCLUDES})

set(TARGET_NAME ${TARGET_COMMONS})

set(SRCS
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

add_library(${TARGET_NAME} SHARED ${SRCS})
target_link_libraries(${TARGET_NAME}
  ${LIBS_COMMONS}
  ${ace-client_LIBRARIES}
  ${dpl-event_LIBRARIES}
  ${icu_LIBRARIES}
  ${TARGET_POPUP_ACE_RUNNER_LIB}
)

set_target_properties(${TARGET_NAME} PROPERTIES
 SOVERSION ${CMAKE_PROJECT_API_VERSION}
 VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX})

install_header_file(FunctionDeclaration.h)
install_header_file(FunctionDefinition.h)
install_header_file(Base64.h)
install_header_file(Deprecated.h)
install_header_file(Dimension.h)
install_header_file(Emitters.h)
install_header_file(EventListener.h)
install_header_file(EventReceiver.h)
install_header_file(Exception.h)
install_header_file(IEvent.h)
install_header_file(IExternEventCanceler.h)
install_header_file(ListenerEventEmitter.h)
install_header_file(ListenerEvent.h)
install_header_file(Range.h)
install_header_file(Regex.h)
install_header_file(RegexPatterns.h)
install_header_file(RegexUtils.h)
install_header_file(StaticAssert.h)
install_header_file(StringBuilder.h)
install_header_file(StringUtils.h)
install_header_file(ThreadPool.h)
install_header_file(TypesDeclaration.h)
install_header_file(TypeTraits.h)
install_header_file(plugin_initializer_def.h)
install_header_file(JSObjectDeclaration.h)
install_header_file(JSObject.h)
install_header_file(TimeUtils.h)
install_wrtaccess_header_file(WrtAccess/WrtAccess.h)
