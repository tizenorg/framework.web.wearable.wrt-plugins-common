get_current_path()

set(API_FILESYSTEM_PATH
    ${CURRENT_PATH}
    PARENT_SCOPE
)

set(SRCS_API_FILESYSTEM
  ${CURRENT_PATH}/IManager.cpp
  ${CURRENT_PATH}/IPath.cpp
  ${CURRENT_PATH}/IStream.cpp
  ${CURRENT_PATH}/EventResolve.cpp
  ${CURRENT_PATH}/EventCopy.cpp
  ${CURRENT_PATH}/EventMove.cpp
  ${CURRENT_PATH}/EventRemove.cpp
  ${CURRENT_PATH}/EventFind.cpp
  ${CURRENT_PATH}/EventListNodes.cpp
  ${CURRENT_PATH}/EventOpen.cpp
  ${CURRENT_PATH}/EventReadText.cpp
  ${CURRENT_PATH}/INode.cpp
  ${CURRENT_PATH}/NodeFilter.cpp
  ${CURRENT_PATH}/PathUtils.cpp
  PARENT_SCOPE
)

