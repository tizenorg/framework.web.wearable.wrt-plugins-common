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
# @file        CMakeLists.txt
# @author      Andrzej Surdej (a.surdej@sasmung.com)
# @version     1.0
# @brief
#

SET(TARGET_WIDGETDB_TEST "wrt-plugins-common-tests-widgetdb")

PKG_SEARCH_MODULE(dpl REQUIRED dpl-efl)
PKG_SEARCH_MODULE(dpl-test REQUIRED dpl-test-efl)

INCLUDE_DIRECTORIES(
  ${dpl_INCLUDE_DIRS}
  ${dpl-test_INCLUDE_DIRS}
)

# Set widgetdb tests sources
SET(WIDGETDB_TESTS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/TestInit.cpp"
                           "${CMAKE_CURRENT_SOURCE_DIR}/WidgetDBTest.cpp")

WRT_TEST_BUILD(${TARGET_WIDGETDB_TEST} ${WIDGETDB_TESTS_SOURCES})
WRT_TEST_INSTALL(${TARGET_WIDGETDB_TEST})

TARGET_LINK_LIBRARIES(${TARGET_WIDGETDB_TEST}
  "wrt-plugins-widgetdb"
  ${dpl_LIBRARIES}
  ${dpl-test_LIBRARIES}
)

INSTALL(PROGRAMS
  ${CMAKE_CURRENT_SOURCE_DIR}/widgetdb_tests_prepare_db.sh
  DESTINATION bin
  )
