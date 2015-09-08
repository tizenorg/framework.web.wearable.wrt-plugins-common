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
#ifndef WRTDEVICEAPIS_FILESYSTEM_EVENTREMOVE_H_
#define WRTDEVICEAPIS_FILESYSTEM_EVENTREMOVE_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "INode.h"
#include "Enums.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class EventRemove : public Commons::IEvent<EventRemove>
{
  public:
    /**
     * Creates new event object.
     * @param path Path to the node to remove.
     * @return New object.
     */
    explicit EventRemove(const IPathPtr& path);

    /**
     * Gets path to the node to remove.
     * @return Path.
     */
    IPathPtr getPath() const;

    /**
     * Gets options.
     * @return Options.
     */
    int getOptions() const;

    /**
     * Sets options.
     * @param options Options.
     */
    void setOptions(int options);

  private:
    IPathPtr m_path; ///< Source path.
    int m_options;  ///< Options for remove action @see
                    // WrtDeviceApis::Api::Filesystem::Options.
};

typedef DPL::SharedPtr<EventRemove> EventRemovePtr;
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_EVENTREMOVE_H_
