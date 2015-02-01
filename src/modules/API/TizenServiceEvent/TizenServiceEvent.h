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
 * @file        TizenServiceEvent.h
 * @author      Yunchan Cho (yunchan.cho@samsung.com)
 * @version     0.1
 * @brief       Tizen Service event class
 */

#ifndef _WRT_PLUGIN_TIZEN_SERVICE_EVENT_CLASS_H_
#define _WRT_PLUGIN_TIZEN_SERVICE_EVENT_CLASS_H_

#include <string>
#include "ITizenServiceEvent.h"

namespace WrtDeviceApis {
namespace TizenServiceEvent {
namespace Api {
class TizenServiceEvent : public ITizenServiceEvent
{
  public:

    TizenServiceEvent();
    ~TizenServiceEvent();

    float getScale() const;
    std::string getBundle() const;

    void setScale(const float scale);
    void setBundle(const std::string &bundle);

  private:
    float m_scale;
    std::string m_bundle;
};
} // Api
} // TizenServiceEvent
} // WrtDeviceApis

#endif // _WRT_PLUGIN_TIZEN_SERVICE_EVENT_CLASS_H_