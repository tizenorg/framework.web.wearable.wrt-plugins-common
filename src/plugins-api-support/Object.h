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

#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_OBJECT_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_OBJECT_H_

#include <memory>
#include <list>

#include <IObject.h>
#include <dpl/optional_typedefs.h>

namespace WrtPluginsApi {
class Object;
typedef std::shared_ptr<Object> ObjectPtr;

struct ObjectOption
{
    DPL::OptionalBool overlayedMode;
};
typedef std::shared_ptr<ObjectOption> ObjectOptionPtr;

class Object : public IObject
{
  public:
    Object(const char* name,
           ClassRef ref,
           IObjectType type = IObjectType::Object);

    Object(const char* name,
           ClassRef ref,
           const char* parentName = IObject::WINDOW_OBJECT(),
           IObjectType type = IObjectType::Object);

    Object(const char* name,
           ClassRef interfaceRef,
           const char* interfaceName,
           ClassRef constructorRef,
           const char* parentName = IObject::WINDOW_OBJECT(),
           IObjectType type = IObjectType::Object);

    ~Object();

    void AddChild(const IObjectPtr&);

    void setBoolOption(IObjectOption option, bool value);

    IObjectsListPtr GetChildren() const;

    ClassRef GetClass() const;

    /*
     * Available only for object with type InterfaceInstance
     * */
    ClassRef GetClassConstructor() const;

    const char* GetInterfaceName() const;

    const char* GetName() const;

    IObjectType GetType() const;

    const char* GetParentName() const;

    ObjectOptionPtr GetOptions() const;

  private:
    const char* m_name;
    ClassRef m_classRef;

    const char* m_parentName;

    IObjectType m_type;

    ClassRef m_interfaceRef;
    const char* m_interfaceName;
    ClassRef m_constructorRef;

    ObjectOptionPtr m_options;

    IObjectsListPtr m_children;
};
}

#endif
