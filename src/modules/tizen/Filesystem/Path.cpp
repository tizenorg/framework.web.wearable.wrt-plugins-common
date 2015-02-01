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
#include <memory>
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <limits.h>
#include <dpl/log/log.h>
#include <dpl/assert.h>
#include <Commons/Exception.h>
#include <Commons/StringUtils.h>
#include "Path.h"

namespace WrtDeviceApis {
namespace Filesystem {
using namespace Api;

const Path::SeparatorType Path::m_pathSeparator = '/';

IPathPtr Path::create(const std::string& path)
{
    DPL::SharedPtr<Path> result(new Path());
    result->reset(path);
    return DPL::StaticPointerCast<IPath>(result);
}

std::string Path::getFullPath() const
{
    return m_fullPath;
}

std::string Path::getPath() const
{
    return m_path;
}

std::string Path::getName() const
{
    return m_name;
}

IPathPtr Path::append(const std::string& path)
{
    reset(m_fullPath + m_pathSeparator + path);
    return DPL::StaticPointerCast<IPath>(SharedFromThis());
}

IPathPtr Path::append(const IPathPtr& path)
{
    reset(m_fullPath + m_pathSeparator + path->getFullPath());
    return DPL::StaticPointerCast<IPath>(SharedFromThis());
}

bool Path::isAbsolute() const
{
    return (!m_fullPath.empty() && (m_fullPath[0] == m_pathSeparator));
}

IPath::SeparatorType Path::getSeparator()
{
    return m_pathSeparator;
}

bool Path::isValid(const std::string& str)
{
    return !str.empty();
}

IPathPtr Path::clone() const
{
    return Path::create(m_fullPath);
}

Path::Path()
{}

void Path::reset(const std::string& str)
{
    if (!isValid(str)) {
        ThrowMsg(Commons::InvalidArgumentException,
                 "Not a valid path: " + str + ".");
    }

    std::string tmp = Commons::String::unique(Commons::String::trim(
                                                  str), m_pathSeparator);
    std::string::size_type pos = tmp.find_last_of(m_pathSeparator);
    if (pos == std::string::npos) {
        m_fullPath = m_name = tmp;
        m_path.clear();
    } else {
        if (0 == pos) {
            m_fullPath = m_path = m_pathSeparator;
        } else {
            m_fullPath = m_path = tmp.substr(0, pos);
            m_fullPath += m_pathSeparator;
        }
        m_name = tmp.substr(pos + 1);
        m_fullPath += m_name;
    }
}
} // Filesystem
} // WrtDeviceApis
