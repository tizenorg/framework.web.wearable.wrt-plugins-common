/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file    IObject_cast.h
 * @author
 * @version
 * @brief
 */
#ifndef _WRT_PLUGINS_COMMON_PLUGINS_API_SUPPORT_CAST_H_
#define _WRT_PLUGINS_COMMON_PLUGINS_API_SUPPORT_CAST_H_

#include <memory>
#include <IObject.h>
#include <Object.h>

namespace WrtPluginsApi {
inline ObjectPtr CAST(const IObjectPtr& object)
{
    return std::dynamic_pointer_cast<Object>(object);
}
}
#endif
