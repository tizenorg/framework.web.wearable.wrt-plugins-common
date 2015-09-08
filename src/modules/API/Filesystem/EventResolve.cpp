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
#include "EventResolve.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
EventResolve::EventResolve(const IPathPtr& path) : m_path(path)
{}

IPathPtr EventResolve::getPath() const
{
    return m_path;
}

INodePtr EventResolve::getResult() const
{
    return m_node;
}

void EventResolve::setResult(const INodePtr& node)
{
    m_node = node;
}
} // API
} // Filesystem
} // WrtDeviceApis
