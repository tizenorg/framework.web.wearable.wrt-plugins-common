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
 * @file        ipc_message_support.cpp
 * @author      Jihoon Chung (jihoon.chung@samsung.com)
 * @version     1.0
 * @brief       Implementation of IPC between plugins and UI Process
 */
#include "ipc_message_support.h"

#include <string>
#include <sstream>
#include <WKBundle.h>
#include <WKString.h>
#include <WKType.h>
#include <dpl/log/secure_log.h>
#include <dpl/assert.h>

static WKBundleRef s_injectedBundleRef = NULL;
static unsigned  int s_xWindowHandle = 0;

namespace {
const char* const TIZEN_GET_WINDOW_HANDLE = "tizen://getWindowHandle";
const char* const TIZEN_IS_BACKGROUND_SUPPORTED = "ToUIProcess::BACKGROUND_SUPPORTED";
const char* const TIZEN_CLEAR_ALL_COOKIES = "tizen://clearAllCookies";

static void sendPostMessage(const char* name, const char* body)
{
    WKStringRef nameWKString = WKStringCreateWithUTF8CString(name);
    WKStringRef bodyWKString = NULL;
    if (body) {
        bodyWKString = WKStringCreateWithUTF8CString(body);
    }

    WKBundlePostMessage(s_injectedBundleRef,
                        nameWKString,
                        bodyWKString);
    WKRelease(nameWKString);
    if (bodyWKString) {
        WKRelease(bodyWKString);
    }
}

static int sendAsyncMessage(const char* name, const char* body, AsyncReplyCallback replyCallback, void* data)
{
    using namespace IPCMessageSupport;

    int handle = AsyncConnectionManager::instance().addConnection(AsyncConnectionPtr(new AsyncConnection(replyCallback, data)));

    std::string strBody = body;
    std::stringstream ss;

    ss << handle;
    strBody = ss.str() + "_" + strBody;

    sendPostMessage(name, strBody.c_str());

    return handle;
}

} // namespace


namespace IPCMessageSupport {

AsyncConnectionManager::~AsyncConnectionManager()
{
    m_connectionMap.clear();
}

AsyncConnectionManager &AsyncConnectionManager::instance()
{
    static AsyncConnectionManager instance;

    return instance;
}

int AsyncConnectionManager::addConnection(AsyncConnectionPtr connection)
{
    static int latestHandle = -1;
    int newHandle;

    latestHandle++;
    if (latestHandle < 0) latestHandle = 0;

    newHandle = latestHandle;

    m_connectionMap.insert(AsyncConnectionMap::value_type(newHandle, connection));

    return newHandle;
}

bool AsyncConnectionManager::removeConnection(int handle)
{
    bool ret = (m_connectionMap.erase(handle) == 1);

    return ret;
}

AsyncConnectionPtr AsyncConnectionManager::getConnection(int handle)
{
    AsyncConnectionMap::iterator iter = m_connectionMap.find(handle);

    if (iter != m_connectionMap.end())
    {
        return iter->second;
    }

    return AsyncConnectionPtr();
}


} // namespace IPCMessageSupport


void IPCMessageSupport::setWKBundleRef(WKBundleRef bundleRef)
{
    _D("called");
    s_injectedBundleRef = bundleRef;
}

void IPCMessageSupport::setXwindowHandle(unsigned int handle)
{
    _D("called");
    s_xWindowHandle = handle;
}

const char* IPCMessageSupport::sendMessageToUiProcess(
    const char* name,
    const char* body)
{
    _D("called");
    if (s_injectedBundleRef == NULL) {
        _E("UI Process information isn't set");
        return NULL;
    }
    _D("name = [%s]", name);
    if (body) {
        _D("body = [%s]", body);
    }

    if (!name) {
        return NULL;
    }

    // tizen://getWindowHandle
    if (!strcmp(name, TIZEN_GET_WINDOW_HANDLE)) {
        if (s_xWindowHandle == 0) {
            return NULL;
        } else {
            std::stringstream ss;
            ss << s_xWindowHandle;
            std::string ret  = ss.str();
            return strdup(ret.c_str());
        }
    }

    // tizen://clearAllCookies
    if (!strcmp(name, TIZEN_CLEAR_ALL_COOKIES)) {
        sendPostMessage(name, body);
        return NULL;
    }

    return NULL;
}

int IPCMessageSupport::sendAsyncMessageToUiProcess(const char* name, const char* body, AsyncReplyCallback replyCallback, void* data)
{
    _D("called");

    if (s_injectedBundleRef == NULL)
    {
        _E("UI Process information isn't set");
        return -1;
    }

    if (name == NULL)
    {
        _E("msg name is null!");
        return -1;
    }

    if (body == NULL)
    {
        body = "";
    }

    _D("name = [%s]\nbody = [%s]", name, body);

    return sendAsyncMessage(name, body, replyCallback, data);
}

void* IPCMessageSupport::ignoreAsyncMessageReply(int handle)
{
    _D("called");

    AsyncConnectionPtr connection = AsyncConnectionManager::instance().getConnection(handle);

    if (!connection)
    {
        return NULL;
    }

    AsyncConnectionManager::instance().removeConnection(handle);

    return connection->data;
}

void IPCMessageSupport::replyAsyncMessageToWebProcess(Ewk_Context* ewkContext, int handle, const char* body)
{
    _D("called");

    if (ewkContext == NULL)
    {
        return;
    }

    std::string strBody = (body) ? (body) : "";
    std::stringstream ss;
    ss << handle;

    strBody = ss.str() + "_" + strBody;

    ewk_context_message_post_to_injected_bundle(ewkContext, REPLY_ASYNC, strBody.c_str());
}

namespace {
  std::string WKStringToString(WKStringRef str)
  {
      if (WKStringIsEmpty(str)) {
          return std::string();
      } else {
          size_t size = WKStringGetMaximumUTF8CStringSize(str);
          std::unique_ptr<char []> buffer(new char[size + 1]);
          WKStringGetUTF8CString(str, buffer.get(), size + 1);
          return std::string(buffer.get());
      }
  }
}  // namespace

std::string IPCMessageSupport::sendSyncMessageToUiProcess(const char* name, const char* body) {
    _D("called : %s %s", name ? name : "", body ? body : "");

    WKStringRef nameWKString = WKStringCreateWithUTF8CString(name);
    WKStringRef bodyWKString = NULL;
    WKTypeRef retval = NULL;
    if (body) {
        bodyWKString = WKStringCreateWithUTF8CString(body);
    }

    WKBundlePostSynchronousMessage(
            s_injectedBundleRef,
            nameWKString,
            bodyWKString,
            &retval);

    WKRelease(nameWKString);
    if (bodyWKString) {
        WKRelease(bodyWKString);
    }
    if (retval != NULL) {
        std::string ret = WKStringToString(static_cast<WKStringRef>(retval));
        WKRelease(retval);
        return ret;
    } else {
        return std::string();
    }
}
