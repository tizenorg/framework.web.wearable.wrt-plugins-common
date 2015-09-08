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

#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_SECURITYEXCEPTIONS_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_SECURITYEXCEPTIONS_H_

#include <JavaScriptCore/JavaScript.h>
#include "JSDOMExceptionFactory.h"

namespace WrtDeviceApis {
namespace CommonsJavaScript {
/**
 * synchronously checks access status and throws JS Security exception if
 * necessary
 */
#define SYNC_ACCESS_STATUS_HANDLER(status, context, exception)                 \
    do {                                                                       \
        switch (status)                                                        \
        {                                                                      \
        case AceSecurityStatus::InternalError:                                 \
            return JSDOMExceptionFactory::UnknownException.make(                  \
                       context, exception);                                    \
            break;                                                             \
                                                                               \
        case AceSecurityStatus::AccessDenied:                                  \
            return JSDOMExceptionFactory::SecurityException.make(                 \
                       context, exception);                                    \
            break;                                                             \
                                                                               \
        default:                                                               \
            break;                                                             \
        } \
    } while (0)

/**
 * checks access status and returns an error through JSCallbackManager if
 * necessary
 */
#define ASYNC_CBM_ACCESS_STATUS_HANDLER(status, context, cbm)                  \
    do {                                                                       \
        switch (status)                                                        \
        {                                                                      \
        case AceSecurityStatus::InternalError:                                 \
            cbm->callOnError(JSDOMExceptionFactory::UnknownException.make(        \
                                 context));                                    \
            return JSValueMakeNull(context);                                   \
                                                                               \
        case AceSecurityStatus::AccessDenied:                                  \
            cbm->callOnError(JSDOMExceptionFactory::SecurityException.make(       \
                                 context));                                    \
            return JSValueMakeNull(context);                                   \
                                                                               \
        default:                                                               \
            break;                                                             \
        }                                                                      \
    } while (0)
}
}
#endif /*WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_SECURITYEXCEPTIONS_H_ */

