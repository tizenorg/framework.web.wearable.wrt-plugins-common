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
 * @file    plugin_iframe_support.cpp
 * @version 1.0
 * @brief
 */

#include "plugin_iframe_support.h"

#include <algorithm>
#include <dpl/foreach.h>
#include <dpl/log/secure_log.h>

void IframesSupport::registerDeclaration(
    const JSObjectDeclarationPtr& declaration)
{
    _D("Registration iframes-supported plugins %s", declaration->getName().c_str());

    if (declaration->getParentName().find('.') != std::string::npos) {
        _E("The object will not be loaded to iframes");
        return;
    }
    m_iframesObjects.push_back(declaration);
}

void IframesSupport::registerIframe(const JSObjectPtr& iframe)
{
    _D("LoadedIframes size: %d", m_loadedIframes.size() );

    m_loadedIframes.insert(iframe);
}

void IframesSupport::unregisterIframe(const JSObjectPtr& iframe)
{
    _D("LoadedIframes size: %d", m_loadedIframes.size() );

    auto it_loaded = std::find_if(m_loadedIframes.begin(),
                                  m_loadedIframes.end(),
                                  std::bind2nd(EqualToJSObjectPtr(), iframe));
    //object not found, so thats the new iframe
    if (it_loaded == m_loadedIframes.end()) {
        _D("Nothing to unregister");
        return;
    }

    m_loadedIframes.erase(it_loaded);
}

bool IframesSupport::hasIframes() const
{
    return !m_loadedIframes.empty();
}

IframesSupport::DeclarationsList IframesSupport::getIframeObjects() const
{
    return m_iframesObjects;
}

void IframesSupport::clean()
{
    m_iframesObjects.clear();
    m_loadedIframes.clear();
}
