
get_current_path()

set(WIDGET_INTERFACE_INCLUDE_DIRS
    ${CURRENT_PATH}
    PARENT_SCOPE
)

SET(API_WIDGET_INTERFACE_PATH
    ${CURRENT_PATH}
    PARENT_SCOPE
)

set(SRCS_WIDGET_INTERFACE
    ${CURRENT_PATH}/WidgetInterface.cpp
    PARENT_SCOPE
)
