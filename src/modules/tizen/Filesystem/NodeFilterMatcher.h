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

#ifndef WRTDEVICEAPIS_FILESYSTEM_NODEFILTERMATCHER_H_
#define WRTDEVICEAPIS_FILESYSTEM_NODEFILTERMATCHER_H_

#include <string>
#include <boost/optional.hpp>
#include <Filesystem/INode.h>
#include <Filesystem/NodeFilter.h>

namespace WrtDeviceApis {
namespace Filesystem {
class NodeFilterMatcher
{
  public:
    static bool match(const Api::INodePtr& value,
                      const Api::NodeFilterPtr& filter);

  private:
    static bool matchString(const std::string& value,
                            const Api::OptionalString& filter);

    template<typename Type>
    static bool matchRange(const Type& value,
                           const boost::optional<Type>& min,
                           const boost::optional<Type>& max);

  private:
    NodeFilterMatcher();
};
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_NODEFILTERMATCHER_H_
