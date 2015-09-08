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
 * @file        PluginManagerFactory.h
 * @author      Lukasz Marek (l.marek@samsung.com)
 * @version     0.1
 */

#ifndef WRTDEVICEAPIS_PLUGIN_MANAGER_FACTORY_H_
#define WRTDEVICEAPIS_PLUGIN_MANAGER_FACTORY_H_

#include <string>
#include <dpl/noncopyable.h>
#include <PluginManager/IPluginManager.h>

namespace WrtDeviceApis {
namespace PluginManager {
namespace Api {
class PluginManagerFactory : private DPL::Noncopyable
{
  public:
    static PluginManagerFactory& getInstance();
    IPluginManagerPtr getPluginManager(
        int widgetHandle,
        const std::string &objectUri,
        JSObjectRef object,
        JSContextRef context) const;

  private:
    PluginManagerFactory();
};
}
}
}

#endif // WRTDEVICEAPIS_PLUGIN_MANAGER_FACTORY_H_
