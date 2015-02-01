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
#include "INode.h"
#include <Commons/ThreadPool.h>

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
INode::INode() :
    Commons::EventRequestReceiver<EventListNodes>(
        Commons::ThreadEnum::FILESYSTEM_THREAD),
    Commons::EventRequestReceiver<EventOpen>(
        Commons::ThreadEnum::FILESYSTEM_THREAD),
    Commons::EventRequestReceiver<EventReadText>(
        Commons::ThreadEnum::FILESYSTEM_THREAD)
{}

INode::~INode()
{}
} // API
} // Filesystem
} // WrtDeviceApis
