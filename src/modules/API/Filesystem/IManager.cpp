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
#include <Filesystem/Manager.h>
#include <Commons/ThreadPool.h>
#include "IManager.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
IManager& IManager::getInstance()
{
    static Manager instance;
    return instance;
}

bool IManager::fileExists(const std::string &file)
{
    return WrtDeviceApis::Filesystem::Manager::fileExists(file);
}

IManager::IManager() :
    Commons::EventRequestReceiver<EventResolve>(
        Commons::ThreadEnum::FILESYSTEM_THREAD),
    Commons::EventRequestReceiver<EventCopy>(
        Commons::ThreadEnum::FILESYSTEM_THREAD),
    Commons::EventRequestReceiver<EventMove>(
        Commons::ThreadEnum::FILESYSTEM_THREAD),
    Commons::EventRequestReceiver<EventRemove>(
        Commons::ThreadEnum::FILESYSTEM_THREAD),
    Commons::EventRequestReceiver<EventFind>(
        Commons::ThreadEnum::FILESYSTEM_THREAD)
{}

IManager::~IManager()
{}
} // API
} // Filesystem
} // WrtDeviceApis
