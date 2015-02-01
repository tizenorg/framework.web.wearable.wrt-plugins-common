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
 * @file    IObject.h
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */
#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_IOBJECT_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_IOBJECT_H_

#include <memory>
#include <list>

extern "C" {
typedef struct OpaqueJSClass* (*ClassRef)();
}

namespace WrtPluginsApi {
enum class IObjectType
{
    Object,
    Function,
    Interface,
    InterfaceInstance
};

enum class IObjectOption
{
    Overlayed
};

class IObject;
typedef std::shared_ptr<IObject> IObjectPtr;

class IObject
{
  public:
    constexpr static const char* WINDOW_OBJECT()
    {
        return "window";
    }

    virtual void AddChild(const IObjectPtr&) = 0;

    /*
     * Optional
     * */
    virtual void setBoolOption(IObjectOption option, bool value) = 0;

    virtual ~IObject(){}
};

typedef std::list<IObjectPtr> IObjectsList;
typedef std::shared_ptr<IObjectsList> IObjectsListPtr;
}
#endif
