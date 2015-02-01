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

#ifndef WRT_PLUGINS_WRT_ACCESS_H_
#define WRT_PLUGINS_WRT_ACCESS_H_

#include <string>

#include <dpl/singleton.h>
#include <Commons/TypesDeclaration.h>

namespace WrtDeviceApis {
namespace Commons {
class WrtAccess
{
 public:
    enum CheckAccessReturnType
    {
        CHECK_ACCESS_GRANTED,
        CHECK_ACCESS_PRIVILEGE_DENIED,
        CHECK_ACCESS_PRIVACY_DENIED,
        CHECK_ACCESS_INTERNAL_ERROR
    };

    void initialize(int widgetId);
    void deinitialize(int widgetId);
    int getWidgetId() const;
    CheckAccessReturnType checkAccessControl(const AceFunction &aceFunction) const;

  private:

    typedef std::string SessionId;
    SessionId GenerateSessionId();

    WrtAccess();
    virtual ~WrtAccess();

    int m_widgetId;
    SessionId m_sessionId;
    size_t m_pluginOwners;


    friend class DPL::Singleton<WrtAccess>;
};

typedef DPL::Singleton<WrtAccess> WrtAccessSingleton;
}
} // WrtDeviceApisCommon

#endif //WRT_PLUGINS_WRT_ACCESS_H_
