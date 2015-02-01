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
 * @file        plugin_container_support.h
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version     1.0
 */

#include "plugin_container_support.h"

#include <fstream>

#include <dpl/log/secure_log.h>
#include <dpl/foreach.h>
#include <dpl/wrt-dao-ro/feature_dao_read_only.h>
#include <dpl/wrt-dao-ro/global_config.h>

 using namespace WrtDB;

#include <dpl/wrt-dao-ro/widget_dao_read_only.h>

PluginContainerSupport::PluginContainerSupport() :
    m_initialized(false),
    m_widgetHandle(0)
{
     // Reading root plugins list from so files
    readRootPluginsList();
}

PluginContainerSupport::~PluginContainerSupport()
{
    // Remove all plugin models
    m_pluginModels.clear();

    // Remove all feature models
    m_featureModels.clear();
 }

void PluginContainerSupport::Initialize(int widgetHandle)
{
    if (isInitialized() == true && m_widgetHandle != widgetHandle) {
        // re-initialize required
        m_initialized = false;

        m_pluginModels.clear();
        m_featureModels.clear();
    }

    if (isInitialized() == false) {
        m_initialized = true;
        m_widgetHandle = widgetHandle;

        readAllowedPlugins(widgetHandle);
        readRootPlugins(widgetHandle);
    }
}

std::list<std::string> PluginContainerSupport::getAllowedFeatures(
    int widgetHandle) const
{
    //TODO it has to return LIST NOT SET!!!
    WidgetDAOReadOnly widgetDao(widgetHandle);
    DbWidgetFeatureSet features = widgetDao.getFeaturesList();

    std::list<std::string> allowedFeatures;
    FOREACH(it, features) {
        _D("Loading api-feature: %s", DPL::ToUTF8String(it->name).c_str());
        if (it->rejected) {
            _W("Api-feature was rejected by ace. (Api-feature name: %s)",
               it->name.c_str());
            continue;
        }

        allowedFeatures.push_back(DPL::ToUTF8String(it->name));
    }
    return allowedFeatures;
}

void PluginContainerSupport::readAllowedPlugins(int widgetHandle)
{
    std::list<std::string> allowedFeatures;
    auto requested = getAllowedFeatures(widgetHandle);
    FOREACH(f, requested)
    {
        allowedFeatures.push_back(*f);
    }

    FeatureData* dt = NULL;
    std::map<FeatureHandle,
             FeatureData> featureDataList = FeatureDAOReadOnly::GetFeatures(
            allowedFeatures);
    DeviceCapList deviceCapabilities =
        FeatureDAOReadOnly::GetDevCapWithFeatureHandle();
    FOREACH(data, featureDataList) {
        dt = &(data->second);
        registerPluginModel(dt->pluginHandle);
        registerFeatureModel(data->first, dt, deviceCapabilities);
    }
}

void PluginContainerSupport::readRootPlugins(int widgetHandle)
{
    WidgetDAOReadOnly dao(widgetHandle);
    WidgetType appType = dao.getWidgetType();
    if (appType == WrtDB::APP_TYPE_TIZENWEBAPP) {
         FOREACH(it_rootPluginHandle, m_rootPluginsList)
        {
            _D("*it_rootPluginHandle: %d", *it_rootPluginHandle);
            registerPluginModel(*it_rootPluginHandle);
        }
    } else {
        _D("Not defined app type");
    }
}

void PluginContainerSupport::registerFeatureModel(
    FeatureHandle handle,
    FeatureData* data,
    DeviceCapList
    deviceCapabilities)
{
    FeatureModelPtr model = getFeatureModel(handle);
    if (model) {
        _D("Model for feature: %d already created", handle);
        return;
    }

    _D("Creating Model for feature: %d", handle);

    model.reset(new FeatureModel(handle));

    std::set<std::string> devCapList;
    auto ret = deviceCapabilities.equal_range(handle);
    for (auto devCapIt = ret.first; devCapIt != ret.second; devCapIt++) {
        devCapList.insert((*devCapIt).second);
    }
    model->SetData(data->featureName, devCapList, data->pluginHandle);
    m_featureModels.insert(model);
}

void PluginContainerSupport::registerPluginModel(DbPluginHandle handle)
{
    PluginModelPtr model = getPluginModelById(handle);

    if (model) {
        _D("Model for plugin: %d already registered", handle);
        return;
    }

    if (PluginDAOReadOnly::INSTALLATION_COMPLETED !=
        PluginDAOReadOnly::getInstallationStateForHandle(handle))
    {
        _W("Failed To CreateModel for handle %d", handle);
        return;
    }

    model.Reset(new PluginModel(handle));

    _D("Model Created. Handle: %d, name: %s",
       handle,
       model->LibraryName.Get().c_str());

    m_pluginModels.insert(model);
}

void PluginContainerSupport::readRootPluginsList()
{
    _D("Reading root plugins list from so files...");
    m_rootPluginsList = PluginDAOReadOnly::getRootPluginHandleList();
}

FeatureModelPtr
PluginContainerSupport::getFeatureModel(const std::string &name) const
{
    FOREACH(iter, m_featureModels)
    {
        if ((*iter)->Name.Get() == name) {
            return *iter;
        }
    }

    return FeatureModelPtr();
}

FeatureModelPtr
PluginContainerSupport::getFeatureModel(const FeatureHandle handle) const
{
    FOREACH(iter, m_featureModels)
    {
        if ((*iter)->FHandle.Get() == handle) {
            return *iter;
        }
    }

    return FeatureModelPtr();
}

PluginModelPtr
PluginContainerSupport::getPluginModelById(DbPluginHandle handle) const
{
    FOREACH(pluginModel, m_pluginModels)
    {
        if ((*pluginModel)->Handle.Get() == handle) {
            return *pluginModel;
        }
    }

    return PluginModelPtr();
}

PluginModelPtr
PluginContainerSupport::getPluginModel(const FeatureModelPtr &feature) const
{
    if (!feature) {
        _D("Null Ptr for feature model");
        return PluginModelPtr();
    } else {
        _D("Feature located in plugin: %d", feature->PHandle.Get());
        return getPluginModelById(feature->PHandle.Get());
    }
}

 PluginContainerSupport::PluginsList
PluginContainerSupport::getRootPlugins() const
{
    PluginsList plugins;

    FOREACH(it, m_rootPluginsList) {
        PluginModelPtr plugin = getPluginModelById(*it);
        if (!plugin) {
            _W("PluginModel not found");
            continue;
        }

        plugins.push_back(plugin);
    }

    return plugins;
}

PluginContainerSupport::PluginsList
PluginContainerSupport::getPluginsList() const
{
    PluginsList plugins;

    FOREACH(it, m_pluginModels) {
        plugins.push_back(*it);
    }

    return plugins;
}

PluginModelPtr
PluginContainerSupport::getPluginForFeature(const std::string& featureName)
{
    return getPluginModel(getFeatureModel(featureName));
}
