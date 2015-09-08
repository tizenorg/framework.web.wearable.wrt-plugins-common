get_current_path()

pkg_search_module(pcrecpp REQUIRED libpcrecpp
                  ecore REQUIRED ecore-1)

set(INCLUDES_PLATFORM_IMPLEMENTATION_FILESYSTEM
  ${pcrecpp_INCLUDE_DIRS}
  ${ecore_INCLUDE_DIRS}
  PARENT_SCOPE
)

set(LIBS_PLATFORM_IMPLEMENTATION_FILESYSTEM
  ${pcrecpp_LIBRARIES}
  ${ecore_LIBRARIES}
  PARENT_SCOPE
)

set(SRCS_PLATFORM_IMPLEMENTATION_FILESYSTEM
  ${CURRENT_PATH}/Manager.cpp
  ${CURRENT_PATH}/Node.cpp
  ${CURRENT_PATH}/Path.cpp
  ${CURRENT_PATH}/Stream.cpp
  ${CURRENT_PATH}/NodeFilterMatcher.cpp
  ${CURRENT_PATH}/Utils.cpp
  PARENT_SCOPE
)
