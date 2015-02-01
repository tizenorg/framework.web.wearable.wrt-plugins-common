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

#ifndef WRT_PLUGINS_WRT_ENGINE_WIDGET_FEATURE_INTERFACE_H_
#define WRT_PLUGINS_WRT_ENGINE_WIDGET_FEATURE_INTERFACE_H_

#include <list>
#include <string>
#include <dpl/shared_ptr.h>

namespace WrtDeviceApis {
namespace WidgetDB {
namespace Api {
class IWidgetFeature
{
  public:

    virtual ~IWidgetFeature()
    {}

    virtual std::string getName() const = 0;

    virtual bool isRequestedByWidget() const = 0;
};

typedef DPL::SharedPtr<IWidgetFeature> IWidgetFeaturePtr;
} // Api
} // WidgetDB
} // WrtDeviceApis

#endif
