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
 * @file        ipc_message_support.h
 * @author      Jihoon Chung (jihoon.chung@samsung.com)
 * @version     1.0
 * @brief       Implementation of IPC between plugins and UI Process
 */
#ifndef WRT_SRC_IPC_MESSAGE_SUPPORT
#define WRT_SRC_IPC_MESSAGE_SUPPORT

#include <memory>
#include <map>
#include <string>
#include <WKBundle.h>
#include <EWebKit.h>
#include <EWebKit_internal.h>

typedef void (*AsyncReplyCallback)(unsigned int, void*, const char*);

namespace IPCMessageSupport {

static const char * const REPLY_ASYNC = "ToInjectedBundle::REPLY_ASYNC";
static const char * const TIZEN_CHANGE_USERAGENT = "tizen://changeUA";
static const char * const TIZEN_DELETE_ALL_COOKIES = "tizen://deleteAllCookies";
static const char * const TIZEN_EXIT = "tizen://exit";
static const char * const TIZEN_HIDE = "tizen://hide";

class AsyncConnection
{
  public:
    AsyncReplyCallback replyCallback;
    void* data;

    AsyncConnection(AsyncReplyCallback r, void* d) :
        replyCallback(r),
        data(d)
    {
    }
};

typedef std::shared_ptr<AsyncConnection> AsyncConnectionPtr;
typedef std::map<unsigned int, AsyncConnectionPtr> AsyncConnectionMap;

class AsyncConnectionManager
{
  private:
    AsyncConnectionMap m_connectionMap;

  public:
    ~AsyncConnectionManager();

    // Singleton
    static AsyncConnectionManager &instance();

    int addConnection(AsyncConnectionPtr connection);
    bool removeConnection(int handle);
    AsyncConnectionPtr getConnection(int handle);

};

void setWKBundleRef(WKBundleRef bundleRef);
void setXwindowHandle(unsigned int handle);
const char* sendMessageToUiProcess(const char* name, const char* body);
int  sendAsyncMessageToUiProcess(const char* name, const char* body, AsyncReplyCallback replyCallback, void* data);
void* ignoreAsyncMessageReply(int handle);
void replyAsyncMessageToWebProcess(Ewk_Context* ewkContext, int handle, const char* body);
std::string sendSyncMessageToUiProcess(const char* name, const char* body);
}

#endif // WRT_SRC_IPC_MESSAGE_SUPPORT
