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

#include <memory>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <dpl/log/log.h>
#include <dpl/log/secure_log.h>
#include <dpl/scoped_array.h>
#include <dpl/scoped_resource.h>
#include <dpl/assert.h>
#include <Commons/Exception.h>
#include "WrtAccess.h"
#include <ace_api_client.h>
#include <dpl/singleton_safe_impl.h>

#include "popup-runner.h"
IMPLEMENT_SAFE_SINGLETON(WrtDeviceApis::Commons::WrtAccess)

namespace {
/**
 * Helper class - single parameter and its value
 */
struct AceParam
{
    const char *name;
    const char *value;

    AceParam() :
        name(NULL), value(NULL)
    {}

    AceParam(const char *name, const char *value) :
        name(name), value(value)
    {}
};

/**
 * Helper class - list of params for single dev cap
 */
struct AceParamList
{
    size_t count;
    AceParam* param;
    AceParamList() :
        count(0),
        param(NULL)
    {}
};

struct DeviceCapParamPolicy
{
    typedef AceParamList* Type;
    static Type NullValue()
    {
        return NULL;
    }
    static void Destroy(Type ptr)
    {
        if (ptr) {
            delete[] ptr->param;
        }
        delete[] ptr;
    }
};

/**
 * Helper class - modified ScopedArray for ace_param_list_t
 */
class ScopedDeviceCapArray : public DPL::ScopedResource<DeviceCapParamPolicy>
{
  public:
    explicit ScopedDeviceCapArray(AceParamList *ptr =
                                      DeviceCapParamPolicy::NullValue()) :
        DPL::ScopedResource<DeviceCapParamPolicy>(ptr)
    {}

    AceParamList & operator [](std::ptrdiff_t k) const
    {
        AssertMsg(this->m_value != DeviceCapParamPolicy::NullValue(),
               "Dereference of scoped NULL array!");
        AssertMsg(k >= 0, "Negative array index");

        return this->m_value[k];
    }
};
} // namespace

namespace WrtDeviceApis {
namespace Commons {
WrtAccess::WrtAccess() :
    m_sessionId(GenerateSessionId()),
    m_pluginOwners(0)
{}

WrtAccess::~WrtAccess()
{}

WrtAccess::SessionId WrtAccess::GenerateSessionId()
{
    const size_t SESSION_ID_LENGTH = 32;

    std::ostringstream pid;
    pid << static_cast<int>(getpid());

    std::string session_id = pid.str();

    session_id.reserve(session_id.length() + SESSION_ID_LENGTH);

    for (size_t i = 0; i < SESSION_ID_LENGTH; ++i) {
        int c = random() % 16;

        session_id += (c < 10 ?
                       static_cast<char>('0' + c) :
                       static_cast<char>('A' + c - 10));
    }
    return session_id;
}

void WrtAccess::initialize(int widgetId)
{
    _D("initialize");
    if (widgetId < 0) {
        _E("Invalid widget id");
        Throw(Exception);
    }

    m_widgetId = widgetId;

    if (!m_pluginOwners++) {
        DPL::Log::LogSystemSingleton::Instance().SetTag("SECURITY_DAEMON");
        ace_return_t ret = ace_client_initialize(Wrt::Popup::run_popup);
        DPL::Log::LogSystemSingleton::Instance().SetTag("WRT_PLUGINS");
        Assert(ACE_OK == ret);
    }
}

void WrtAccess::deinitialize(int /*widgetId*/)
{
    _D("deinitialize");

    if (!--m_pluginOwners) {
        DPL::Log::LogSystemSingleton::Instance().SetTag("SECURITY_DAEMON");
        ace_return_t ret = ace_client_shutdown();
        DPL::Log::LogSystemSingleton::Instance().SetTag("WRT_PLUGINS");
        Assert(ACE_OK == ret);
    }
}

int WrtAccess::getWidgetId() const
{
    return m_widgetId;
}

WrtAccess::CheckAccessReturnType WrtAccess::checkAccessControl(const AceFunction& aceFunction) const
{
    AssertMsg(
        m_pluginOwners, "WrtAccessSingleton needs to be initialized with"
                        "WidgetId during on_widget_start_callback in each plugin");
    size_t deviceCount = aceFunction.deviceCapabilities.size();

    DPL::ScopedArray <const char *> deviceScopedArray;
    ScopedDeviceCapArray paramsScopedArray;

    if (deviceCount) {
        deviceScopedArray.Reset(new const char*[deviceCount]);
        paramsScopedArray.Reset(new AceParamList[deviceCount]);

        for (size_t i = 0; i < deviceCount; ++i) {
            deviceScopedArray[i] =
                aceFunction.deviceCapabilities.at(i).devCapName.c_str();
            paramsScopedArray[i].count =
                aceFunction.deviceCapabilities.at(i).devCapParams.size();

            paramsScopedArray[i].param =
                new AceParam[paramsScopedArray[i].count];

            for (size_t j = 0; j < paramsScopedArray[i].count; ++j) {
                paramsScopedArray[i].param[j].name =
                    aceFunction.deviceCapabilities.at(i).
                        devCapParams[j].name.c_str();
                paramsScopedArray[i].param[j].value =
                    aceFunction.deviceCapabilities.at(i).
                        devCapParams[j].value.c_str();
            }
        }
    }

    size_t featuresCount = aceFunction.features.size();

    DPL::ScopedArray <const char*> featureScopedArray;
    if (featuresCount) {
        featureScopedArray.Reset(new const char*[featuresCount]);

        for (size_t i = 0; i < featuresCount; ++i) {
            featureScopedArray[i] =
                aceFunction.features.at(i).name.c_str();
        }
    }

    _D("constructing ACE request");

    ace_request_t aceRequest;
    aceRequest.session_id =
        const_cast<const ace_session_id_t>(m_sessionId.c_str());
    aceRequest.widget_handle = getWidgetId();
    aceRequest.feature_list.count = featuresCount;
    aceRequest.feature_list.items =
        const_cast<ace_string_t*>(featureScopedArray.Get());
    aceRequest.dev_cap_list.count = deviceCount;
    aceRequest.dev_cap_list.items = new ace_dev_cap_t[deviceCount];

    const char**  devCapNames = deviceScopedArray.Get();
    AceParamList* paramList = paramsScopedArray.Get();

    unsigned int i;
    for (i = 0; i < deviceCount; ++i) {
        aceRequest.dev_cap_list.items[i].name =
            const_cast<const ace_string_t>(devCapNames[i]);
        aceRequest.dev_cap_list.items[i].param_list.count = paramList[i].count;
        aceRequest.dev_cap_list.items[i].param_list.items =
            new ace_param_t[paramList[i].count];
        unsigned int j;
        for (j = 0; j < paramList[i].count; ++j) {
            aceRequest.dev_cap_list.items[i].param_list.items[j].name =
                const_cast<ace_string_t>(paramList[i].param[j].name);
            aceRequest.dev_cap_list.items[i].param_list.items[j].value =
                const_cast<ace_string_t>(paramList[i].param[j].value);
        }
    }

    ace_check_result_t aceCheckResult = ACE_PRIVILEGE_DENIED;
    DPL::Log::LogSystemSingleton::Instance().SetTag("SECURITY_DAEMON");
    ace_return_t ret = ace_check_access_ex(&aceRequest, &aceCheckResult);
    DPL::Log::LogSystemSingleton::Instance().SetTag("WRT_PLUGINS");
    for (i = 0; i < deviceCount; ++i) {
        delete[] aceRequest.dev_cap_list.items[i].param_list.items;
    }
    delete[] aceRequest.dev_cap_list.items;

    if (ACE_OK != ret) {
        _E("Error in ace check: %d", static_cast<int>(ret));
        return CHECK_ACCESS_INTERNAL_ERROR;
    }

    if (aceCheckResult == ACE_ACCESS_GRANTED) {
        return CHECK_ACCESS_GRANTED;
    }
    else if (aceCheckResult == ACE_PRIVILEGE_DENIED) {
        return CHECK_ACCESS_PRIVILEGE_DENIED;
    }
    else if (aceCheckResult == ACE_PRIVACY_DENIED) {
        return CHECK_ACCESS_PRIVACY_DENIED;
    }

    return CHECK_ACCESS_INTERNAL_ERROR;
}
}
} // WrtDeviceApisCommon
