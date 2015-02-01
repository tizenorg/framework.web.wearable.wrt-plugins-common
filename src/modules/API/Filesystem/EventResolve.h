/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef WRTDEVICEAPIS_FILESYSTEM_EVENTRESOLVE_H_
#define WRTDEVICEAPIS_FILESYSTEM_EVENTRESOLVE_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "IPath.h"
#include "INode.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class EventResolve : public Commons::IEvent<EventResolve>
{
  public:
    /**
     * Creates new event object.
     * @param path Path to the node to resolve.
     * @return New object.
     */
    explicit EventResolve(const IPathPtr& path);

    /**
     * Gets path of the node to resolve.
     * @return Path to resolve.
     */
    IPathPtr getPath() const;

    /**
     * Gets resolved node.
     * NULL if none found. Check exception code.
     * @return Resolved node or NULL.
     */
    INodePtr getResult() const;

    /**
     * Sets resolved node.
     * @param node Resolved node.
     */
    void setResult(const INodePtr& node);

  private:
    IPathPtr m_path; ///< Path to the requested node.
    INodePtr m_node; ///< Resolved node.
};

typedef DPL::SharedPtr<EventResolve> EventResolvePtr;
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_EVENTRESOLVE_H_
