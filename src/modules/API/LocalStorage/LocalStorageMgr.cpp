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
 *
 *
 * @file       LocalStorageMgr.cpp
 * @author     Andrzej Surdej (a.surdej@samsung.com)
 * @version    0.1
 * @brief
 */
#include "LocalStorageMgr.h"
#include <WidgetInterface/WidgetInterface.h>

namespace WrtDeviceApis {
namespace LocalStorage {
namespace Api {
ILocalStoragePtr getLocalStorage(int widgetId)
{
    return ILocalStoragePtr(new WrtDeviceApis::WidgetInterfaceObject(widgetId));
}
} // Api
} // LocalStorage
} // WrtDeviceApis
