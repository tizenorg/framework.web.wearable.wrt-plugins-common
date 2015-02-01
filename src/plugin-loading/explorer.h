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
 * @file    explorer.h
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */

#ifndef WRT_SRC_PLUGIN_SERVICE_EXPLORER_H_
#define WRT_SRC_PLUGIN_SERVICE_EXPLORER_H_

#include <string>

#include <dpl/noncopyable.h>
#include <dpl/exception.h>
#include <dpl/shared_ptr.h>

#include <Commons/JSObject.h>
#include <Commons/JSObjectDeclaration.h>
//JSGlobalContextRef
#include "javascript_interface.h"
#include "plugin_iframe_support.h"
#include "plugin_property_support.h"

//JS overlay
#include <js_overlay_types.h>

using namespace PluginModule;
using namespace WrtPlugins::W3C;

class Explorer : private DPL::Noncopyable
{
  public:
    explicit Explorer(JSGlobalContextRef context);
    ~Explorer();

    //register to global frame
    bool registerObject(const JSObjectDeclarationPtr& declaration,
                        JSGlobalContextRef context);

    bool registerObject (const JSObjectDeclarationPtr& declaration,
                         const std::string& parentObjectName,
                         JSObjectPtr parentObject,
                         JSGlobalContextRef context);

    void deregisterObject(const JSObjectDeclarationPtr& declaration);

    // new load/unload frame API
    void loadFrame(JSGlobalContextRef context);
    void unloadFrame(JSGlobalContextRef context);

    void removePluginsFromIframes();
    void cleanIframesData();
    void callEventListeners(CustomEventType eventType, void* data);

    WindowPropertySupport* getWindowPropertySupport();

  private:

    IframesSupport m_iframeSupport;

    //globalContext
    JSGlobalContextRef m_context;

    //global object of main frame
    JSObjectPtr m_globalObject;

    std::unique_ptr<WindowPropertySupport> m_propertySupport;

  private:
    void registerObjectIntoIframe(const JSObjectDeclarationPtr& declaration,
                                  JSObjectPtr parentObject,
                                  JSGlobalContextRef context);

    bool register_(const JSObjectDeclarationPtr& declaration,
                   JSObjectPtr parent,
                   JSGlobalContextRef context);

    JSObjectPtr getJSObjectProperty(const std::string& propertyName,
                                    JSObjectPtr object);

    JSObjectPtr getProperty(
        const std::string& requestedProperty,
        JSObjectPtr providedObject);
};

typedef DPL::SharedPtr<Explorer> ExplorerPtr;
#endif
