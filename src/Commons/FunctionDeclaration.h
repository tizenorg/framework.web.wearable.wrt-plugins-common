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
#ifndef WRTDEVICEAPIS_COMMONS_FUNCTION_DECLARATION_
#define WRTDEVICEAPIS_COMMONS_FUNCTION_DECLARATION_

#include <string>
#include <algorithm>
#include <dpl/log/log.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <Commons/Exception.h>
#include <Commons/TypesDeclaration.h>
#include <Commons/TypeTraits.h>
#include <Commons/plugin_initializer_def.h>

#define ACE_DECLARE_FUNCTION(function_definition) \
    extern WrtDeviceApis::Commons::AceFunction ace_##function_definition

#define ACE_DECLARE_PARAM(param_definition) \
    extern WrtDeviceApis::Commons::AceDeviceCapParam ace_param_## \
    param_definition

class DevCapFinder
{
  public:
    explicit DevCapFinder(const std::string& devcap) : m_searchFor(devcap)
    {}
    explicit DevCapFinder(const char* devcap) : m_searchFor(devcap)
    {}
    bool operator()(const WrtDeviceApis::Commons::AceDeviceCapability& dc)
    const
    {
        return m_searchFor == dc.devCapName;
    }

  private:
    std::string m_searchFor;
};

#define ACE_ADD_DEV_CAP_PARAM(dev_caps_list, dev_cap_name, param)              \
    do {                                                                       \
        WrtDeviceApis::Commons::AceDeviceCaps::iterator devcapit =                \
            std::find_if(dev_caps_list.begin(),                                \
                         dev_caps_list.end(),                                  \
                         DevCapFinder(dev_cap_name));                          \
        if (devcapit == dev_caps_list.end())                                   \
        {                                                                      \
            ThrowMsg(                                                          \
                WrtDeviceApis::Commons::InvalidArgumentException,                 \
                "Trying to set a param that doesn't exist: " <<                \
                dev_cap_name);                                                 \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            LogDebug("Setting dev cap " << dev_cap_name << " param: " <<        \
                    param.name << " to value " << param.value);                \
            devcapit->devCapParams.push_back(param);                           \
        }                                                                      \
    } while (0)

/*
 * Macro must be run inside plugin function. Plugin function must follow this
 * declaration:
 * plugin_example_function(JScontextRef cotext, JSObjectRef function,
 * JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
 * JSValueRef *exception);
 * where context, arguments, argumentsCount, exception are fixed and could not
 * be change.
 * */

#define DECLARE_FUNCTION_GETTER(Plugin_Module)                                 \
    WrtDeviceApis::Commons::AceFunction get##Plugin_Module##FunctionData(     \
        const std::string & functionId);

#define DEFINE_FUNCTION_GETTER(Plugin_Module, FunctionMap)                     \
    WrtDeviceApis::Commons::AceFunction get##Plugin_Module##FunctionData(     \
        const std::string & functionId)                                        \
    {                                                                          \
        WrtDeviceApis::Commons::FunctionMapping::const_iterator it =              \
            FunctionMap.find(functionId);                                      \
        if (it == FunctionMap.end())                                           \
        {                                                                      \
            std::string errorMsg();                                            \
            ThrowMsg(WrtDeviceApis::Commons::InvalidArgumentException,            \
                     "Function with id " << functionId << "not found");        \
        }                                                                      \
        return it->second;                                                     \
    }

typedef WrtDeviceApis::Commons::AceFunction (&AceFunctionGetter)(const std::
                                                                     string&);

namespace WrtDeviceApis {
namespace Commons {
enum class AceSecurityStatus
{
    AccessGranted,
    AccessDenied,
    PrivacyDenied,
    InternalError
};

template <typename ... Args>
class DefaultArgsVerifier
{
  public:
    void operator()(AceFunction& aceFunction, Args && ... args) const
    {
        static_assert(
            WrtDeviceApis::Commons::AlwaysFalse<Args ...>::value,
            "Please provide a specialization for these argument types!");
    }
};

template <>
class DefaultArgsVerifier<>
{
  public:
    void operator()(AceFunction& /*aceFunction*/) const
    {}
};

template <typename FunctionGetter,
          typename ArgumentsVerifier,
          typename ... Args>
AceSecurityStatus aceCheckAccess(
    const FunctionGetter& f,
    const char* functionName,
    Args && ... args)
{
    using namespace WrtDeviceApis::Commons;

    AceFunction aceFunction = f(functionName);

    ArgumentsVerifier argsVerify;
    argsVerify(aceFunction, args ...);

    WrtAccess::CheckAccessReturnType ret =
        WrtAccessSingleton::Instance().checkAccessControl(aceFunction);

    if (ret == WrtAccess::CHECK_ACCESS_PRIVILEGE_DENIED) {
        return AceSecurityStatus::AccessDenied;
    }
    else if (ret == WrtAccess::CHECK_ACCESS_PRIVACY_DENIED) {
        return AceSecurityStatus::PrivacyDenied;
    }
    else if (ret == WrtAccess::CHECK_ACCESS_INTERNAL_ERROR) {
        return AceSecurityStatus::InternalError;
    }

    return AceSecurityStatus::AccessGranted;
}
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_FUNCTION_DECLARARION_
