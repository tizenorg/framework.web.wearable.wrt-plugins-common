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
/*
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version     0.1
 * @brief
 */

#ifndef WRTDEVICEAPIS_COMMONS_IEXTERN_EVENT_CANCELER_H_
#define WRTDEVICEAPIS_COMMONS_IEXTERN_EVENT_CANCELER_H_

#include <memory>

#include <dpl/shared_ptr.h>
#include <dpl/log/log.h>

namespace WrtDeviceApis {
namespace Commons {
/**
 * This class is related to JSPendingOperation object.
 *
 * It is interface for controllers which want to perform
 * an additional accation when the PendingOberation object
 * is canceled.
 * */
template<class TemplateEvent>
class IExternEventCanceler
{
  public:
    virtual void OnCancelEvent(const std::shared_ptr<TemplateEvent>& event)
    { LogError("Pure virtual function call"); }

    virtual void OnCancelEvent(const DPL::SharedPtr<TemplateEvent>& event)
    { LogError("Pure virtual function call"); }

    virtual ~IExternEventCanceler()
    {}
};
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_IEXTERN_EVENT_CANCELER_H_
