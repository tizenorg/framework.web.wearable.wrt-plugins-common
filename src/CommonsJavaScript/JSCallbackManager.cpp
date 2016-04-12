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
 * @file        JSCallbackManager.cpp
 * @author      Lukasz Marek (l.marek@samsung.com)
 * @version     0.1
 */

#include "JSCallbackManager.h"
#include <WKBundle.h>
#include <dpl/log/secure_log.h>
#include <CommonsJavaScript/JSLifeManager.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
JSCallbackManagerPtr JSCallbackManager::createObject(
    JSContextRef context,
    JSValueRef onSuccess,
    JSValueRef onError,
    bool
    acceptJSNullAsOnSuccess,
    bool acceptJSNullAsOnError)
{
    JSObjectRef l_onSuccess = NULL;
    JSObjectRef l_onError = NULL;

    if (NULL != onSuccess &&
        (!acceptJSNullAsOnSuccess || !JSValueIsNull(context, onSuccess)))
    {
        l_onSuccess = JSValueToObject(context, onSuccess, NULL);
        if (!l_onSuccess ||
            !JSObjectIsFunction(context, l_onSuccess))
        {
            ThrowMsg(Commons::InvalidArgumentException,
                     "success callback is not a function");
        }
    } else {
        //LogWarning("onSuccessCallback is NULL and is not registred");
    }
    if (NULL != onError &&
        (!acceptJSNullAsOnError || !JSValueIsNull(context, onError)))
    {
        l_onError = JSValueToObject(context, onError, NULL);
        if (!l_onError ||
            !JSObjectIsFunction(context, l_onError))
        {
            ThrowMsg(Commons::InvalidArgumentException,
                     "error callback is not a function");
        }
    } else {
        //LogWarning("onErrorCallback is NULL and is not registred");
    }
    return JSCallbackManagerPtr(new JSCallbackManager(context, l_onSuccess,
                                                      l_onError));
}

JSCallbackManager::JSCallbackManager(JSContextRef context,
                                     JSObjectRef onSuccess,
                                     JSObjectRef onError) :
    m_onSuccess(NULL),
    m_onError(NULL),
    m_context(context),
    m_object(NULL)
{
    setOnSuccess(onSuccess);
    setOnError(onError);
}

JSCallbackManager::~JSCallbackManager()
{
    if (m_onSuccess) {
        JSValueSafeUnprotect(m_context, m_onSuccess);
    }

    if (m_onError) {
        JSValueSafeUnprotect(m_context, m_onError);
    }

    if (m_object) {
        JSValueSafeUnprotect(m_context, m_object);
    }
}

void JSCallbackManager::setOnSuccess(JSValueRef onSuccess)
{
    if (m_onSuccess != NULL) {
        JSValueSafeUnprotect(m_context, m_onSuccess);
    }

    if (onSuccess) {
        m_onSuccess = JSValueToObject(m_context, onSuccess, NULL);
    } else {
        m_onSuccess = NULL;
    }

    if (m_onSuccess != NULL) {
        JSValueSafeProtect(m_context, m_onSuccess);
    }
}

JSValueRef JSCallbackManager::getOnSuccess() const
{
    return m_onSuccess;
}

void JSCallbackManager::setOnError(JSValueRef onError)
{
    if (m_onError != NULL) {
        JSValueSafeUnprotect(m_context, m_onError);
    }

    if (onError) {
        m_onError = JSValueToObject(m_context, onError, NULL);
    } else {
        m_onError = NULL;
    }

    if (m_onError != NULL) {
        JSValueSafeProtect(m_context, m_onError);
    }
}

JSValueRef JSCallbackManager::getOnError() const
{
    return m_onError;
}

void JSCallbackManager::setObject(JSObjectRef object)
{
    if (m_object != NULL) {
        JSValueSafeUnprotect(m_context, m_object);
    }

    m_object = object;

    if (m_object != NULL) {
        JSValueSafeProtect(m_context, m_object);
    }
}

JSObjectRef JSCallbackManager::getObject() const
{
    return m_object;
}

void JSCallbackManager::setContext(JSContextRef context)
{
    m_context = context;
}

void JSCallbackManager::callOnSuccess()
{
    if (m_onSuccess == NULL) {
        //LogDebug("Success callback is not set");
        return;
    }
    makeCallback(m_context, NULL, m_onSuccess, NULL, 0);
}

void JSCallbackManager::callOnSuccess(JSValueRef obj)
{
    if (m_onSuccess == NULL) {
        //LogDebug("Success callback is not set");
        return;
    }
    JSValueRef objParam[1] = { obj };
    makeCallback(m_context, NULL, m_onSuccess, objParam, 1);
}

void JSCallbackManager::callOnSuccess(JSValueRef obj[],
                                      int paramCount)
{
    if (m_onSuccess == NULL) {
        //LogDebug("Success callback is not set");
        return;
    }
    makeCallback(m_context, NULL, m_onSuccess, obj, paramCount);
}

void JSCallbackManager::callOnError()
{
    if (m_onError == NULL) {
        //LogDebug("Error callback is not set");
        return;
    }
    makeCallback(m_context, NULL, m_onError, NULL, 0);
}

void JSCallbackManager::callOnError(JSValueRef obj)
{
    if (m_onError == NULL) {
        //LogDebug("Error callback is not set");
        return;
    }
    JSValueRef objParam[1] = { obj };
    makeCallback(m_context, NULL, m_onError, objParam, 1);
}

void JSCallbackManager::callOnError(JSValueRef obj[],
                                    int paramCount)
{
    if (m_onError == NULL) {
        //LogDebug("Error callback is not set");
        return;
    }
    makeCallback(m_context, NULL, m_onError, obj, paramCount);
}

void JSCallbackManager::makeCallback(JSContextRef context,
                                     JSObjectRef object,
                                     JSObjectRef callback,
                                     JSValueRef argv[],
                                     unsigned argc)
{
    if (callback == NULL) {
        //LogError("callback is NULL");
        return;
    }

    if (JSObjectIsFunction(context, callback)) {
        JSValueRef exception = NULL;

        if (argc == 0) {
            JSObjectCallAsFunction(context, callback, object, 0, NULL, &exception);
        } else {
            JSObjectCallAsFunction(context, callback, object, argc, argv, &exception);
        }

        if (exception) {
            WKBundleReportException(context, exception);
        }
    }
}
}
}
