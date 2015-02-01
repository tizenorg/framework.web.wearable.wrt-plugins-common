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
/**
 * @author          Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#include "NodeFilterMatcher.h"

#include <pcrecpp.h>

namespace WrtDeviceApis {
namespace Filesystem {
using namespace Api;

bool NodeFilterMatcher::match(const INodePtr& value,
                              const NodeFilterPtr& filter)
{
    if (filter) {
        if (!matchString(value->getPath()->getName(), filter->getName())) {
            return false;
        }

        if (!matchRange(value->getCreated(),
                        filter->getMinCreated(),
                        filter->getMaxCreated()))
        {
            return false;
        }

        if (!matchRange(value->getModified(),
                        filter->getMinModified(),
                        filter->getMaxModified()))
        {
            return false;
        }

        if (!matchRange(value->getSize(),
                        filter->getMinSize(),
                        filter->getMaxSize()))
        {
            return false;
        }
    }
    return true;
}

bool NodeFilterMatcher::matchString(const std::string& value,
                                    const OptionalString& filter)
{
    if (!!filter) {
        return pcrecpp::RE(*filter).PartialMatch(value);
    }
    return true;
}

template<typename Type>
bool NodeFilterMatcher::matchRange(const Type& value,
                                   const boost::optional<Type>& min,
                                   const boost::optional<Type>& max)
{
    if ((!!min && (value < *min)) ||
        (!!max && (value > *max)))
    {
        return false;
    }
    return true;
}
} // Filesystem
} // WrtDeviceApis
