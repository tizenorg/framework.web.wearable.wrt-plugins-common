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
        DESTINATION ${DESTINATION_HEADERS_JS})
ENDMACRO()


PKG_SEARCH_MODULE(webkit2 REQUIRED ewebkit2)

SET(TARGET_NAME ${TARGET_COMMONS_JAVASCRIPT})

INCLUDE_DIRECTORIES(${DIR_JS_COMMONS}
                    ${wrt-plugin-api_INCLUDE_DIRS}
                    ${webkit2_INCLUDE_DIRS}}
                    )


SET(SRCS
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
  #support safe protect
  ${CMAKE_CURRENT_SOURCE_DIR}/JSLifeManager.cpp
)


ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET_NAME}
        ${LIBS_COMMONS}
        ${webkit2_LIBRARIES}
        ${TARGET_COMMONS}
)

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
 SOVERSION ${CMAKE_PROJECT_API_VERSION}
 VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION ${DESTINATION_LIB_PREFIX})

#utils
INSTALL_HEADER_FILE(PrivateObject.h)
#tools
INSTALL_HEADER_FILE(Converter.h)
INSTALL_HEADER_FILE(Validator.h)
INSTALL_HEADER_FILE(JSUtils.h)
INSTALL_HEADER_FILE(ScopedJSStringRef.h)
#callback manager
INSTALL_HEADER_FILE(JSCallbackManager.h)
#exceptions
INSTALL_HEADER_FILE(JSDOMException.h)
INSTALL_HEADER_FILE(JSDOMExceptionFactory.h)
INSTALL_HEADER_FILE(DOMExceptionData.h)
INSTALL_HEADER_FILE(SecurityExceptions.h)
#static declarations
INSTALL_HEADER_FILE(Security/StaticDeclaration.h)
INSTALL_HEADER_FILE(Security/SecurityFunctionDeclaration.h)
#pending operation
INSTALL_HEADER_FILE(SupportPendingOperation.h)
INSTALL_HEADER_FILE(Utils.h)
INSTALL_HEADER_FILE(JSPendingOperation.h)
INSTALL_HEADER_FILE(JSPendingOperationPrivateObject.h)
#Safe Protect
INSTALL_HEADER_FILE(JSLifeManager.h)

