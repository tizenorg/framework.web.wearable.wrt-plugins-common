get_current_path()

pkg_search_module(dpl-wrt-dao-ro REQUIRED dpl-wrt-dao-ro)
#pkg_search_module(wrt-plugin-loading REQUIRED wrt-plugin-loading)

set(INCLUDES_PLATFORM_IMPLEMENTATION_PLUGIN_MANAGER
  ${dpl-wrt-dao-ro_INCLUDE_DIRS}
  ${wrt-plugin-loading_INCLUDE_DIRS}
  ${PLUGIN_LOADING_DIRS}
  PARENT_SCOPE
)

set(LIBS_PLATFORM_IMPLEMENTATION_PLUGIN_MANAGER
  ${dpl-wrt-dao-ro_LIBRARIES}
  ${wrt-plugin-loading_LIBRARIES}
  ${TARGET_PLUGIN_LOADING_LIB}
  PARENT_SCOPE
)

set(SRCS_PLATFORM_IMPLEMENTATION_PLUGIN_MANAGER
  ${CURRENT_PATH}/PluginManager.cpp
  PARENT_SCOPE
)
