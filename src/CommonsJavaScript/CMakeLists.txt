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
        DESTINATION ${DESTINATION_HEADERS_JS})
endmacro()


pkg_search_module(webkit2 REQUIRED ewebkit2)

set(TARGET_NAME ${TARGET_COMMONS_JAVASCRIPT})

include_directories(${DIR_JS_COMMONS}
                    ${wrt-plugin-api_INCLUDE_DIRS}
                    ${webkit2_INCLUDE_DIRS}}
                    )


set(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/Converter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ScopedJSStringRef.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/JSCallbackManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/JSUtils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ScopedJSStringRef.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Validator.cpp
  # wrapper for simple version of aceCheckAcces
  ${CMAKE_CURRENT_SOURCE_DIR}/Security/SecurityFunctionDeclaration.cpp
  #ECMA Script Exception
  ${CMAKE_CURRENT_SOURCE_DIR}/JSDOMException.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/DOMExceptionData.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/JSDOMExceptionFactory.cpp
  #Pending Operation support
  ${CMAKE_CURRENT_SOURCE_DIR}/JSPendingOperation.cpp
)


add_library(${TARGET_NAME} SHARED ${SRCS})
target_link_libraries(${TARGET_NAME}
        ${LIBS_COMMONS}
        ${webkit2_LIBRARIES}
        ${TARGET_COMMONS}
)
set_target_properties(${TARGET_NAME} PROPERTIES
 SOVERSION ${CMAKE_PROJECT_API_VERSION}
 VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX})

#utils
install_header_file(PrivateObject.h)
#tools
install_header_file(Converter.h)
install_header_file(Validator.h)
install_header_file(JSUtils.h)
install_header_file(ScopedJSStringRef.h)
#callback manager
install_header_file(JSCallbackManager.h)
#exceptions
install_header_file(JSDOMException.h)
install_header_file(JSDOMExceptionFactory.h)
install_header_file(DOMExceptionData.h)
install_header_file(SecurityExceptions.h)
#static declarations
install_header_file(Security/StaticDeclaration.h)
install_header_file(Security/SecurityFunctionDeclaration.h)
#pending operation
install_header_file(SupportPendingOperation.h)
install_header_file(Utils.h)
install_header_file(JSPendingOperation.h)
install_header_file(JSPendingOperationPrivateObject.h)

