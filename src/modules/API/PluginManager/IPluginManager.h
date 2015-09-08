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
 * @file        IPluginManager.h
 * @author      Lukasz Marek (l.marek@samsung.com)
 * @version     0.1
 * @brief
 */

#ifndef WRTDEVICEAPIS_PLUGINMANAGER_IPLUGINMANAGER_H_
#define WRTDEVICEAPIS_PLUGINMANAGER_IPLUGINMANAGER_H_

#include <string>
#include <vector>
#include <dpl/shared_ptr.h>
#include <JavaScriptCore/JavaScript.h>

namespace WrtDeviceApis {
namespace PluginManager {
namespace Api {
class IPluginManager
{
  public:
    typedef std::vector<std::string> PropertyList;

    virtual ~IPluginManager() {}
    virtual bool hasChild(const std::string &name) const = 0;
    virtual bool loadChild(const std::string &name) const = 0;

    virtual JSValueRef getProperty(const std::string &name) const = 0;
    virtual bool setProperty(const std::string &name,
                             JSValueRef value) = 0;
    virtual bool deleteProperty(const std::string &name) = 0;

    virtual PropertyList getProperties() const = 0;
    virtual void addPropertiesToList(
        JSPropertyNameAccumulatorRef propertyNames) const = 0;
};

typedef DPL::SharedPtr<IPluginManager> IPluginManagerPtr;
}
}
}

#endif // WRTDEVICEAPIS_PLUGINMANAGER_IPLUGINMANAGER_H_
