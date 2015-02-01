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
#ifndef WRTDEVICEAPIS_IEVENTFILESYSTEMFIND_H_
#define WRTDEVICEAPIS_IEVENTFILESYSTEMFIND_H_

#include <list>
#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "IPath.h"
#include "INodeTypes.h"
#include "Enums.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class EventFind : public Commons::IEvent<EventFind>
{
  public:
    /**
     * Creates new event object.
     * @param path Start path.
     * @return New object.
     */
    explicit EventFind(const IPathPtr& path);

    /**
     * Gets start path.
     * @return Path to start searching from.
     */
    IPathPtr getPath() const;

    /**
     * Gets found nodes.
     * @return Nodes.
     */
    NodeList getResult() const;

    /**
     * Adds found node.
     * @param node Found node.
     */
    void addResult(const INodePtr& node);

    /**
     * Sets found nodes.
     * @param nodes Nodes.
     */
    void setResult(const NodeList& nodes);

    /**
     * Adds find filter.
     * @param name Filter name.
     * @param value Filter value.
     */
    void addFilter(FindFilter name,
                   const std::string& value);

    /**
     * Gets all filters.
     * @return Filters.
     */
    FiltersMap getFilters() const;

  private:
    IPathPtr m_path;    ///< Start path.
    NodeList m_nodes;   ///< Resolved nodes.
    FiltersMap m_filters; ///< Search filters.
};

typedef DPL::SharedPtr<EventFind> EventFindPtr;
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_IEVENTFILESYSTEMFIND_H_
