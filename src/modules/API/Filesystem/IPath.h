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
#ifndef WRTDEVICEAPIS_FILESYSTEM_IPATH_H_
#define WRTDEVICEAPIS_FILESYSTEM_IPATH_H_

#include <string>
#include <dpl/shared_ptr.h>

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class IPath;
typedef DPL::SharedPtr<IPath> IPathPtr;

class IPath
{
  public:
    typedef char SeparatorType;

  public:
    /**
     * Creates path object from specified string.
     * @param str Path string.
     * @return Path.
     * @throw InvalidArgumentException If supplied string is not a valid path.
     * @throw PlatformException If error in platform occurs.
     * @remarks Ownership passed to the caller.
     */
    static IPathPtr create(const std::string& str);

    /**
     * Gets separator used by current platform.
     * @return Path separator.
     */
    static SeparatorType getSeparator();

  public:
    virtual ~IPath() = 0;

    /**
     * Gets full path.
     * @return Full path.
     */
    virtual std::string getFullPath() const = 0;

    /**
     * Gets base path (full path w/o name).
     * @return Base path
     */
    virtual std::string getPath() const = 0;

    /**
     * Gets the last part of path.
     * @return Path's name.
     * @return Last part is typically name of a directory or file.
     */
    virtual std::string getName() const = 0;

    /**
     * Appends path specified as string to current path.
     * @param path Path to append.
     * @return Current path object.
     */
    virtual IPathPtr append(const std::string& path) = 0;

    /**
     * Appends path specified as path object to current path.
     * @param path Path to append.
     * @return Current path object.
     */
    virtual IPathPtr append(const IPathPtr& path) = 0;

    /**
     * Checks if path is abolute.
     * @return True if absolute, false if relative.
     */
    virtual bool isAbsolute() const = 0;

    /**
     * Clones this object.
     * @return Independent copy.
     */
    virtual IPathPtr clone() const = 0;
};

inline const IPathPtr operator+(const IPath& lhs,
                                const IPath& rhs)
{
    return IPath::create(lhs.getFullPath())->append(rhs.getFullPath());
}

inline const IPathPtr operator+(const IPath& lhs,
                                const std::string& rhs)
{
    return IPath::create(lhs.getFullPath())->append(rhs);
}

inline const IPathPtr operator+(const std::string& lhs,
                                const IPath& rhs)
{
    return IPath::create(lhs)->append(rhs.getFullPath());
}

inline bool operator==(const IPath& lhs, const IPath& rhs)
{
    return (lhs.getFullPath() == rhs.getFullPath());
}

inline bool operator==(const IPath& lhs, const std::string& rhs)
{
    return (lhs.getFullPath() == rhs);
}

inline bool operator==(const std::string& lhs, const IPath& rhs)
{
    return (lhs == rhs.getFullPath());
}

inline bool operator!=(const IPath& lhs, const IPath& rhs)
{
    return (lhs.getFullPath() != rhs.getFullPath());
}

inline bool operator!=(const IPath& lhs, const std::string& rhs)
{
    return (lhs.getFullPath() != rhs);
}

inline bool operator!=(const std::string& lhs, const IPath& rhs)
{
    return (lhs != rhs.getFullPath());
}
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_IPATH_H_
