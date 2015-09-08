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
 * @file        PluginManager.h
 * @author      Lukasz Marek (l.marek@samsung.com)
 * @version     0.1
 * @brief
 */

#ifndef WRTDEVICEAPIS_PLUGINMANAGER_PLUGINMANAGER_H_
#define WRTDEVICEAPIS_PLUGINMANAGER_PLUGINMANAGER_H_

#include <map>
#include <string>
#include <API/PluginManager/IPluginManager.h>
#include <boost/optional.hpp>

namespace WrtDeviceApis {
namespace PluginManager {
class PluginManager : public Api::IPluginManager
{
  public:
    PluginManager(int widgetHandle,
                  const std::string &objectUri,
                  JSObjectRef object,
                  JSContextRef context);
    ~PluginManager();
    bool hasChild(const std::string &name) const;
    bool loadChild(const std::string &name) const;

    JSValueRef getProperty(const std::string &name) const;
    bool setProperty(const std::string &name,
                     JSValueRef value);
    bool deleteProperty(const std::string &name);

    PropertyList getProperties() const;
    void addPropertiesToList(JSPropertyNameAccumulatorRef propertyNames) const;

  private:
    typedef std::map<std::string, JSValueRef> ObjectList;

    int m_widgetHandle;
    JSObjectRef m_objectInstance;
    std::string m_objectUri;
    std::string m_shortUri;
    mutable boost::optional<PropertyList> m_propertyCache;
    ObjectList m_objectList;
    JSContextRef m_context;
};
}
}

#endif
