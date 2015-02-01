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
 * @file    plugin_iframe_support.h
 * @version 1.0
 * @brief
 */

#ifndef _WRTENGINE_SRC_PLUGINSERVICE_IFRAME_SUPPORT_H_
#define _WRTENGINE_SRC_PLUGINSERVICE_IFRAME_SUPPORT_H_

#include <list>
#include <set>
#include <Commons/JSObjectDeclaration.h>
#include <Commons/JSObject.h>
#include "javascript_interface.h"

class IframesSupport
{
  public:
    typedef std::list<JSObjectDeclarationPtr> DeclarationsList;

  public:
    void registerDeclaration(const JSObjectDeclarationPtr& declaration);

    void registerIframe(const JSObjectPtr& iframe);

    void unregisterIframe(const JSObjectPtr& iframe);

    DeclarationsList getIframeObjects() const;

    bool hasIframes() const;

    void clean();

  private:
    std::list<JSObjectDeclarationPtr> m_iframesObjects;
    std::set<JSObjectPtr> m_loadedIframes;

    // used to find matching object from m_loadedIframes
    struct EqualToJSObjectPtr
    {
        typedef JSObjectPtr first_argument_type;
        typedef JSObjectPtr second_argument_type;
        typedef bool result_type;

        result_type operator()(const first_argument_type &s1,
                               const second_argument_type &s2) const
        {
            return (s1->getObject() == s2->getObject());
        }
    };
};

#endif
