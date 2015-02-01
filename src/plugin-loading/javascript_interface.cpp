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
 * @file        plugin_webkit.h
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of webkit js plugin
 *              accessing routines in EFL
 */
#include <javascript_interface.h>
#include <dpl/log/secure_log.h>
#include <dpl/scoped_array.h>
#include <vector>
#include <dpl/singleton_impl.h>
#include <string>
#include <dpl/foreach.h>
#include <dpl/noncopyable.h>
#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/JSObjectRef.h>

IMPLEMENT_SINGLETON(JavaScriptInterface)

#define CHECK_JSVALUE_IS_UNDEFINED_RETURN(context, object, ret) \
    if (JSValueIsUndefined(context, object)) { \
        _E("Object %s is undefined", #object); \
        return ret; \
    }

#define CHECK_JSOBJECT_IS_NULL_RETURN(object, ret) \
    if (!object) { \
        _E("Object %s is NULL", #object); \
        return ret; \
    }

namespace {
/**
 * Converts JSStringRef to std::string
 * */
std::string toString(const JSStringRef& arg)
{
    Assert(arg);
    std::string result;
    size_t jsSize = JSStringGetMaximumUTF8CStringSize(arg);
    if (jsSize > 0) {
        ++jsSize;
        DPL::ScopedArray<char> buffer(new char[jsSize]);
        size_t written = JSStringGetUTF8CString(arg, buffer.Get(), jsSize);
        if (written > jsSize) {
            _E("Conversion could not be fully performed.");
            return std::string();
        }
        result = buffer.Get();
    }

    return result;
}

/**
 * Converts JSValueRef to std::string
 * */
std::string toString(JSContextRef ctx, JSValueRef value)
{
    Assert(ctx && value);
    std::string result;
    JSStringRef str = JSValueToStringCopy(ctx, value, NULL);
    result = toString(str);
    JSStringRelease(str);
    return result;
}

JSValueRef getProperty(JSContextRef ctx, JSValueRef value, const char* name)
{
    Assert(ctx && value && name);
    JSValueRef result = NULL;
    JSObjectRef obj = JSValueToObject(ctx, value, NULL);
    if (obj) {
        JSStringRef str = JSStringCreateWithUTF8CString(name);
        result = JSObjectGetProperty(ctx, obj, str, NULL);
        JSStringRelease(str);
    }
    return result;
}

JSValueRef getPropertyObj(JSContextRef ctx, JSObjectRef obj, const char* name)
{
    Assert(ctx && obj && name);
    JSStringRef str = JSStringCreateWithUTF8CString(name);
    JSValueRef result = JSObjectGetProperty(ctx, obj, str, NULL);
    JSStringRelease(str);
    return result;
}

JSObjectRef getJSGlobalObject(JSGlobalContextRef context)
{
    return JSContextGetGlobalObject(context);
}
}

typedef JSValueRef (*JSCFunction)(
    JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception);

void JavaScriptInterface::setObjectProperty(JSGlobalContextRef context,
                                            const JSObjectPtr& parentObject,
                                            const std::string &propertyName,
                                            const JSObjectPtr& propertyObject)
{
    _D("JSObjectSetProperty(%p, \"%s\")", context, propertyName.c_str());

    //create name
    JSStringRef name = JSStringCreateWithUTF8CString(propertyName.c_str());
    //set property
    JSObjectSetProperty(
        static_cast<JSGlobalContextRef>(context),
        static_cast<JSObjectRef>(parentObject->getObject()), name,
        static_cast<JSObjectRef>(propertyObject->getObject()),
        kJSPropertyAttributeReadOnly, 0);

    JSStringRelease(name);
}

void JavaScriptInterface::removeObjectProperty(JSGlobalContextRef context,
                                               const JSObjectPtr& parentObject,
                                               const std::string &propertyName)
{
    if (!context) {
        //nothing to do -> no context
        return;
    }
    _D("JSObjectDeleteProperty(%p, \"%s\")", context, propertyName.c_str());

    JSStringRef name = JSStringCreateWithUTF8CString(propertyName.c_str());
    JSObjectDeleteProperty(
        static_cast<JSGlobalContextRef>(context),
        static_cast<JSObjectRef>(parentObject->getObject()), name, 0);

    JSStringRelease(name);
}

JavaScriptInterface::PropertiesList JavaScriptInterface::
    getObjectPropertiesList(
    JSGlobalContextRef context,
    const JSObjectPtr& object) const
{
    PropertiesList result;
    JSPropertyNameArrayRef properties = JSObjectCopyPropertyNames(
            static_cast<JSGlobalContextRef>(context),
            static_cast<JSObjectRef>(object->getObject()));
    std::size_t count = JSPropertyNameArrayGetCount(properties);
    result.reserve(count);
    _D("propesties count %d", count);
    for (std::size_t i = 0; i < count; ++i) {
        JSStringRef property = JSPropertyNameArrayGetNameAtIndex(properties, i);
        result.push_back(toString(property));
    }
    JSPropertyNameArrayRelease(properties);
    return result;
}

JSObjectPtr JavaScriptInterface::makeJSFunctionObject(
    JSGlobalContextRef context,
    const std::string &name,
    js_function_impl functionImplementation) const
{
    _D("JSObjectMakeFunctionWithCallback(%p, \"%s\")", context, name.c_str());
    JSStringRef jsFunName = JSStringCreateWithUTF8CString(name.c_str());

    JSObjectRef object = JSObjectMakeFunctionWithCallback(
            context,
            jsFunName,
            reinterpret_cast<JSObjectCallAsFunctionCallback>(
                functionImplementation));

    JSStringRelease(jsFunName);
    return JSObjectPtr(new JSObject(static_cast<void*>(object)));
}

JSObjectPtr JavaScriptInterface::makeJSClassObject(
    JSGlobalContextRef context,
    JSObjectDeclaration::ConstClassTemplate classTemplate) const
{
    _D("JSObjectMake(%p)", context);
    JSObjectRef object = JSObjectMake(
            context,
            static_cast<JSClassRef>(
                const_cast<JSObjectDeclaration::ClassTemplate>(classTemplate)),
            NULL);
    return JSObjectPtr(new JSObject(object));
}

JSObjectPtr JavaScriptInterface::makeJSObjectBasedOnInterface(
    JSGlobalContextRef context,
    const std::string &interfaceName) const
{
    _D("makeJSObjectBasedOnInterface(%p, \"%s\")", context, interfaceName.c_str());
    JSObjectPtr jsInterfaceObj = getJSObjectProperty(context,
                                                     getGlobalObject(
                                                         context),
                                                     interfaceName);
    JSObjectRef object = JSObjectCallAsConstructor(context,
                                                   static_cast<JSObjectRef>(
                                                       jsInterfaceObj->
                                                           getObject()), 0,
                                                   NULL,
                                                   NULL);
    return JSObjectPtr(new JSObject(static_cast<void*>(object)));
}

JSObjectPtr JavaScriptInterface::makeJSInterface(
    JSGlobalContextRef context,
    JSObjectDeclaration::ConstClassTemplate classTemplate,
    JSObjectDeclaration::ConstructorCallback constructorCallback) const
{
    _D("makeJSInterface(%p)", context);
    JSObjectRef object = JSObjectMakeConstructor(context,
                                                 static_cast<JSClassRef>(
                                                     const_cast<
                                                         JSObjectDeclaration::
                                                             ClassTemplate>(
                                                         classTemplate)),
                                                 reinterpret_cast<
                                                     JSObjectCallAsConstructorCallback>(
                                                     constructorCallback)
                                                 );
    return JSObjectPtr(new JSObject(static_cast<void*>(object)));
}

JSObjectPtr JavaScriptInterface::createObject(
    JSGlobalContextRef context,
    const JSObjectDeclarationPtr&
    declaration)
{
    typedef JSObjectDeclaration::Options JO;

    if (declaration->getOptions()) {
        switch (declaration->getOptions()->getType()) {
        case JO::ClassType::Function:
            return makeJSFunctionObject(
                       context,
                       declaration->getName(),
                       declaration->getOptions()->getFunctionImpl());

        case JO::ClassType::Class:
            if (declaration->getInterfaceName().empty()) {
                return makeJSClassObject(
                           context,
                           declaration->getClassTemplate());
            } else {
                return makeJSObjectBasedOnInterface(
                           context,
                           declaration->getInterfaceName());
            }

        case JO::ClassType::Interface:
            return makeJSInterface(
                       context,
                       /* product class template */
                       declaration->getClassTemplate(),
                       declaration->getConstructorCallback());

        default:
            _E("Invalid class type. Making empty JS object.");
            return JSObjectPtr(new JSObject(
                                   JSObjectMake(context, NULL, NULL)));
        }
    } else {
        _D("No declaration options");
        return JSObjectPtr(new JSObject(
                               JSObjectMake(
                                   context,
                                   static_cast<JSClassRef>(
                                       const_cast<JSObjectDeclaration::
                                                      ClassTemplate>(
                                           declaration->getClassTemplate())),
                                   NULL)));
    }
}

JSObjectPtr JavaScriptInterface::getGlobalObject(JSGlobalContextRef context)
const
{
    return JSObjectPtr(new JSObject(static_cast<JSObject::RealObject>(
                                        JSContextGetGlobalObject(static_cast<
                                                                     JSGlobalContextRef>(
                                                                     context)))));
}

JSObjectPtr JavaScriptInterface::copyObjectToIframe(
    JSGlobalContextRef context,
    const JSObjectPtr& iframe,
    const std::string& name)
{
    _D("copyObjectToIframe(%s)", name.c_str());

    JSGlobalContextRef jsGlobalContext =
        static_cast<JSGlobalContextRef>(context);

    JSObjectRef globalObject = JSContextGetGlobalObject(jsGlobalContext);

    JSValueRef requestedObject = getPropertyObj(jsGlobalContext,
                                                globalObject,
                                                name.c_str());
    CHECK_JSVALUE_IS_UNDEFINED_RETURN(jsGlobalContext,
                                      requestedObject,
                                      JSObjectPtr());

    JSStringRef requestedObjectStr =
        JSStringCreateWithUTF8CString(name.c_str());

    JSObjectSetProperty(jsGlobalContext,
                        static_cast<JSObjectRef>(iframe->getObject()),
                        requestedObjectStr,
                        requestedObject,
                        kJSPropertyAttributeReadOnly,
                        NULL);

    JSStringRelease(requestedObjectStr);

    return JSObjectPtr(
               new JSObject(const_cast<OpaqueJSValue*>(requestedObject)));
}

JavaScriptInterface::ObjectsListPtr
JavaScriptInterface::getIframesList(JSGlobalContextRef ctx) const
{
    JSGlobalContextRef context = static_cast<JSGlobalContextRef>(ctx);
    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    ObjectsListPtr retList = getIframesList(context, globalObject);

    return retList;
}

JavaScriptInterface::ObjectsListPtr
JavaScriptInterface::getIframesList(JSContextRef context,
                                    JSObjectRef globalObject) const
{
    JSValueRef frames = getPropertyObj(context, globalObject, "frames");
    CHECK_JSVALUE_IS_UNDEFINED_RETURN(context, frames, ObjectsListPtr());

    JSObjectRef frames_o = JSValueToObject(context, frames, NULL);
    CHECK_JSOBJECT_IS_NULL_RETURN(frames_o, ObjectsListPtr());

    JSValueRef len = getPropertyObj(context, frames_o, "length");
    CHECK_JSVALUE_IS_UNDEFINED_RETURN(context, len, ObjectsListPtr());

    size_t count = JSValueToNumber(context, len, NULL);
    _D("frames_o.length = %%d", count);

    ObjectsListPtr retList = ObjectsListPtr(new ObjectsList());

    for (size_t i = 0; i < count; ++i) {
        std::stringstream ss;
        ss << i;
        JSValueRef frame = getPropertyObj(context,
                                          frames_o,
                                          ss.str().c_str());
        if (JSValueIsUndefined(context, frame)) {
            _E("Selected frame is null: frame[%d]", i);
            continue;
        }
        JSObjectRef frame_obj = JSValueToObject(context, frame, NULL);
        if (!frame_obj) {
            _E("frame_obj is NULL.");
            continue;
        }
        retList->push_back(JSObjectPtr(new JSObject(frame_obj)));
        ObjectsListPtr childList = getIframesList(context, frame_obj);

        retList->merge(*childList);
        _D("Frame Value Pointer: %p", frame);
        _D("Frame Object Pointer: %p", frame_obj);
    }

    return retList;
}

JSObjectPtr JavaScriptInterface::getJSObjectProperty(JSGlobalContextRef ctx,
                                                     const JSObjectPtr& frame,
                                                     const std::string& name)
const
{
    _D("makeJSObjectBasedOnInterface(%p, \"%s\")", ctx, name.c_str());
    JSObjectRef frame_js = static_cast<JSObjectRef>(frame->getObject());
    JSValueRef property = getPropertyObj(ctx, frame_js, name.c_str());
    JSObjectRef objProp = JSValueToObject(ctx, property, NULL);

    return JSObjectPtr(new JSObject(objProp));
}

void JavaScriptInterface::removeIframes(JSGlobalContextRef context)
{
    const char* deleteIframesScript =
        "frame_set = document.getElementsByTagName('iframe');"
        "len = frame_set.length;"
        "for(i=0; i< len; i++)"
        "   frame_set[0].parentNode.removeChild(frame_set[0]); ";

    JSGlobalContextRef ctx = static_cast<JSGlobalContextRef>(context);

    JSStringRef script_src = JSStringCreateWithUTF8CString(deleteIframesScript);

    JSEvaluateScript(ctx, script_src, 0, 0, 0, 0);

    JSStringRelease(script_src);
}

void JavaScriptInterface::invokeGarbageCollector(JSGlobalContextRef context)
{
    LogDebug("Garbage collection");
    JSGarbageCollect(context);
    JSGarbageCollect(context);
    JSGarbageCollect(context);
    JSGarbageCollect(context);
}
