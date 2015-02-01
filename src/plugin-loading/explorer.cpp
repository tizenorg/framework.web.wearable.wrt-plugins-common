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
 * @file    explorer.cpp
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @author  Yunchan Cho (yunchan.cho@samgsung.com)
 * @version
 * @brief
 */

#include <algorithm>
#include <dpl/foreach.h>
#include <dpl/log/secure_log.h>
#include "explorer.h"
#include "plugin_property_support.h"

// JS overlay
#include <js_overlay_types.h>

namespace {
//const char* SEPARATOR = ".";
const std::string GLOBAL_OBJECT_NAME = "GLOBAL_OBJECT";
}

using namespace std;
using namespace PluginModule;
using namespace WrtPlugins::W3C;

Explorer::Explorer(JSGlobalContextRef context) :
    m_context(context)
{
    if (!context) {
        _W("Context is NULL");
        return;
    }

    m_globalObject =
        JavaScriptInterfaceSingleton::Instance().getGlobalObject(context);

    //register main frame;
    m_iframeSupport.registerIframe(m_globalObject);

    m_propertySupport.reset(new WindowPropertySupport(m_context));
}

Explorer::~Explorer()
{
}

JSObjectPtr Explorer::getJSObjectProperty(const std::string& propertyName,
                                          JSObjectPtr object)
{
    return JavaScriptInterfaceSingleton::Instance().getJSObjectProperty(
               m_context, object, propertyName);
}

JSObjectPtr Explorer::getProperty(
    const string& requestedProperty,
    JSObjectPtr providedObject)
{
    _D("Requested Property: %s", requestedProperty.c_str());

    if (!providedObject) {
        _W("Provided object is empty");
        return JSObjectPtr();
    }

    std::string currentObjectName;
    std::string currentPropertyRequested = requestedProperty;
    JSObjectPtr currentObject = providedObject;

    while (true) {
        auto pos = currentPropertyRequested.find(".");
        if (pos == string::npos) {
            return getJSObjectProperty(currentPropertyRequested, currentObject);
        }

        //check if requested object has child
        currentObjectName = currentPropertyRequested.substr(0, pos);

        if (currentPropertyRequested.size() <= pos + 1) {
            _W("Wrong name of requested property");
            return JSObjectPtr();
        }
        currentPropertyRequested = currentPropertyRequested.substr(pos + 1);

        currentObject = getJSObjectProperty(currentObjectName, currentObject);
        if (!currentObject) {
            _W("Failed to get property: %s", currentObjectName.c_str());
            return JSObjectPtr();
        }
    }
}

bool Explorer::registerObject(const JSObjectDeclarationPtr& declaration,
                              JSGlobalContextRef context)
{
    bool retValue;

    if (declaration->getParentName() == GLOBAL_OBJECT_NAME) {
        retValue = register_(declaration, m_globalObject, context);
    } else { //PIM support
        //it should be master object name
        string masterName = declaration->getParentName();
        auto pos = masterName.find(".");
        if (string::npos != pos) {
            _W("ParentName not allowed");
            return false;
        }
        auto masterObject = getJSObjectProperty(masterName, m_globalObject);
        if (masterObject->getObject() == 0) {
            return false;
        }
        retValue = registerObject(declaration,
                                  masterName,
                                  masterObject,
                                  context);
    }

    if (declaration->checkIframesSupported()) {
        m_iframeSupport.registerDeclaration(declaration);
        //        m_iframeSupport.invokeCallbackForMainFrame(m_context,
        //                                                   declaration,
        //                                                   newObject);
    }

    return retValue;
}

bool Explorer::registerObject(const JSObjectDeclarationPtr& declaration,
                              const std::string& providedObjectName,
                              JSObjectPtr providedObject,
                              JSGlobalContextRef context)
{
    _D("%s <- %s", providedObjectName.c_str(), declaration->getName().c_str());
    std::string fullParentName = declaration->getParentName();

    if (fullParentName == providedObjectName) {
        _D("Provided object was matched!!");
        return register_(declaration, providedObject, context);
    }

    //check if object exists in fullParentName
    auto pos = fullParentName.find(providedObjectName);
    if (pos == string::npos) {
        _W("Object: %s should be child of: %s but you provided object: %s",
           declaration->getName().c_str(),
           declaration->getParentName().c_str(),
           providedObjectName.c_str());
        return false;
    }

    if (fullParentName.size() <= pos + providedObjectName.size() + 1) {
        _W("Invalid object name");
        return false;
    }

    auto currentRequesteProperty =
        fullParentName.substr(pos + providedObjectName.size() + 1);

    JSObjectPtr jsObject = getProperty(
            currentRequesteProperty, providedObject);

    if (!jsObject->getObject()) {
        _W("Object: %s should be child of: %s. you provided object: %s but object is null",
           declaration->getName().c_str(),
           declaration->getParentName().c_str(),
           providedObjectName.c_str());
        return false;
    }

    return register_(declaration, jsObject, context);
}

bool Explorer::register_(const JSObjectDeclarationPtr& declaration,
                         JSObjectPtr parent,
                         JSGlobalContextRef context)
{
    _D("Registration object: %s <- %s",
       declaration->getParentName().c_str(),
       declaration->getName().c_str());
    AssertMsg(parent, "parent object is NULL");

    typedef JSObjectDeclaration::Options JO;

    JSGlobalContextRef gContext = (context == NULL) ? m_context : context;
    JSObjectPtr objectInstance = JavaScriptInterfaceSingleton::Instance().
            createObject(gContext, declaration);

    JavaScriptInterfaceSingleton::Instance().setObjectProperty(
        gContext,
        parent,
        declaration->getName(),
        objectInstance);

    //notice
    if (declaration->getOptions() &&
        (declaration->getOptions()->getIframeNotice() ==
         JO::IFrameNotice::AlwaysNotice))
    {
        declaration->getOptions()->invokeCallback(gContext,
                                                  parent->getObject(),
                                                  objectInstance->getObject());
    }

    return true;
}

void Explorer::registerObjectIntoIframe(
    const JSObjectDeclarationPtr& declaration,
    JSObjectPtr frameObject,
    JSGlobalContextRef context)
{
    if (declaration->getParentName() == GLOBAL_OBJECT_NAME) {
        _D("Connect to Global object of IFRAME");
        register_(declaration, frameObject, context);
        return;
    }

    //PIM SUPPORT
    {
        _D("Connect to NOT global object of IFRAME");
        //it should be master object name
        string masterName = declaration->getParentName();
        auto pos = masterName.find(".");
        if (string::npos != pos) {
            _W("ParentName not allowed");
            return;
        }
        auto masterObject = getJSObjectProperty(masterName, frameObject);
        if (!masterObject->getObject()) {
            _W("Object not exist in frame");
            return;
        }
        register_(declaration, masterObject, context);
    }
}

void Explorer::loadFrame(JSGlobalContextRef context)
{
    JSObjectPtr frameObject =
        JavaScriptInterfaceSingleton::Instance().getGlobalObject(context);

    if (frameObject->getObject() == m_globalObject->getObject()) {
        // Main page was already loaded from constructor
        _W("Main page loaded");
        return;
    }

    m_iframeSupport.registerIframe(frameObject);

    auto iframeObjects = m_iframeSupport.getIframeObjects();

    FOREACH(object, iframeObjects)
    {
        _D("Register object: %s", (*object)->getName().c_str());
        registerObjectIntoIframe(*object, frameObject, context);
    }
}

void Explorer::unloadFrame(JSGlobalContextRef context)
{
    JSObjectPtr frameObject =
        JavaScriptInterfaceSingleton::Instance().getGlobalObject(context);
    m_iframeSupport.unregisterIframe(frameObject);
}

void Explorer::removePluginsFromIframes()
{
    if (m_iframeSupport.hasIframes()) {
        JavaScriptInterfaceSingleton::Instance().removeIframes(m_context);
    }
}

void Explorer::deregisterObject(const JSObjectDeclarationPtr& declaration)
{
    if (GLOBAL_OBJECT_NAME != declaration->getParentName()) {
        _W("Ignored remove property %s", declaration->getName().c_str());
        return;
    }

    JavaScriptInterfaceSingleton::Instance().
        removeObjectProperty(m_context,
                             m_globalObject,
                             declaration->getName());
}

void Explorer::cleanIframesData()
{
    m_iframeSupport.clean();
}

void Explorer::callEventListeners(CustomEventType /*eventType*/, void* /*data*/)
{
#if 0 // deprecated
    using namespace WrtPlugins::Tizen;
    // get iframe objects from javascript global context
    JavaScriptInterface::ObjectsListPtr frameLists =
        JavaScriptInterfaceSingleton::Instance().getIframesList(m_context);

    // get main frame object from javascript global context
    JSObjectRef mainFrame = JSContextGetGlobalObject(m_context);
    frameLists->push_back(JSObjectPtr(new JSObject(mainFrame)));

    if (eventType == ServiceCustomEvent) {
        using namespace WrtDeviceApis::TizenServiceEvent::Api;
        // set user data of js callback function for 'appservice' js event
        ITizenServiceEventPtr event = ITizenServiceEventPtr(
                new TizenServiceEvent());
        event->setScale(m_propertySupport->getScale());
        event->setBundle(m_propertySupport->getBundle());

        // call js callback function for 'appservice' js event on each frame
        FOREACH(it, *frameLists)
        {
            JSObjectRef frame = static_cast<JSObjectRef>((*it).Get()->getObject());

            auto eventPriv =
                new JSTizenServiceEventPrivateObject(m_context, event);
            JSObjectRef eventObject =
                JSObjectMake(m_context,
                             JSTizenServiceEvent::getClassRef(), eventPriv);

            AddEventListenerSupport::CallCustomEventListenersFromIFrame(
                frame, eventType, eventObject);
        }
    } else if (eventType == SoftKeyboardChangeCustomEvent) {
        using namespace WrtDeviceApis::SoftKeyboardChangeEvent::Api;
        if (!data) {
            LogDebug("no ime size");
            return;
        }

        // set user data of js callback function for 'softkeyboardchange' js
        // event
        SoftKeyboardChangeArgs* args =
            static_cast<SoftKeyboardChangeArgs *>(data);
        ISoftKeyboardChangeEventPtr event =
            ISoftKeyboardChangeEventPtr(
                new SoftKeyboardChangeEvent(
                    args->state,
                    args->width,
                    args->height));
        LogDebug("softkeyboard event's state: " << args->state);

        // call js callback function for 'softkeyboardchange' js event on each
        // frame
        FOREACH(it, *frameLists)
        {
            JSObjectRef frame = static_cast<JSObjectRef>((*it).Get()->getObject());

            auto eventPriv =
                new JSSoftKeyboardChangeEventPrivateObject(m_context, event);
            JSObjectRef eventObject =
                JSObjectMake(
                    m_context,
                    JSSoftKeyboardChangeEvent::getClassRef(),
                    eventPriv);

            AddEventListenerSupport::CallCustomEventListenersFromIFrame(
                frame, eventType, eventObject);
        }
    } else {
        LogDebug("Not supported custom event type");
        return;
    }
#endif
}

WindowPropertySupport* Explorer::getWindowPropertySupport()
{
    return m_propertySupport.get();
}
