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
#include "EventFind.h"

#include <Filesystem/Path.h>
#include "INode.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
EventFind::EventFind(const IPathPtr& path) : m_path(path)
{}

IPathPtr EventFind::getPath() const
{
    return m_path;
}

NodeList EventFind::getResult() const
{
    return m_nodes;
}

void EventFind::addResult(const INodePtr& node)
{
    m_nodes.push_back(node);
}

void EventFind::setResult(const NodeList& nodes)
{
    m_nodes = nodes;
}

void EventFind::addFilter(FindFilter name,
                          const std::string& value)
{
    m_filters.insert(std::pair<FindFilter, std::string>(name, value));
}

FiltersMap EventFind::getFilters() const
{
    return m_filters;
}
} // API
} // Filesystem
} // WrtDeviceApis
