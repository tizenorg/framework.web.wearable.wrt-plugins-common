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

SET(TARGET_NAME ${TARGET_MODULE_WIDGET})

SET(SRCS
    ${SRCS_API_WIDGET}
    ${SRCS_IMPLEMENTATION_WIDGET}
)

ADD_LIBRARY(${TARGET_NAME} STATIC ${SRCS})

TARGET_LINK_LIBRARIES(${TARGET_NAME}
   ${LIBS_COMMONS}
   ${TARGET_COMMONS}
   ${TARGET_MODULE_WIDGETDB}
)

SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES
    COMPILE_DEFINITIONS LOG_TAG="${LOG_TAG}")
