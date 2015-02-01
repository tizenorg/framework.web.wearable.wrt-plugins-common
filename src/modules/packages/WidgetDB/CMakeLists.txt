#
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
    INSTALL(FILES ${API_WIDGETDB_PATH}/${HEADER_FILE}
        DESTINATION ${DESTINATION_HEADER_PREFIX}/WidgetDB)
endmacro()

set(TARGET_NAME ${TARGET_MODULE_WIDGETDB})

include_directories(
    ${INCLUDES_PLATFORM_IMPLEMENTATION_WIDGETDB}
)

set(SRCS
  ${SRCS_API_WIDGETDB}
  ${SRCS_PLATFORM_IMPLEMENTATION_WIDGETDB}
)

add_library(${TARGET_NAME} SHARED ${SRCS})

target_link_libraries(${TARGET_NAME}
   ${LIBS_COMMONS}
   ${TARGET_COMMONS}
   ${LIBS_PLATFORM_IMPLEMENTATION_WIDGETDB}
)
set_target_properties(${TARGET_NAME} PROPERTIES 
 SOVERSION ${CMAKE_PROJECT_API_VERSION}
 VERSION ${CMAKE_PROJECT_VERSION}
)

INSTALL(TARGETS ${TARGET_NAME} LIBRARY DESTINATION
    ${DESTINATION_LIB_PREFIX})

install_header_file(IWidgetDB.h)
install_header_file(IWidgetFeature.h)
install_header_file(WidgetDBMgr.h)

