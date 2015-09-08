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

class PluginLogic::Impl
{
    PluginContainerSupportPtr m_pluginsSupport;

    typedef std::map<JSContextRef, JSPageSessionPtr> PagesSessionsSet;
    PagesSessionsSet m_sessions;
    unsigned int m_windowHandle;

  public:
    Impl();
    ~Impl();

    // Widget session
    void initSession(int widgetHandle);
    void startSession(int widgetHandle,
                      JSGlobalContextRef context,
                      double scaleFactor,
                      const char* encodedBundle,
                      const char* theme);

    void stopSession(JSGlobalContextRef context);

    void performLibrariesUnload();

    bool loadPluginOnDemand(const WrtDB::DbPluginHandle &pluginHandle,
                            JavaScriptObject& parentObject,
                            JSGlobalContextRef context);

    void loadFrame(JSGlobalContextRef context);
    void unloadFrame(JSGlobalContextRef context);

    void setCustomProperties(JSGlobalContextRef ctx,
                             double scaleFactor,
                             const char* encodedBundle,
                             const char* theme);

    void dispatchJavaScriptEvent(JSGlobalContextRef ctx,
                                 CustomEventType eventType,
                                 void* data);

    unsigned int windowHandle() const;
    void setWindowHandle(unsigned int handle);

    static bool s_sanityCheck;
};

IMPLEMENT_SINGLETON(PluginLogic);

bool PluginLogic::Impl::s_sanityCheck = false;

#define PLUGIN_LOGIC_SANITY_CHECK \
    if (!s_sanityCheck) \
    { \
        _E("Object is not available. Wrong flow occured"); \
        return; \
    }

PluginLogic::Impl::Impl() :
    m_windowHandle(0)
{
    s_sanityCheck = true;

    DPL::Log::LogSystemSingleton::Instance().SetTag("WRT_PLUGINS");
    _D("Initializing Plugin Logic...");
    m_pluginsSupport = PluginContainerSupportPtr(new PluginContainerSupport());

    // explicit call to keep singleton's lifetime until calling destructor.
    JsFunctionManagerSingleton::Instance();
    JavaScriptInterfaceSingleton::Instance();
}

PluginLogic::Impl::~Impl()
{
    _D("called");

    s_sanityCheck = false;

    FOREACH(it, m_sessions)
    {
        _W("Must stop widget session before exit!");
        it->second->stopSession();
    }
}

void PluginLogic::initSession(int widgetHandle)
{
    m_impl->initSession(widgetHandle);
}

void PluginLogic::startSession(int widgetHandle,
                               JSGlobalContextRef ctx,
                               double scaleFactor,
                               const char* encodedBundle,
                               const char* theme)
{
    m_impl->startSession(widgetHandle, ctx, scaleFactor, encodedBundle, theme);
}

void PluginLogic::stopSession(JSGlobalContextRef context)
{
    m_impl->stopSession(context);
}

void PluginLogic::performLibrariesUnload()
{
    _W("This function is DEPRECATED");
    //    m_impl->performLibrariesUnload();
}

bool PluginLogic::loadPluginOnDemand(
    const WrtDB::DbPluginHandle &pluginHandle,
    JavaScriptObject& parentObject,
    JSGlobalContextRef context)
{
    return m_impl->loadPluginOnDemand(pluginHandle, parentObject, context);
}

void PluginLogic::loadPluginsIntoIframes(JSGlobalContextRef /*context*/)
{
    _W("This function is DEPRECATED");
}

void PluginLogic::setCustomProperties(double /*scaleFactor*/,
                                      const char* /*encodedBundle*/,
                                      const char* /*theme*/)
{
    _W("This function is DEPRECATED");
}

void PluginLogic::setCustomProperties(JSGlobalContextRef context,
                                      double scaleFactor,
                                      const char* encodedBundle,
                                      const char* theme)
{
    m_impl->setCustomProperties(context, scaleFactor, encodedBundle, theme);
}

void PluginLogic::dispatchJavaScriptEvent(CustomEventType /*eventType*/)
{
    _W("This function is DEPRECATED");
}

void PluginLogic::dispatchJavaScriptEvent(JSGlobalContextRef context,
                                          CustomEventType eventType,
                                          void *data)
{
    m_impl->dispatchJavaScriptEvent(context, eventType, data);
}

void PluginLogic::loadFrame(JSGlobalContextRef context)
{
    m_impl->loadFrame(context);
}

void PluginLogic::unloadFrame(JSGlobalContextRef context)
{
    m_impl->unloadFrame(context);
}

unsigned int PluginLogic::windowHandle() const
{
    return m_impl->windowHandle();
}

void PluginLogic::setWindowHandle(unsigned int handle)
{
    m_impl->setWindowHandle(handle);
}

PluginLogic::PluginLogic() : m_impl(new PluginLogic::Impl())
{}

PluginLogic::~PluginLogic()
{}

void PluginLogic::Impl::initSession(int widgetHandle)
{
    _D(">---------------------[init session START]---------------------<");

    m_pluginsSupport->Initialize(widgetHandle);
    PluginContainerSupport::PluginsList rootPluginList =
        m_pluginsSupport->getRootPlugins();

    FOREACH(it, rootPluginList)
    {
        PluginModelPtr& pluginModel = *it;
        PluginPtr pluginLib = pluginModel->LibraryInstance.Get();

        if (!pluginLib) {
            std::string path = pluginModel->LibraryPath.Get() +
                std::string(LIBRARY_PATH_SEPARATOR) +
                pluginModel->LibraryName.Get();

            pluginLib = Plugin::LoadFromFile(path);

            if (!pluginLib) {
                _W("Loading library failed");
            } else {
                pluginModel->LibraryInstance.Set(pluginLib);
                _D("pluginModel->LibraryInstance.Set() : %s",
                   pluginLib->GetFileName().c_str());
            }
        } else {
            _D("Already loaded");
        }
    }
    _D("========== init session END ==========");
}

void PluginLogic::Impl::startSession(int widgetHandle,
                                     JSGlobalContextRef context,
                                     double scaleFactor,
                                     const char* encodedBundle,
                                     const char* theme)
{
    _D("========== start session START ==========");

    if (!m_pluginsSupport->isInitialized(widgetHandle)) {
        m_pluginsSupport->Initialize(widgetHandle);
    }
    auto sessionIt = m_sessions.find(context);

    // Check if corresponding session if not already created
    if (sessionIt != m_sessions.end()) {
        _W("Session already started!");
    } else {
        auto newSession = JSPageSessionPtr(new JSPageSession(m_pluginsSupport));
        newSession->startSession(widgetHandle,
                                 context,
                                 scaleFactor,
                                 encodedBundle,
                                 theme);

        m_sessions[context] = newSession;
    }
    _D("========== start session END ==========");
}

void PluginLogic::Impl::stopSession(JSGlobalContextRef context)
{
    _D("========== stop session START ==========");

    auto sessionIt = m_sessions.find(context);
    if (sessionIt == m_sessions.end()) {
        _W("Session not exist!");
    } else {
        sessionIt->second->stopSession();
        m_sessions.erase(sessionIt);
    }
    _D("========== stop session END ==========");
}

bool PluginLogic::Impl::loadPluginOnDemand(
    const WrtDB::DbPluginHandle &pluginHandle,
    JavaScriptObject& parentObject,
    JSGlobalContextRef context
    )
{
    _D("========== load ondemand plugin ==========");

    auto sessionIt = m_sessions.find(context);
    if (sessionIt == m_sessions.end()) {
        _W("Session not exist!");
        return false;
    }

    return sessionIt->second->loadPluginOnDemand(pluginHandle,
                                                 parentObject,
                                                 context);
}

void PluginLogic::Impl::loadFrame(JSGlobalContextRef context)
{
    _D("========== load frame START ==========");
    PLUGIN_LOGIC_SANITY_CHECK

    auto sessionIt = m_sessions.find(context);
    if (sessionIt == m_sessions.end()) {
        _W("Session not exist!");
    } else {
        sessionIt->second->loadFrame(context);
    }
    _D("========== load frame END ==========");
}

void PluginLogic::Impl::unloadFrame(JSGlobalContextRef context)
{
    _D("========== unload frame START ==========");
    PLUGIN_LOGIC_SANITY_CHECK

    auto sessionIt = m_sessions.find(context);
    if (sessionIt == m_sessions.end()) {
        LogWarning("Session not exist!");
    } else {
        sessionIt->second->unloadFrame(context);

        // I don't know why this session should be removed here.
        // session list is removed also from stopSession().
        //m_sessions.erase(sessionIt);
        }
    _D("========== unload frame END ==========");
}

void PluginLogic::Impl::setCustomProperties(JSGlobalContextRef context,
                                            double scaleFactor,
                                            const char* encodedBundle,
                                            const char* theme)
{
    PLUGIN_LOGIC_SANITY_CHECK

    auto sessionIt = m_sessions.find(context);
    if (sessionIt == m_sessions.end()) {
        _W("Session not exist!");
        return;
    }

    sessionIt->second->setCustomProperties(scaleFactor,
                                           encodedBundle,
                                           theme);
}

void PluginLogic::Impl::dispatchJavaScriptEvent(JSGlobalContextRef context,
                                                CustomEventType eventType,
                                                void* data)
{
    PLUGIN_LOGIC_SANITY_CHECK

    auto sessionIt = m_sessions.find(context);
    if (sessionIt == m_sessions.end()) {
        _W("Session not exist!");
        return;
    }

    sessionIt->second->dispatchJavaScriptEvent(eventType, data);
}

unsigned int PluginLogic::Impl::windowHandle() const
{
    if (!s_sanityCheck) {
        LogError("Object is not available. Wrong flow occured");
    }
    return m_windowHandle;
}

void PluginLogic::Impl::setWindowHandle(unsigned int handle)
{
    PLUGIN_LOGIC_SANITY_CHECK
    m_windowHandle = handle;
}
