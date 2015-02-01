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
#ifndef WRTDEVICEAPIS_FILESYSTEM_EVENTMOVE_H_
#define WRTDEVICEAPIS_FILESYSTEM_EVENTMOVE_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "IPath.h"
#include "INode.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class EventMove : public Commons::IEvent<EventMove>
{
  public:
    /**
     * Creates new event object.
     * @param src Source path.
     * @param dest Destination path.
     * @return New object.
     */
    EventMove(const IPathPtr& src,
              const IPathPtr& dest);

    /**
     * Gets destination path.
     * @return Destination path.
     */
    IPathPtr getDestination() const;

    /**
     * Gets source path.
     * @return Source path.
     */
    IPathPtr getSource() const;

    /**
     * Gets result node.
     * @return Result node.
     */
    INodePtr getResult() const;

    /**
     * Sets result node.
     * @param node Result node.
     */
    void setResult(const INodePtr& node);

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
    IPathPtr m_src;   ///< Source path.
    IPathPtr m_dest;  ///< Destination path.
    INodePtr m_result; ///< Result node.
    int m_options;    ///< Options for copy action @see
                      // WrtDeviceApis::Api::Filesystem::Options.
};

typedef DPL::SharedPtr<EventMove> EventMovePtr;
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_EVENTMOVE_H_
