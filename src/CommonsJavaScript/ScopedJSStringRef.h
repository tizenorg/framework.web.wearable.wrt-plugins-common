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
#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_SCOPEDJSSTRINGREF_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_SCOPEDJSSTRINGREF_H_

#include <JavaScriptCore/JavaScript.h>
#include <dpl/noncopyable.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
/**
 * Takes ownership over JSStringRef object to call JSSTringRelease on it
 * when this object goes out of scope.
 */
class ScopedJSStringRef : private DPL::Noncopyable
{
  public:
    /**
     * Creates this class instance.
     * @param ref JSStringRef to take ownership.
     * @remarks Takes ownership.
     */
    ScopedJSStringRef(JSStringRef ref);

    /**
     * Destroys instance.
     * @remarks Releases stored JSStringRef.
     */
    ~ScopedJSStringRef();

    /**
     * Gets stored JSStringRef.
     * @return JSStringRef object.
     * @remarks Doesn't pass ownership to caller.
     */
    JSStringRef get() const;

  private:
    JSStringRef m_ref;
};
} // CommonsJavaScript
} // WrtDeviceApis

#endif /* WRTDEVICEAPIS_COMMONSJAVASCRIPT_SCOPEDJSSTRINGREF_H_ */
