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
 * @file    js_object_declaration.h
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */

#ifndef WRT_SRC_PLUGIN_SERVICE_JS_OBJECT_DECLARATION_H_
#define WRT_SRC_PLUGIN_SERVICE_JS_OBJECT_DECLARATION_H_

#include <string>
#include <memory>
#include <cassert>
#include <dpl/shared_ptr.h>
#include <dpl/noncopyable.h>
#include <dpl/log/log.h>
#include <wrt_plugin_export.h>

class JSObjectDeclaration : private DPL::Noncopyable
{
  public:
    typedef const void* ConstClassTemplate;
    typedef void* ClassTemplate;
    typedef js_class_constructor_cb_t ConstructorCallback;
    typedef class_definition_options_t ClassOptions;

    class Options : DPL::Noncopyable
    {
      public:
        enum class ClassType
        {
            Class,
            Function,
            Interface
        };

        enum class IFrameObject
        {
            None,
            Reference,
            CreateInstance
        };

        enum class IFrameNotice
        {
            None,
            AlwaysNotice
        };

        //only for function
        enum class IFrameOverlay
        {
            Ignored,
            UseOverlayed,           //deprecated
            OverlayedBeforeOriginal //deprecated
        };

        typedef js_object_instance_t ObjectInstance;
        typedef java_script_context_t JsContext;
        typedef void* PrivateData;

      public:
        ClassType getType() const;

        IFrameObject getIframeObject() const;
        IFrameNotice getIframeNotice() const;
        js_function_impl getFunctionImpl() const;

        void invokeCallback(JsContext ctx,
                            ObjectInstance iframe,
                            ObjectInstance object) const;

        PrivateData getPrivateData() const;

      private:
        const ClassOptions* m_options;

      private:
        explicit Options(const ClassOptions* options) : m_options(options)
        {
            assert(options && "Dont create empty options");
        }

        friend class JSObjectDeclaration;
    };

    typedef std::shared_ptr<Options> OptionsPtr;

  public:

    explicit JSObjectDeclaration(js_entity_definition_ptr_t declaration);

    virtual const std::string& getName() const
    {
        return m_name;
    }

    virtual const std::string& getParentName() const
    {
        return m_parentName;
    }

    virtual ConstClassTemplate getClassTemplate() const
    {
        return m_classTemplate;
    }

    virtual const std::string& getInterfaceName() const
    {
        return m_interfaceName;
    }

    virtual ConstructorCallback getConstructorCallback() const
    {
        return m_constructorCallback;
    }

    const OptionsPtr getOptions() const
    {
        return m_options;
    }

    bool checkIframesSupported() const;

    virtual ~JSObjectDeclaration();

  private:
    std::string m_name;
    std::string m_parentName;
    std::string m_interfaceName;
    ConstClassTemplate m_classTemplate;
    ConstructorCallback m_constructorCallback;
    OptionsPtr m_options;
};

typedef DPL::SharedPtr<JSObjectDeclaration> JSObjectDeclarationPtr;

#endif
