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
#ifndef WRTDEVICEAPIS_FILESYSTEM_PATH_H_
#define WRTDEVICEAPIS_FILESYSTEM_PATH_H_

#include <string>
#include <vector>
#include <dpl/enable_shared_from_this.h>
#include <Filesystem/IPath.h>

namespace WrtDeviceApis {
namespace Filesystem {
class Path : public Api::IPath,
    public DPL::EnableSharedFromThis<Path>
{
  public:
    static Api::IPathPtr create(const std::string& path);
    static Api::IPath::SeparatorType getSeparator();

  public:
    std::string getFullPath() const;
    std::string getPath() const;
    std::string getName() const;
    Api::IPathPtr append(const std::string& path);
    Api::IPathPtr append(const Api::IPathPtr& path);
    bool isAbsolute() const;
    Api::IPathPtr clone() const;

  private:
    /**
     * Checks whether specified string is a valid path.
     * @param path String to verify.
     * @return True when string is a valid path, false otherwise.
     */
    static bool isValid(const std::string& str);

  private:
    Path();
    void reset(const std::string& str);

  private:
    static const SeparatorType m_pathSeparator; ///< Path separator.

  private:
    std::string m_fullPath; ///< Full path.
    std::string m_path;   ///< Base path.
    std::string m_name;   ///< Last part of the path.
};
} // Filesystem
} // WrtDeviceApis

#endif /* WRTDEVICEAPIS_FILESYSTEM_PATH_H_ */
