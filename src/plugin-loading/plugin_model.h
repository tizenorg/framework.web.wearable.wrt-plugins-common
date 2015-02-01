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
 * @file    plugin_model.h
 * @author  Pawel Sikorski (p.sikorski@samgsung.com)
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief   This file contains PluginModel, PluginHandle definitions.
 */

#ifndef WRT_SRC_PLUGIN_SERVICE_PLUGIN_MODEL_H_
#define WRT_SRC_PLUGIN_SERVICE_PLUGIN_MODEL_H_

#include <string>
#include <list>
#include <dpl/shared_ptr.h>
#include <dpl/event/model.h>
#include <dpl/event/property.h>

#include <dpl/wrt-dao-ro/feature_model.h>
#include <dpl/wrt-dao-ro/plugin_dao_read_only.h>
#include <plugin.h>

class PluginModel;

class PluginModel : public DPL::Event::Model
{
  public:

    /**
     * *@brief Plugin handle
     */
    DPL::Event::Property<WrtDB::DbPluginHandle,
                         DPL::Event::PropertyReadOnly> Handle;

    /**
     * *@brief LibraryName
     */
    DPL::Event::Property<std::string,
                         DPL::Event::PropertyReadOnly,
                         DPL::Event::PropertyStorageDynamicCached> LibraryName;

    /**
     * *@brief Library Path
     */
    DPL::Event::Property<std::string,
                         DPL::Event::PropertyReadOnly,
                         DPL::Event::PropertyStorageDynamicCached> LibraryPath;

    /**
     * *@brief Feature set
     */
    DPL::Event::Property<WrtDB::FeatureHandleListPtr,
                         DPL::Event::PropertyReadOnly,
                         DPL::Event::PropertyStorageDynamicCached>
    FeatureHandles;

    /**
     * *@brief Librarydepencies
     */
    DPL::Event::Property<WrtDB::PluginHandleSetPtr,
                         DPL::Event::PropertyReadOnly,
                         DPL::Event::PropertyStorageDynamicCached>
    LibraryDependencies;

    /**
     * *@brief Library instance Low Level
     * */
    DPL::Event::Property<PluginPtr,
                         DPL::Event::PropertyReadWrite,
                         DPL::Event::PropertyStorageCached> LibraryInstance;

  public:
    PluginModel(const WrtDB::DbPluginHandle& handle);

  private:
    WrtDB::DbPluginHandle getHandle() const;
};

typedef DPL::SharedPtr<PluginModel> PluginModelPtr;

#endif
