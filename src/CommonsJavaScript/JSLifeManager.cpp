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
#include "JSLifeManager.h"

#include <dlog.h>

#include <map>
#include <list>
#include <algorithm>

// import temporarily from JSContextRefPrivate.h
extern "C" JSGlobalContextRef JSContextGetGlobalContext(JSContextRef ctx);

namespace WrtDeviceApis {
namespace CommonsJavaScript {

class JSLifeManagerImpl{
    friend class JSLifeManager;
    protected:
        JSLifeManagerImpl();
        ~JSLifeManagerImpl();
        void protect(JSContextRef ctx, JSValueRef value);
        void unprotect(JSContextRef ctx, JSValueRef value);
        void releaseContext(JSContextRef ctx);
        void addGlobalContext(JSContextRef ctx);
        bool isAvailableContext(JSContextRef ctx);
    private:
        JSLifeManagerImpl(const JSLifeManager&) = delete;
        JSLifeManagerImpl& operator=(const JSLifeManager&) = delete;
        typedef std::map<JSValueRef,int> RefCountT;
        std::map<JSContextRef,RefCountT> mRefCount;
        std::list<JSContextRef> validContextList_;
};

JSLifeManagerImpl::JSLifeManagerImpl(){
}

JSLifeManagerImpl::~JSLifeManagerImpl(){
}

void JSLifeManagerImpl::protect( JSContextRef ctx, JSValueRef value ){
    JSGlobalContextRef global = JSContextGetGlobalContext(ctx);
    RefCountT &ref = mRefCount[global];
    RefCountT::iterator itr = ref.find(value);
    if( itr == ref.end()){
        LOGE("protect %p", value);
        JSValueProtect(ctx, value);
        ref.insert( std::make_pair( value, 1));
    }else
        itr->second++;
}

void JSLifeManagerImpl::unprotect( JSContextRef ctx, JSValueRef value ){
    LOGE("JSLifeManagerImpl::unprotect");
    if (!isAvailableContext(ctx)) {
        return;
    }
    JSGlobalContextRef global = JSContextGetGlobalContext(ctx);
    RefCountT &ref = mRefCount[global];
    RefCountT::iterator itr = ref.find(value);
    if( itr == ref.end()){
        return;
    }
    itr->second--;
    if( itr->second == 0 ){
        LOGE("unprotect %p", value);
        JSValueUnprotect(ctx, value);
        ref.erase(itr);
    }
}

void JSLifeManagerImpl::addGlobalContext(JSContextRef ctx) {
  JSGlobalContextRef global = JSContextGetGlobalContext(ctx);
  validContextList_.push_back(global);
}

bool JSLifeManagerImpl::isAvailableContext(JSContextRef ctx) {
  auto found = std::find(validContextList_.begin(),
                         validContextList_.end(),
                         ctx);
  return found != validContextList_.end();
}

void JSLifeManagerImpl::releaseContext( JSContextRef ctx ){
    JSGlobalContextRef global = JSContextGetGlobalContext(ctx);
    RefCountT &ref = mRefCount[global];
    RefCountT::iterator itr = ref.begin();
    while( itr != ref.end() ){
        LOGE("unprotect %p", itr->first);
        JSValueUnprotect( ctx, itr->first );
        ++itr;
    }
    mRefCount.erase(global);
    validContextList_.remove(global);
}

JSLifeManager& JSLifeManager::GetInstance(){
    static JSLifeManager instance;
    return instance;
}

JSLifeManager::JSLifeManager():mImpl(new JSLifeManagerImpl){

}

JSLifeManager::~JSLifeManager(){
    delete mImpl;
}

void JSLifeManager::JSValueProtect(JSContextRef ctx, JSValueRef value){
    mImpl->protect(ctx,value);
}

void JSLifeManager::JSValueUnprotect(JSContextRef ctx, JSValueRef value){
    mImpl->unprotect(ctx, value);
}

void JSLifeManager::ReleaseContext(JSContextRef ctx){
    mImpl->releaseContext(ctx);
}

void JSLifeManager::AddGlobalContext(JSContextRef ctx) {
  mImpl->addGlobalContext(ctx);
}

bool JSLifeManager::IsAvailableContext(JSContextRef ctx) {
  return mImpl->isAvailableContext(ctx);
}

} // CommonsJavaScript
} // WrtDeviceApis
