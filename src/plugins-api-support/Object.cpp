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
 * @file    Object.cpp
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */

#include "Object.h"

namespace WrtPluginsApi {
void Object::AddChild(const IObjectPtr& child)
{
    if (!m_children) {
        m_children = IObjectsListPtr(new IObjectsList);
    }
    m_children->push_back(child);
}

void Object::setBoolOption(IObjectOption option, bool value)
{
    if (!m_options) {
        m_options = ObjectOptionPtr(new ObjectOption);
    }

    switch (option) {
    case IObjectOption::Overlayed:
        m_options->overlayedMode = value;
        break;
    default:
        break;
    }
}

IObjectsListPtr Object::GetChildren() const
{
    return m_children;
}

ClassRef Object::GetClass() const
{
    return m_classRef;
}

ClassRef Object::GetClassConstructor() const
{
    return m_constructorRef;
}

const char* Object::GetInterfaceName() const
{
    return m_interfaceName;
}

const char* Object::GetName() const
{
    return m_name;
}

IObjectType Object::GetType() const
{
    return m_type;
}

const char* Object::GetParentName() const
{
    return m_parentName;
}

ObjectOptionPtr Object::GetOptions() const
{
    return m_options;
}

Object::Object(const char* name,
               ClassRef ref,
               IObjectType type) :
    m_name(name),
    m_classRef(ref),
    m_parentName(0),
    m_type(type),
    m_interfaceRef(0),
    m_interfaceName(0),
    m_constructorRef(0)
{}

Object::Object(const char* name,
               ClassRef ref,
               const char* parentName,
               IObjectType type) :
    m_name(name),
    m_classRef(ref),
    m_parentName(parentName),
    m_type(type),
    m_interfaceRef(0),
    m_interfaceName(0),
    m_constructorRef(0)
{}

Object::Object(const char* name,
               ClassRef interfaceRef,
               const char* interfaceName,
               ClassRef constructorRef,
               const char* parentName,
               IObjectType type) :
    m_name(name),
    m_parentName(parentName),
    m_type(type),
    m_interfaceRef(interfaceRef),
    m_interfaceName(interfaceName),
    m_constructorRef(constructorRef)
{}

Object::~Object()
{}
}
