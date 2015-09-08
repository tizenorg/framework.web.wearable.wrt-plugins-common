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
    INSTALL(FILES ${API_FILESYSTEM_PATH}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADER_PREFIX}/Filesystem)
ENDMACRO()

SET(TARGET_NAME ${TARGET_MODULE_FILESYSTEM})

SET(SRCS
  ${SRCS_API_FILESYSTEM}
  ${SRCS_PLATFORM_IMPLEMENTATION_FILESYSTEM}
)

ADD_LIBRARY(${TARGET_NAME} SHARED ${SRCS})
TARGET_LINK_LIBRARIES(${TARGET_NAME}
   ${LIBS_COMMONS}
   ${TARGET_COMMONS}
)
SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES 
 SOVERSION ${CMAKE_PROJECT_API_VERSION}
 VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION
    ${DESTINATION_LIB_PREFIX})

INSTALL_HEADER_FILE(IManager.h)
INSTALL_HEADER_FILE(INode.h)
INSTALL_HEADER_FILE(INodeTypes.h)
INSTALL_HEADER_FILE(IPath.h)
INSTALL_HEADER_FILE(IStream.h)
INSTALL_HEADER_FILE(EventResolve.h)
INSTALL_HEADER_FILE(EventCopy.h)
INSTALL_HEADER_FILE(EventMove.h)
INSTALL_HEADER_FILE(EventRemove.h)
INSTALL_HEADER_FILE(EventFind.h)
INSTALL_HEADER_FILE(EventListNodes.h)
INSTALL_HEADER_FILE(EventOpen.h)
INSTALL_HEADER_FILE(EventReadText.h)
INSTALL_HEADER_FILE(Enums.h)
INSTALL_HEADER_FILE(NodeFilter.h)
