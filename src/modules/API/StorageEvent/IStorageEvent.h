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
 * @file        IStorageEvent.h
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 * @brief       Storage event interfece
 */

#ifndef WRTDEVICEAPIS_COMMONS_STORAGE_EVENT_INTERFACE_H
#define WRTDEVICEAPIS_COMMONS_STORAGE_EVENT_INTERFACE_H

#include <dpl/optional_typedefs.h>
#include <dpl/shared_ptr.h>
#include <LocalStorage/ILocalStorage.h>

namespace WrtDeviceApis {
namespace StorageEvent {
namespace Api {
class IStorageEvent
{
  public:

    virtual std::string getKey() const = 0;
    virtual DPL::OptionalString getOldValue() const = 0;
    virtual DPL::OptionalString getNewValue() const = 0;
    virtual std::string getUrl() const = 0;
    virtual LocalStorage::Api::ILocalStoragePtr getStorageArea() const = 0;

    virtual void setKey(const std::string &inKey) = 0;
    virtual void setOldValue(const std::string &inValue) = 0;
    virtual void setNewValue(const std::string &inValue) = 0;
    virtual void setUrl(const std::string &inUrl) = 0;
    virtual void setStorageArea(
        const LocalStorage::Api::ILocalStoragePtr &inStorageArea) = 0;
};

typedef DPL::SharedPtr<IStorageEvent> IStorageEventPtr;
} // Api
} // StorageEvent
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_COMMONS_STORAGE_EVENT_INTERFACE_H
