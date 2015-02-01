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
 * @file    plugin_installer_types.h
 * @author  Andrzej Surdej (a.surdej@samgsung.com)
 * @version
 * @brief
 */

#ifndef WRT_PLUGINS_INSTALLER_PLUGIN_INSTALLER_TYPES_H
#define WRT_PLUGINS_INSTALLER_PLUGIN_INSTALLER_TYPES_H

#include <dpl/wrt-dao-ro/common_dao_types.h>

/* Define db type */
typedef WrtDB::DbWidgetHandle WidgetHandle;
typedef WrtDB::DbWidgetHandleList WidgetHandleList;

typedef WrtDB::DbWidgetFeature WidgetFeature;
typedef WrtDB::DbWidgetFeatureSet WidgetFeatureSet;

typedef WrtDB::DbPluginHandle PluginHandle;

#endif /* WRT_PLUGINS_INSTALLER_PLUGIN_INSTALLER_TYPES_H */