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
 * @file    plugin_model.cpp
 * @author  Pawel Sikorski (p.sikorski@samgsung.com)
 * @author  Przemyslaw Dobrowolski (p.dobrowolsk@samgsung.com)
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief   This file contains PluginModel, PluginHandle definitions.
 */

#include "plugin_model.h"
#include <dpl/event/model_bind_to_dao.h>
#include <dpl/wrt-dao-ro/feature_dao_read_only.h>

using namespace WrtDB;

template <typename RetType, RetType(PluginDAOReadOnly::*extFun) () const >
struct BindToPluginDAO :
    DPL::Event::BindToDAO<PluginModel,
                          RetType,
                          WrtDB::DbPluginHandle,
                          PluginDAOReadOnly,
                          &PluginModel::getHandle,
                          extFun>
{};

template <typename RetType, RetType(*extFun) (WrtDB::DbPluginHandle)>
struct BindToFeatureDAOStatic :
    DPL::Event::BindToDAO_Static<PluginModel,
                                 RetType,
                                 WrtDB::DbPluginHandle,
                                 &PluginModel::getHandle,
                                 extFun>
{};

PluginModel::PluginModel(const WrtDB::DbPluginHandle& handle) :
    Handle(this, handle),
    LibraryName(this,
                BindToPluginDAO<std::string,
                                &PluginDAOReadOnly::getLibraryName>::Get),
    LibraryPath(this,
                BindToPluginDAO<std::string,
                                &PluginDAOReadOnly::getLibraryPath>::Get),
    FeatureHandles(this,
                   BindToFeatureDAOStatic<FeatureHandleListPtr,
                                          FeatureDAOReadOnly::
                                              GetFeatureHandleListForPlugin>::
                       Get),
    LibraryDependencies(this,
                        BindToPluginDAO<PluginHandleSetPtr,
                                        &PluginDAOReadOnly::
                                            getLibraryDependencies>::Get),
    LibraryInstance(this, PluginPtr())
{}

WrtDB::DbPluginHandle PluginModel::getHandle() const
{
    return Handle.Get();
}
