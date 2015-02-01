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
 * @file        plugin_logic.cpp
 * @author      Piotr Fatyga (p.fatyga@samsung.com)
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @author      Przemyslaw Dobrowolski (p.dobrowolsk@samsung.com)
 * @version     1.0
 * @brief       This file is the implementation file of plugin and
 *              feature loading routines
 * @brief       This code is intended to work behind view controller
 */

#include "plugin_logic.h"

#include <dpl/assert.h>
#include <dpl/scoped_array.h>
#include <dpl/log/secure_log.h>
#include <dpl/foreach.h>
#include <dpl/singleton_impl.h>
#include <dpl/wrt-dao-ro/widget_dao_read_only.h>
#include <dpl/wrt-dao-ro/common_dao_types.h>
#include <dpl/wrt-dao-ro/global_config.h>

#include <JavaScriptCore/JavaScript.h>

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <cstring>

#include <wrt_plugin_export.h>
#include <js_overlay_types.h>

#include "explorer.h"
#include "plugin.h"
#include "plugin_model.h"
#include "javascript_interface.h"
#include "js_function_manager.h"
#include "plugin_container_support.h"

#include "js_page_session.h"

using namespace std;
using namespace WrtDB;
using namespace WrtPlugins::W3C;

namespace {
const char *LIBRARY_PATH_SEPARATOR = "/";
}

class JSPageSession::Impl
{
  private:
    typedef std::set<PluginPtr> LoadedPlugins;
    typedef std::set<JSGlobalContextRef> LoadedContexts;

  private:
    ///< Widget handle using this session
    int m_widgetHandle;

    //view for this session
    JSGlobalContextRef m_context;

    bool m_sessionStarted;

    ///< JS object explorer for this session
    Explorer* m_objectExplorer;

    PluginContainerSupportPtr m_pluginsSupport;

    ///< All loaded plugins. Each one must be unloaded. Plugins means
    //set of features connected with such plugin (library)
    LoadedPlugins m_loadedPlugins;

    // Set of currently loaded web pages' contexts. These contexts are
    // exactly matching with each frames' global context.
    LoadedContexts m_loadedContexts;

  private:
    PluginPtr loadLibrary(PluginModelPtr& pluginModel);

    void installStandardFunctions();
    void installRootPlugins();
    void installRequestedFeatures();

    //returns true if success or false if failed
    bool installPlugin(PluginModelPtr plugin);
    bool installPluginOnDemand(PluginModelPtr plugin,
                               JavaScriptObject& parentObject,
                               JSGlobalContextRef context);

    void unloadPluginsFromSession();

  public:
    Impl(const PluginContainerSupportPtr& containerSupport);
    ~Impl();

    // Widget session
    void startSession(int widgetHandle,
                      JSGlobalContextRef view,
                      double scaleFactor,
                      const char* encodedBundle,
                      const char* theme);

    void stopSession();

    void performLibrariesUnload();

    bool loadPluginOnDemand(const WrtDB::DbPluginHandle &pluginHandle,
                            JavaScriptObject& parentObject,
                            JSGlobalContextRef context);

    void loadFrame(JSGlobalContextRef context);
    void unloadFrame(JSGlobalContextRef context);

    void setCustomProperties(double scaleFactor,
                             const char* encodedBundle,
                             const char* theme);

    void dispatchJavaScriptEvent(CustomEventType eventType, void* data);
};

JSPageSession::Impl::Impl(const PluginContainerSupportPtr& support) :
        m_widgetHandle(0),
        m_context(NULL),
        m_sessionStarted(false),
        m_objectExplorer(NULL)
{
    _D("Initializing Page Session");
    m_pluginsSupport = support;
}

JSPageSession::Impl::~Impl()
{
    if (m_sessionStarted) {
        LogError("Must stop widget session before exit!");
        stopSession();
    }

    _D("Deinitializing Page Session");
}

 void JSPageSession::Impl::installStandardFunctions()
 {
    _D("========== install standard Functions START ==========");

    //add standard functions
    FOREACH(it, JsFunctionManagerSingleton::Instance().getFunctions()) {
        m_objectExplorer->registerObject(*it, NULL);
    }

    _D("========== install standard Functions END ==========");
 }

void JSPageSession::Impl::installRootPlugins()
{
    _D("========== install root Functions START ==========");

    PluginContainerSupport::PluginsList rootPlugins =
                    m_pluginsSupport->getRootPlugins();
    FOREACH(it, rootPlugins)
    {
        installPlugin(*it);
    }

    _D("========== install root Functions END ==========");
}

bool JSPageSession::Impl::installPlugin(PluginModelPtr plugin)
{
    AssertMsg(plugin, "Plugin Model is NULL");
    PluginPtr library = loadLibrary(plugin);

    if (!library) {
        _E("Loading library failed");
        return false;
    }

    _D("Install Plugin : %s", library->GetFileName().c_str());

    // Register new class
    FOREACH(it, *(library->GetClassList()))
    {
        if (!m_objectExplorer->registerObject(*it, NULL)) {
            _E("Object Registration failed : %s", (*it)->getName().c_str());
        }
    }
    return true;
}

void JSPageSession::Impl::installRequestedFeatures()
{
    _D("========== install requested Features START ==========");

    std::list<std::string> allowedFeatures =
        m_pluginsSupport->getAllowedFeatures(m_widgetHandle);

    PluginContainerSupport::PluginsList allowedPlugins;

    FOREACH(feature, allowedFeatures)
    {
        _D("Processing feature: %s", (*feature).c_str());

        PluginModelPtr plugin = m_pluginsSupport->getPluginForFeature(*feature);

        if (!plugin) {
            _W("It didn't have plugins! : %s", (*feature).c_str());
            continue;
        }

        ImplementedObjectsList implObjs =
            PluginDAOReadOnly::
                getImplementedObjectsForPluginHandle(plugin->Handle.Get());

        FOREACH(obj, implObjs)
        {
            _D("Processing object: %s", (*obj).c_str());
            /* This can be optimalized, but would need extra data in database.
             * There should be a list of features that are allowed to be
             * installed at widget start */
            if (obj->find(".") == obj->rfind(".")) {
                allowedPlugins.push_back(plugin);
                _D("Plugin will be added: %s", plugin->LibraryName.Get().c_str());
                break;
            }
        }
    }

    FOREACH(plugin, allowedPlugins)
    {
        _D("Installation plugin: %s", (*plugin)->LibraryName.Get().c_str());
        installPlugin(*plugin);
    }

    _D("========== install requested Features END ==========");
}

bool JSPageSession::Impl::loadPluginOnDemand(
    const WrtDB::DbPluginHandle &pluginHandle,
    JavaScriptObject& parentObject,
    JSGlobalContextRef context)
{
    AssertMsg(parentObject.instance && !parentObject.name.empty(),
              "Wrong arguments");

    if (!m_sessionStarted) {
        _W("Session not started");
        return false;
    }
    //    //TODO here may be a bug. if plugin contains feature rejected and
    // accepted
    //    LogDebug("Installing feature : " << widgetFeature.name);
    //    if (widgetFeature.rejected) {
    //        LogWarning("This api-feature was rejected");
    //        return;
    //    }
    //
    //    auto plugin = m_pluginsSupport->getPluginModelById(pluginHandle);
    //    if (!plugin) {
    //        LogError("Failed to load plugin. plugin handle: " <<
    // pluginHandle);
    //        return false;
    //    }
    m_pluginsSupport->registerPluginModel(pluginHandle);
    return installPluginOnDemand(
               m_pluginsSupport->getPluginModelById(pluginHandle),
               parentObject,
               context);
}

bool JSPageSession::Impl::installPluginOnDemand(PluginModelPtr plugin,
                                                JavaScriptObject& parentObject,
                                                JSGlobalContextRef context)
{
    AssertMsg(plugin, "Plugin Model is NULL");
    PluginPtr library = loadLibrary(plugin);

    if (!library) {
        _E("Loading library failed");
        return false;
    }

    _D("Install Plugin %s", library->GetFileName().c_str());

    if (!(parentObject.instance)) {
        _E("NULL pointer value");
        return false;
    }

    JSObjectPtr parent(new JSObject(parentObject.instance));

    if (!parent->getObject()) {
        _E("NULL pointer value");
        assert(false);
        return false;
    }

    FOREACH(it, *(library->GetClassList()))
    {
        bool installationStatus =
            m_objectExplorer->registerObject(*it,
                                             parentObject.name,
                                             parent,
                                             context);

        if (!installationStatus) {
            _E("Object Registration failed : %s; Parent object name: %s",
               (*it)->getName().c_str(),
               parentObject.name.c_str());
            return false;
        }
    }

    _D("Plugin on demand registration completed");
    return true;
}

void JSPageSession::Impl::setCustomProperties(double scaleFactor,
                                              const char* encodedBundle,
                                              const char* theme)
{
    m_objectExplorer->getWindowPropertySupport()
        ->setScaleToNavigatorProperty(scaleFactor);
    m_objectExplorer->getWindowPropertySupport()
        ->setBundleToWindowProperty(encodedBundle);
    m_objectExplorer->getWindowPropertySupport()
        ->setThemeToNavigatorProperty(theme);
}

void JSPageSession::Impl::dispatchJavaScriptEvent(CustomEventType eventType,
                                                  void* data)
{
    // Check if session is already started
    if (!m_sessionStarted) {
        _W("Session not started!");
        return;
    }
    m_objectExplorer->callEventListeners(eventType, data);
}

void JSPageSession::Impl::startSession(int widgetHandle,
                                       JSGlobalContextRef context,
                                       double scaleFactor,
                                       const char* encodedBundle,
                                       const char* theme)
{

    // Check if corresponding session if not already created
    if (m_sessionStarted) {
        _W("Session already started!");
        return;
    }

    // Create js object explorer object
    m_objectExplorer = new Explorer(context);

    m_sessionStarted = true;
    m_widgetHandle = widgetHandle;
    m_loadedPlugins.clear();
    m_context = context;

    // Register standard features
    installStandardFunctions();

    WidgetDAOReadOnly dao(m_widgetHandle);
    WidgetType appType = dao.getWidgetType();
    if (appType == WrtDB::APP_TYPE_TIZENWEBAPP) {
        installRootPlugins();
    }
    // Register special features
    installRequestedFeatures();

    // set scale, bundle as window's property
    setCustomProperties(scaleFactor, encodedBundle, theme);
}

void JSPageSession::Impl::stopSession()
{
    if (!m_sessionStarted) {
        _W("Session not started!");
        return;
    }

    unloadPluginsFromSession();
    m_sessionStarted = false;
}

void JSPageSession::Impl::unloadPluginsFromSession()
{
    m_objectExplorer->removePluginsFromIframes();
    m_objectExplorer->cleanIframesData();

    // delete js object for overlayed js functions
    FOREACH(it, JsFunctionManagerSingleton::Instance().getFunctions())
    {
        m_objectExplorer->deregisterObject(*it);
    }

    // delete js object for plugins
    FOREACH(pluginIt, m_loadedPlugins)
    {
        _D("Unregistering plugin %s", (*pluginIt)->GetFileName().c_str());
        (*pluginIt)->OnWidgetStop(m_widgetHandle);

        FOREACH(it, *((*pluginIt)->GetClassList())) {
            m_objectExplorer->deregisterObject(*it);
        }
    }

    JavaScriptInterfaceSingleton::Instance().invokeGarbageCollector(m_context);

    m_loadedPlugins.clear();

    delete m_objectExplorer;
    m_objectExplorer = NULL;
}

void JSPageSession::Impl::performLibrariesUnload()
{
#if 0
    LogDebug("Perform library unload");

    size_t unloadedLibraries = 0;

    FOREACH(pluginIt, m_loadedPlugins)
    {
        LogDebug("Preparing library: " << (*pluginIt)->LibraryName.Get());

        PluginPtr plugin = (*pluginIt)->LibraryInstance.Get();
        if (!plugin) {
            LogWarning("Library not loaded " << (*pluginIt)->LibraryName.Get());
            continue;
        }
        unloadedLibraries++;
        (*pluginIt)->LibraryInstance.Set(PluginPtr());
    }

    LogDebug("unloaded " << unloadedLibraries << " unreferenced libraries!");
#endif
}

PluginPtr JSPageSession::Impl::loadLibrary(PluginModelPtr& pluginModel)
{
    PluginPtr pluginLib = pluginModel->LibraryInstance.Get();
    if (!pluginLib) {
        std::string path = pluginModel->LibraryPath.Get() +
            std::string(LIBRARY_PATH_SEPARATOR) +
            pluginModel->LibraryName.Get();

        pluginLib = Plugin::LoadFromFile(path);

        if (!pluginLib) {
            _E("Loading library failed");
        } else {
            pluginModel->LibraryInstance.Set(pluginLib);

            _D("On widget start");
            // This is first time for this plugin, start widget Session
            pluginLib->OnWidgetStart(
                m_widgetHandle);
            m_loadedPlugins.insert(pluginLib);

            FOREACH(context, m_loadedContexts)
            {
                pluginLib->OnFrameLoad(*context);
            }
        }
    } else {
        _D("Get from LibraryInstance");
        _D("On widget start");
        // This is first time for this plugin, start widget Session
        pluginLib->OnWidgetStart(
            m_widgetHandle);
        m_loadedPlugins.insert(pluginLib);

        FOREACH(context, m_loadedContexts)
        {
            pluginLib->OnFrameLoad(*context);
        }
    }

    return pluginLib;
}

void JSPageSession::Impl::loadFrame(JSGlobalContextRef context)
{
    if (!m_sessionStarted) {
        _W("Session NOT started!");
        return;
    }

    m_loadedContexts.insert(context);

    FOREACH(pluginIt, m_loadedPlugins)
    {
        _D("Try to call 'OnFrameLoad' callback : %s",
           (*pluginIt)->GetFileName().c_str());
        (*pluginIt)->OnFrameLoad(context);
    }

    m_objectExplorer->loadFrame(context);
}

void JSPageSession::Impl::unloadFrame(JSGlobalContextRef context)
{
    if (!m_sessionStarted) {
        _W("Session NOT started!");
        return;
    }

    m_loadedContexts.erase(context);

    FOREACH(pluginIt, m_loadedPlugins)
    {
        _D("Try to call 'OnFrameUnload' callback : %s",
           (*pluginIt)->GetFileName().c_str());
        (*pluginIt)->OnFrameUnload(context);
    }

    m_objectExplorer->unloadFrame(context);
}

void JSPageSession::startSession(int widgetHandle,
                                 JSGlobalContextRef ctx,
                                 double scaleFactor,
                                 const char* encodedBundle,
                                 const char* theme)
{
    m_impl->startSession(widgetHandle, ctx, scaleFactor, encodedBundle, theme);
}

void JSPageSession::stopSession()
{
    m_impl->stopSession();
}

void JSPageSession::performLibrariesUnload()
{
    m_impl->performLibrariesUnload();
}

bool JSPageSession::loadPluginOnDemand(
    const WrtDB::DbPluginHandle &pluginHandle,
    JavaScriptObject& parentObject,
    JSGlobalContextRef context)
{
    return m_impl->loadPluginOnDemand(pluginHandle, parentObject, context);
}

void JSPageSession::setCustomProperties(double scaleFactor,
                                        const char* encodedBundle,
                                        const char* theme)
{
    m_impl->setCustomProperties(scaleFactor, encodedBundle, theme);
}

void JSPageSession::dispatchJavaScriptEvent(CustomEventType eventType,
                                            void* data)
{
    m_impl->dispatchJavaScriptEvent(eventType, data);
}

void JSPageSession::loadFrame(JSGlobalContextRef context)
{
    m_impl->loadFrame(context);
}

void JSPageSession::unloadFrame(JSGlobalContextRef context)
{
    m_impl->unloadFrame(context);
}

JSPageSession::JSPageSession(const PluginContainerSupportPtr& containerSupport)
    :
    m_impl(new JSPageSession::Impl(containerSupport))
{}

JSPageSession::~JSPageSession()
{}
