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
#ifndef WRTDEVICEAPIS_FILESYSTEM_EVENTLISTNODES_H_
#define WRTDEVICEAPIS_FILESYSTEM_EVENTLISTNODES_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "INodeTypes.h"
#include "NodeFilter.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class EventListNodes : public Commons::IEvent<EventListNodes>
{
  public:
    /**
     * Creates new event object.
     * @param path Node to list children for.
     * @return New object.
     */
    explicit EventListNodes(const INodePtr& node);

    /**
     * Gets parent node.
     * @return Parent node.
     */
    INodePtr getNode() const;

    /**
     * Gets filter.
     * @return Filter.
     */
    NodeFilterPtr getFilter() const;

    /**
     * Sets filter.
     * @param filter Filter to list only specific nodes.
     */
    void setFilter(const NodeFilterPtr& filter);

    /**
     * Gets nodes.
     * @return Nodes list.
     */
    NodeList getResult() const;

    /**
     * Sets nodes list.
     * @param list Nodes list.
     */
    void setResult(const NodeList& list);

  private:
    NodeList m_list;      ///< List of child nodes.
    INodePtr m_node;      ///< Node to list children for.
    NodeFilterPtr m_filter; ///< Filter.
};

typedef DPL::SharedPtr<EventListNodes> EventListNodesPtr;
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_EVENTLISTNODES_H_
