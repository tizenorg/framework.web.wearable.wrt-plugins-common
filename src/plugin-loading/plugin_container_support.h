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
#ifndef PLUGIN_CONTAINER_SUPPORT_H
#define PLUGIN_CONTAINER_SUPPORT_H

#include <memory>
#include <set>
#include <string>

#include <dpl/wrt-dao-ro/feature_dao_read_only.h>
#include <dpl/wrt-dao-ro/feature_model.h>

#include "plugin_model.h"

class PluginContainerSupport
{
  public:
    typedef std::list<WrtDB::FeatureModelPtr> FeaturesList;
    typedef std::list<PluginModelPtr> PluginsList;

  private:
    // Standard features
    typedef std::list<std::string> StandardFeatureList;

    // Main feature list. Always equal to DB feature list
    typedef std::set<WrtDB::FeatureModelPtr> FeatureModelContainer;

    // Main plugin list. Always equal to DB plugin list
    typedef std::set<PluginModelPtr> PluginModelsContainer;

    WrtDB::PluginHandleList m_rootPluginsList;

    PluginModelsContainer m_pluginModels;

    FeatureModelContainer m_featureModels;

    bool m_initialized;

    int m_widgetHandle;

  private:
    // Reading
     void readRootPluginsList();

    // Plugin models
    typedef std::multimap<WrtDB::FeatureHandle, std::string> DeviceCapList;
    void registerFeatureModel(WrtDB::FeatureHandle handle,
                              WrtDB::FeatureData* data,
                              DeviceCapList deviceCapabilities);

    WrtDB::FeatureModelPtr getFeatureModel(const std::string &name) const;
    WrtDB::FeatureModelPtr getFeatureModel(WrtDB::FeatureHandle handle) const;

    PluginModelPtr getPluginModel(const WrtDB::FeatureModelPtr &feature) const;

  public:
    PluginContainerSupport();

    bool isInitialized()
    {
        return m_initialized;
    }
    bool isInitialized(int widgetHandle)
    {
        return (m_initialized && m_widgetHandle == widgetHandle);
    }
    void Initialize(int widgetHandle);
    void registerPluginModel(WrtDB::DbPluginHandle handle);
    std::list<std::string> getAllowedFeatures(int widgetHandle) const;
    void readAllowedPlugins(int widgetHandle);
    void readRootPlugins(int widgetHandle);
    PluginsList  getRootPlugins() const;
    PluginsList  getPluginsList() const;

    PluginModelPtr getPluginForFeature(const std::string& featureName);

    PluginModelPtr getPluginModelById(WrtDB::DbPluginHandle handle) const;

    ~PluginContainerSupport();
};

typedef std::shared_ptr<PluginContainerSupport> PluginContainerSupportPtr;

#endif // PLUGIN_CONTAINER_SUPPORT_H
