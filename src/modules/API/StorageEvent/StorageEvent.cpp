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
 * @file        StorageEvent.cpp
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 * @brief       Storage event class implementation
 */

#include "StorageEvent.h"

namespace WrtDeviceApis {
namespace StorageEvent {
namespace Api {
StorageEvent::StorageEvent() :
    m_storageArea(NULL) {}

StorageEvent::StorageEvent(
    const LocalStorage::Api::ILocalStoragePtr &storageArea) :
    m_storageArea(storageArea) {}

std::string StorageEvent::getKey() const
{
    return m_key;
}

DPL::OptionalString StorageEvent::getOldValue() const
{
    return m_oldValue;
}

DPL::OptionalString StorageEvent::getNewValue() const
{
    return m_newValue;
}
std::string StorageEvent::getUrl() const
{
    return m_url;
}

LocalStorage::Api::ILocalStoragePtr StorageEvent::getStorageArea() const
{
    return m_storageArea;
}

void StorageEvent::setKey(const std::string &inKey)
{
    m_key = inKey;
}

void StorageEvent::setOldValue(const std::string &inValue)
{
    m_oldValue = DPL::FromUTF8String(inValue);
}

void StorageEvent::setNewValue(const std::string &inValue)
{
    m_newValue = DPL::FromUTF8String(inValue);
}

void StorageEvent::setUrl(const std::string &inUrl)
{
    m_url = inUrl;
}

void StorageEvent::setStorageArea(
    const LocalStorage::Api::ILocalStoragePtr &inStorageArea)
{
    m_storageArea = inStorageArea;
}
} // Api
} // StorageEvent
} // WrtDeviceApis
