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
#include "EventMove.h"
#include <dpl/assert.h>

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
EventMove::EventMove(const IPathPtr& src,
                     const IPathPtr& dest) :
    m_src(src),
    m_dest(dest),
    m_options(OPT_NONE)
{
    AssertMsg(src, "Source path not set.");
    AssertMsg(dest, "Destination path not set.");
}

IPathPtr EventMove::getDestination() const
{
    return m_dest;
}

IPathPtr EventMove::getSource() const
{
    return m_src;
}

INodePtr EventMove::getResult() const
{
    return m_result;
}

void EventMove::setResult(const INodePtr& node)
{
    m_result = node;
}

int EventMove::getOptions() const
{
    return m_options;
}

void EventMove::setOptions(int options)
{
    m_options = options;
}
} // API
} // Filesystem
} // WrtDeviceApis
