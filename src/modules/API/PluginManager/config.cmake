get_current_path()

set(API_PLUGIN_MANAGER_PATH
    ${CURRENT_PATH}
    PARENT_SCOPE
)

set(SRCS_API_PLUGIN_MANAGER
    ${CURRENT_PATH}/PluginManagerFactory.cpp
    PARENT_SCOPE
)
