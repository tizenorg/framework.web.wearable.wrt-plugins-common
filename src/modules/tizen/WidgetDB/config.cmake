get_current_path()

pkg_search_module(dpl-wrt-dao-ro REQUIRED dpl-wrt-dao-ro)
pkg_search_module(dpl-utils-efl REQUIRED dpl-utils-efl)

set(INCLUDES_PLATFORM_IMPLEMENTATION_WIDGETDB
  ${dpl-wrt-dao-ro_INCLUDE_DIRS}
  ${dpl-utils-efl_INCLUDE_DIRS}
  PARENT_SCOPE
)

set(LIBS_PLATFORM_IMPLEMENTATION_WIDGETDB
  ${dpl-wrt-dao-ro_LDFLAGS}
  ${dpl-utils-efl_LDFLAGS}
  PARENT_SCOPE
)

set(SRCS_PLATFORM_IMPLEMENTATION_WIDGETDB
  ${CURRENT_PATH}/WidgetDB.cpp
  ${CURRENT_PATH}/WidgetFeature.cpp
  PARENT_SCOPE
)
