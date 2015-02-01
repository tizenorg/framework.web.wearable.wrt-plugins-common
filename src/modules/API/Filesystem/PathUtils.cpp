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

#include "PathUtils.h"
#include <dpl/log/log.h>

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
namespace PathUtils {
PathList getParts(const IPathPtr& path)
{
    PathList result;
    IPath::SeparatorType separator = path->getSeparator();
    std::string fullPath = path->getFullPath();
    std::string::size_type pos = 0;
    while ((pos = fullPath.find(separator, pos + 1)) != std::string::npos) {
        result.push_back(IPath::create(fullPath.substr(0, pos)));
    }
    return result;
}
} // PathUtils
} // API
} // Filesystem
} // WrtDeviceApis
