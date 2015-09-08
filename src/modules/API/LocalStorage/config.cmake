get_current_path()

set(API_LOCALSTORAGE_PATH
    ${CURRENT_PATH}
    PARENT_SCOPE
    )

set(SRCS_API_LOCALSTORAGE
  ${CURRENT_PATH}/LocalStorageMgr.cpp
  PARENT_SCOPE
)
