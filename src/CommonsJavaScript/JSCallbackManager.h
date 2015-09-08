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
 * @file        JSCallbackManager.h
 * @author      Lukasz Marek (l.marel@samsung.com)
 * @version     0.1
 */

#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_CALLBACK_MANAGER_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_CALLBACK_MANAGER_H_


#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include <JavaScriptCore/JavaScript.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
class JSCallbackManager;
typedef DPL::SharedPtr<JSCallbackManager> JSCallbackManagerPtr;

class JSCallbackManager : public WrtDeviceApis::Commons::IEventPrivateData
{
  private:
    JSCallbackManager()
    {}

    /**
     *  Used to create JSCallbackManagerPtr object by factory method
     * createObject
     */
    JSCallbackManager(JSContextRef context,
                      JSObjectRef onSuccess,
                      JSObjectRef onError);

  public:

    //JSCallbackManager( JSContextRef context, JSValueRef onSuccess, JSValueRef
    // onError );

    /**
     * Create JSCallbackManagerPtr object witch checking callback parameters
     * if callbacks are NULL it was programmer intention to not use callback and
     * no exception will be rised
     * if callbacks are some object but not a function it will rise exception
     * @param[in] context - global context
     * @param[in] onSuccess - success callback (may be NULL if not needed)
     * @param[in] onError   - error callback (may be NULL if not needed)
     * @param[in] acceptJSNullAsOnSuccess - converts JSNull on onSuccess to NULL
     * when true
     * @param[in] acceptJSNullAsOnError   - converts JSNull on onError to NULL
     * when true
     * @throw Commons::InvalidArgumentException - if callback is not null and is
     * not a js function
     */
    static JSCallbackManagerPtr createObject(
        JSContextRef context,
        JSValueRef onSuccess = NULL,
        JSValueRef onError = NULL,
        bool acceptJSNullAsOnSuccess = false,
        bool acceptJSNullAsOnError = false);

    virtual ~JSCallbackManager();

    void setContext(JSContextRef context);

    JSContextRef getContext() const
    {
        return m_context;
    }

    void setOnSuccess(JSValueRef onSuccess);
    JSValueRef getOnSuccess() const;
    void setOnError(JSValueRef onError);
    JSValueRef getOnError() const;
    void setObject(JSObjectRef object);
    JSObjectRef getObject() const;

    void callOnSuccess();
    void callOnSuccess(JSValueRef obj);
    void callOnSuccess(JSValueRef obj[], int paramCount);

    void callOnError();
    void callOnError(JSValueRef obj);
    void callOnError(JSValueRef obj[], int paramCount);

  private:
    void makeCallback(JSContextRef context,
                      JSObjectRef object,
                      JSObjectRef callback,
                      JSValueRef argv[],
                      unsigned argc);

    JSObjectRef m_onSuccess;
    JSObjectRef m_onError;
    JSContextRef m_context;
    JSObjectRef m_object;
};

bool isCallback(JSContextRef ctx,
                JSValueRef value,
                JSValueRef* exception);
}
}

#endif // WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_CALLBACK_MANAGER_H_
