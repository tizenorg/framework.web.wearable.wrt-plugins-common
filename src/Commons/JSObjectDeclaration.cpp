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
 * @file    js_object_declaration.cpp
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */

#include <dpl/log/log.h>
#include <dpl/assert.h>
#include "JSObjectDeclaration.h"

JSObjectDeclaration::JSObjectDeclaration(js_entity_definition_ptr_t classD) :
    m_name(classD->object_name),
    m_parentName(classD->parent_name),
    m_interfaceName(classD->interface_name),
    m_classTemplate(NULL),
    m_constructorCallback(classD->js_class_constructor_cb)
{
    if (NULL != classD->js_class_template_getter_fun) {
        m_classTemplate = classD->js_class_template_getter_fun();
    }
    if (classD->class_options) {
        m_options = OptionsPtr(new Options(classD->class_options));
    }
}

JSObjectDeclaration::~JSObjectDeclaration()
{}

bool JSObjectDeclaration::checkIframesSupported() const
{
    if (!m_options ||
        m_options->getIframeObject() == Options::IFrameObject::None)
    {
        return false;
    }

    return true;
}

JSObjectDeclaration::Options::ClassType
JSObjectDeclaration::Options::getType() const
{
    AssertMsg(m_options, "Pointer to options is NULL");

    switch (m_options->type) {
    case JS_CLASS: return ClassType::Class;
    case JS_FUNCTION: return ClassType::Function;
    case JS_INTERFACE: return ClassType::Interface;
    default: Assert(!"Wrong value of type");
    }
}

JSObjectDeclaration::Options::IFrameObject
JSObjectDeclaration::Options::getIframeObject() const
{
    AssertMsg(m_options, "Options object is NULL");

    switch (m_options->iframe_option) {
    case NONE: return IFrameObject::None;
    case REFERENCE: return IFrameObject::Reference; // deprecated
    case CREATE_INSTANCE: return IFrameObject::CreateInstance;
    default:
        Assert(!"Wrong value of behaviour type");
    }
}

JSObjectDeclaration::Options::IFrameNotice
JSObjectDeclaration::Options::getIframeNotice() const
{
    AssertMsg(m_options, "Pointer to options is null");

    switch (m_options->iframe_notice) {
    case NONE_NOTICE: return IFrameNotice::None;
    case ALWAYS_NOTICE: return IFrameNotice::AlwaysNotice;
    default:
        Assert(!"Wrong value of notice option");
    }
}

js_function_impl JSObjectDeclaration::Options::getFunctionImpl() const
{
    AssertMsg(m_options, "Pointer to options is null");
    return m_options->function;
}

void JSObjectDeclaration::Options::invokeCallback(JsContext ctx,
                                                  ObjectInstance iframe,
                                                  ObjectInstance object) const
{
    LogDebug("JS Object create, notice.");
    AssertMsg(m_options && m_options->cb, "Empty callback pointer");
    m_options->cb(ctx, iframe, object);
}

JSObjectDeclaration::Options::PrivateData
JSObjectDeclaration::Options::getPrivateData() const
{
    AssertMsg(m_options && m_options->private_data, "empty private data");
    return m_options->private_data;
}
