get_current_path()

set(API_STORAGE_EVENT_PATH
    ${CURRENT_PATH}
    PARENT_SCOPE
    )

set(SRCS_API_STORAGE_EVENT
  ${CURRENT_PATH}/StorageEventMgr.cpp
  ${CURRENT_PATH}/StorageEvent.cpp
  PARENT_SCOPE
)
