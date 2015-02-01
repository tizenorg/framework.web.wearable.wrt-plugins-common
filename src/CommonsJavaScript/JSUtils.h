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
#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_JSUTILS_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_JS_JSUTILS_H_

#include <string>
#include <dpl/log/log.h>
#include <JavaScriptCore/JavaScript.h>
#include <Commons/Exception.h>
#include <CommonsJavaScript/PrivateObject.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
class JSUtils
{
  public:
    static bool hasProperty(JSStaticValue* properties,
                            JSStringRef name);

    /**
     * Gets a property from JSObject if exists
     * @return JSValueRef if property exists, NULL if not
     */
    static JSValueRef getJSProperty(JSContextRef context,
                                    JSValueRef jsValue,
                                    const std::string &name,
                                    JSValueRef* exception = NULL);

    static JSValueRef getJSProperty(JSContextRef context,
                                    JSObjectRef object,
                                    const std::string& name);

    static JSValueRef getJSPropertyOrUndefined(JSContextRef context,
                                               JSObjectRef object,
                                               const std::string& name);

    template<class C>
    static JSObjectRef makeObject(JSContextRef context,
                                  JSClassRef classRef,
                                  C data)
    {
        typedef typename PrivateObjectT<C>::Type Private;

        Private* priv = new Private(context, data);
        JSObjectRef object = JSObjectMake(context, classRef, priv);
        if (!object) {
            ThrowMsg(Commons::NullPointerException,
                     "Could not create JS object.");
        }

        return object;
    }

    static JSObjectRef makeObject(JSContextRef context,
                                  JSClassRef classRef)
    {
        typedef PrivateObjectT<void>::Type Private;

        Private* priv = new Private(context);
        JSObjectRef object = JSObjectMake(context, classRef, priv);
        if (!object) {
            ThrowMsg(Commons::NullPointerException,
                     "Could not create JS object.");
        }

        return object;
    }
}; // JSUtils
} // CommonsJavaScript
} // WrtDeviceApis

#endif /* WRTPLUGINS_COMMONS_JSUTILS_H_ */
