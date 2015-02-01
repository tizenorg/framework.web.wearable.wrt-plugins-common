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
 * @file    js_function_manager.h
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */

#ifndef WRT_SRC_PLUGIN_SERVICE_JS_FUNCTION_MANGER_H_
#define WRT_SRC_PLUGIN_SERVICE_JS_FUNCTION_MANGER_H_

#include <list>
#include <dpl/noncopyable.h>
#include <dpl/singleton.h>

#include <Commons/JSObjectDeclaration.h>

class JsFunctionManager : private DPL::Noncopyable
{
  public:
    typedef std::list<JSObjectDeclarationPtr> Functions;

  public:
    Functions getFunctions();

  private:
    JsFunctionManager()
    {}

    bool initialize();

  private:
    Functions m_functions;

    friend class DPL::Singleton<JsFunctionManager>;
};

typedef DPL::Singleton<JsFunctionManager> JsFunctionManagerSingleton;

#endif

