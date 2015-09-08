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
 * @file        js_overlay_support.cpp
 * @author
 * @version     1.0
 * @brief
 */
#include <memory>
#include <dpl/log/log.h>
#include <dpl/foreach.h>
#include <JavaScriptCore/JavaScript.h>

#include <js_overlay_support.h>

namespace JSOverlaySupport {
class JSFunctionDispatcher
{
  public:
    struct PrivateData
    {
        //required to unprotect object in destructor;
        JSContextRef context;
        JSObjectPtr originalFunction;
        JSObjectPtr overlayFunction;
    };

    static JSClassRef getClassRef();

  private:
    static JSClassDefinition m_classInfo;

  private:
    static void initialize(JSContextRef context, JSObjectRef object);

    static void finalize(JSObjectRef object);

    static JSValueRef callAsFunction(JSContextRef ctx,
                                     JSObjectRef function,
                                     JSObjectRef thisObject,
                                     size_t argumentCount,
                                     const JSValueRef arguments[],
                                     JSValueRef* exception);
};

JSObjectPtr createWrappedFunction(
    JSGlobalContextRef ctx,
    const JSObjectPtr& originalFunction,
    const JSObjectPtr& overlayFunction,
    const std::string& name)
{
    LogDebug("Creation overlay for function: " << name);
    JSStringRef name_js = JSStringCreateWithUTF8CString(name.c_str());

    JSFunctionDispatcher::PrivateData* priv =
        new JSFunctionDispatcher::PrivateData;
    priv->context = ctx;
    priv->overlayFunction = overlayFunction;
    priv->originalFunction = originalFunction;

    auto fun = JSObjectMake(ctx, JSFunctionDispatcher::getClassRef(), priv);

    JSStringRelease(name_js);

    LogDebug("JSValueProtect invoked for: "
             << overlayFunction->getObject() << " "
             << originalFunction->getObject() << " Wrapper:"
             << fun);

    //the value is unprotected in finalize of the JSFunctionDispatcher object
    JSValueProtect(ctx,
                   static_cast<JSObjectRef>(
                       overlayFunction->getObject()));

    return JSObjectPtr(new JSObject(fun));
}

JSClassDefinition JSFunctionDispatcher::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "IGNORED",
    0,
    0,
    0,
    initialize,
    finalize,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    callAsFunction,
    NULL,
    NULL,
    NULL
};

JSClassRef JSFunctionDispatcher::getClassRef()
{
    static auto classRef = JSClassCreate(&m_classInfo);
    return classRef;
}

void JSFunctionDispatcher::initialize(JSContextRef /*context*/,
                                      JSObjectRef /*object*/)
{
    LogDebug("Initialize");
}

void JSFunctionDispatcher::finalize(JSObjectRef object)
{
    LogDebug("finalize");

    PrivateData* priv = static_cast<PrivateData*>(JSObjectGetPrivate(object));
    if (priv) {
        JSValueUnprotect(priv->context,
                         static_cast<JSObjectRef>(
                             priv->overlayFunction->getObject()));
        delete priv;
    }
}

JSValueRef JSFunctionDispatcher::callAsFunction(
    JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LogDebug("Dispatcher invoked");

    JSValueRef result = JSValueMakeUndefined(context);

    JSFunctionDispatcher::PrivateData* priv =
        static_cast<JSFunctionDispatcher::PrivateData*>
        (JSObjectGetPrivate(object));

    if (!priv) {
        LogError("Private object is NULL");
        return result;
    }

    //call overlayed function
    if (priv->overlayFunction->getObject()) {
        LogDebug("Overlayed function will be invoked...");
        result = JSObjectCallAsFunction(
                priv->context,
                static_cast<JSObjectRef>(
                    priv->overlayFunction->getObject()),
                thisObject,
                argumentCount,
                arguments,
                exception);
    }

    //call original function
    if (priv->originalFunction->getObject()) {
        LogDebug("Original function will be invoked..");
        result = JSObjectCallAsFunction(
                context,
                static_cast<JSObjectRef>(
                    priv->originalFunction->getObject()),
                thisObject,
                argumentCount,
                arguments,
                exception);
    }

    LogDebug("Done");
    return result;
}
}
