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
 *
 * @file        ITizenServiceEvent.h
 * @author      Yunchan Cho (yunchan.cho@samsung.com)
 * @version     0.1
 * @brief       Tizen Service event interfece
 */

#ifndef _WRT_PLUGIN_TIZEN_SERVICE_EVENT_INTERFACE_H_
#define _WRT_PLUGIN_TIZEN_SERVICE_EVENT_INTERFACE_H_

#include <string>
#include <dpl/shared_ptr.h>

namespace WrtDeviceApis {
namespace TizenServiceEvent {
namespace Api {
class ITizenServiceEvent
{
  public:
    virtual float getScale() const = 0;
    virtual std::string getBundle() const = 0;

    virtual void setScale(const float scale) = 0;
    virtual void setBundle(const std::string &bundle) = 0;
    virtual ~ITizenServiceEvent() {}
};

typedef DPL::SharedPtr<ITizenServiceEvent> ITizenServiceEventPtr;
} // Api
} // TizenServiceEvent
} // WrtDeviceApis

#endif // _WRT_PLUGIN_TIZEN_SERVICE_EVENT_INTERFACE_H_
