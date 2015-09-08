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
#ifndef _FUNCTION_DECLARATION_
#define _FUNCTION_DECLARATION_

#include <string>
#include <Commons/WrtAccess/WrtAccess.h>
#include <Commons/Exception.h>
#include <JavaScriptCore/JavaScript.h>
#include <Commons/TypesDeclaration.h>
#include <Commons/TypeTraits.h>
#include <dpl/log/log.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
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
    void operator()(WrtDeviceApis::Commons::AceFunction& aceFunction,
                    Args && ... args) const
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
    void operator()(WrtDeviceApis::Commons::AceFunction& /*aceFunction*/) const
    {}
};

template <typename ArgumentsVerifier,
          typename ... Args>
AceSecurityStatus aceCheckAccess2(
    WrtDeviceApis::Commons::AceFunction aceFunction,
    Args && ... args)
{
    using namespace WrtDeviceApis::Commons;

    ArgumentsVerifier argsVerify;
    argsVerify(aceFunction, args ...);

    WrtAccess::CheckAccessReturnType ret =
        WrtAccessSingleton::Instance().checkAccessControl(aceFunction);

    if (ret == WrtAccess::CHECK_ACCESS_PRIVILEGE_DENIED) {
        LogError("Function is not allowed to run - AccessDenied");
        return AceSecurityStatus::AccessDenied;
    }
    else if (ret == WrtAccess::CHECK_ACCESS_PRIVACY_DENIED) {
        LogError("Function is not allowed to run - PrivacyDenied");
        return AceSecurityStatus::PrivacyDenied;
    }
    else if (ret == WrtAccess::CHECK_ACCESS_INTERNAL_ERROR) {
        LogError("InternalError");
        return AceSecurityStatus::InternalError;
    }

    LogDebug("Function accepted!");

    return AceSecurityStatus::AccessGranted;
}

//The simplest version
AceSecurityStatus aceCheckAccessSimple(
    WrtDeviceApis::Commons::AceFunction aceFunction);
}
}

#endif // _FUNCTION_DECLARARION_
