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

#ifndef WRTDEVICEAPIS_FILESYSTEM_NODEFILTER_H_
#define WRTDEVICEAPIS_FILESYSTEM_NODEFILTER_H_

#include <ctime>
#include <cstddef>
#include <string>
#include <boost/optional.hpp>
#include <dpl/shared_ptr.h>
#include <Commons/Range.h>

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
typedef boost::optional<std::string> OptionalString;
typedef boost::optional<std::time_t> OptionalDate;
typedef boost::optional<std::size_t> OptionalSize;

class NodeFilter
{
  public:
    OptionalString getName() const;
    void setName(const OptionalString& name);

    OptionalDate getMinCreated() const;
    void setMinCreated(const OptionalDate& date);

    OptionalDate getMaxCreated() const;
    void setMaxCreated(const OptionalDate& date);

    void setCreated(const OptionalDate& date);

    OptionalDate getMinModified() const;
    void setMinModified(const OptionalDate& date);

    OptionalDate getMaxModified() const;
    void setMaxModified(const OptionalDate& date);

    void setModified(const OptionalDate& date);

    OptionalSize getMinSize() const;
    void setMinSize(const OptionalSize& size);

    OptionalSize getMaxSize() const;
    void setMaxSize(const OptionalSize& size);

    void setSize(const OptionalSize& size);

  private:
    OptionalString m_name;
    Commons::Range<OptionalDate> m_created;
    Commons::Range<OptionalDate> m_modified;
    Commons::Range<OptionalSize> m_size;
};

typedef DPL::SharedPtr<NodeFilter> NodeFilterPtr;
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_NODEFILTER_H_
