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
#ifndef WRTDEVICEAPIS_FILESYSTEM_EVENTOPEN_H_
#define WRTDEVICEAPIS_FILESYSTEM_EVENTOPEN_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "IStream.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class EventOpen : public Commons::IEvent<EventOpen>
{
  public:
    /**
     * Creates new event object.
     * @param mode Mode to open the node in.
     * @return New object.
     */
    explicit EventOpen(int mode);

    /**
     * Gets mode.
     * @return Mode.
     */
    int getMode() const;

    /**
     * Gets result stream.
     * @return Result stream.
     */
    IStreamPtr getResult() const;

    /**
     * Sets result stream.
     * @param stream Result stream.
     */
    void setResult(const IStreamPtr& stream);

  private:
    int m_mode;         ///< Mode.
    IStreamPtr m_stream; ///< Opened stream.
};

typedef DPL::SharedPtr<EventOpen> EventOpenPtr;
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_EVENTOPEN_H_
