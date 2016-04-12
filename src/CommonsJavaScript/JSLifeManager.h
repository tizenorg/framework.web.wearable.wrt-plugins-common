/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef JS_LIFE_MANAGER_H_
#define JS_LIFE_MANAGER_H_

#include <JavaScriptCore/JavaScript.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {

class JSLifeManagerImpl;
class JSLifeManager{
    public:
        void JSValueProtect(JSContextRef ctx, JSValueRef value);
        void JSValueUnprotect(JSContextRef ctx, JSValueRef value);
        void ReleaseContext(JSContextRef ctx);
        void AddGlobalContext(JSContextRef ctx);
        bool IsAvailableContext(JSContextRef ctx);
        static JSLifeManager& GetInstance();
    private:
        JSLifeManager();
        ~JSLifeManager();
        JSLifeManager(const JSLifeManager&) = delete;
        JSLifeManager& operator=(const JSLifeManager&) = delete;
        JSLifeManagerImpl *mImpl;
};
} // CommonsJavaScript
} // WrtDeviceApis


#define JSValueSafeProtect(ctx,value) \
    do {\
        WrtDeviceApis::CommonsJavaScript::JSLifeManager::GetInstance().JSValueProtect(ctx,value); \
    }while(0)


#define JSValueSafeUnprotect(ctx,value) \
    do {\
        WrtDeviceApis::CommonsJavaScript::JSLifeManager::GetInstance().JSValueUnprotect(ctx,value); \
    }while(0)


#endif
