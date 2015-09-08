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
 * @file        StorageEvent.h
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 * @brief       Storage event class
 */

#ifndef WRTDEVICEAPIS_STORAGEEVENT_STORAGE_EVENT_H
#define WRTDEVICEAPIS_STORAGEEVENT_STORAGE_EVENT_H

#include <dpl/optional_typedefs.h>
#include "IStorageEvent.h"

namespace WrtDeviceApis {
namespace StorageEvent {
namespace Api {
class StorageEvent : public IStorageEvent
{
  public:

    StorageEvent();
    explicit StorageEvent(
        const LocalStorage::Api::ILocalStoragePtr &storageArea);

    std::string getKey() const;
    DPL::OptionalString getOldValue() const;
    DPL::OptionalString getNewValue() const;
    std::string getUrl() const;
    LocalStorage::Api::ILocalStoragePtr getStorageArea() const;

    void setKey(const std::string &inKey);
    void setOldValue(const std::string &inValue);
    void setNewValue(const std::string &inValue);
    void setUrl(const std::string &inUrl);
    void setStorageArea(
        const LocalStorage::Api::ILocalStoragePtr &inStorageArea);

  private:
    std::string m_key;
    DPL::OptionalString m_oldValue;
    DPL::OptionalString m_newValue;
    std::string m_url;
    LocalStorage::Api::ILocalStoragePtr m_storageArea;
};
} // Api
} // StorageEvent
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_STORAGEEVENT_STORAGE_EVENT_H
