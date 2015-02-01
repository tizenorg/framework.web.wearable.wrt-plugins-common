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
#ifndef WRT_DOM_EXCEPTION_FACTORY_H_
#define WRT_DOM_EXCEPTION_FACTORY_H_

#include <string>
#include <dpl/noncopyable.h>
#include <JavaScriptCore/JavaScript.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
#define DECLARE_JS_EXCEPTION_FACTORY(Class) static JSDOMExceptionFactory Class

class JSDOMExceptionFactory : public DPL::Noncopyable
{
  public:
    DECLARE_JS_EXCEPTION_FACTORY(UnknownException);
    DECLARE_JS_EXCEPTION_FACTORY(IndexSizeException);
    DECLARE_JS_EXCEPTION_FACTORY(DomstringSizeException);
    DECLARE_JS_EXCEPTION_FACTORY(HierarchyRequestException);
    DECLARE_JS_EXCEPTION_FACTORY(WrongDocumentException);
    DECLARE_JS_EXCEPTION_FACTORY(InvalidCharacterException);
    DECLARE_JS_EXCEPTION_FACTORY(NoDataAllowedException);
    DECLARE_JS_EXCEPTION_FACTORY(NoModificationAllowedException);
    DECLARE_JS_EXCEPTION_FACTORY(NotFoundException);
    DECLARE_JS_EXCEPTION_FACTORY(NotSupportedException);
    DECLARE_JS_EXCEPTION_FACTORY(InuseAttributeException);
    DECLARE_JS_EXCEPTION_FACTORY(InvalidStateException);
    DECLARE_JS_EXCEPTION_FACTORY(SyntaxException);
    DECLARE_JS_EXCEPTION_FACTORY(InvalidModificationException);
    DECLARE_JS_EXCEPTION_FACTORY(NamespaceException);
    DECLARE_JS_EXCEPTION_FACTORY(InvalidAccessException);
    DECLARE_JS_EXCEPTION_FACTORY(ValidationException);
    DECLARE_JS_EXCEPTION_FACTORY(TypeMismatchException);
    DECLARE_JS_EXCEPTION_FACTORY(SecurityException);
    DECLARE_JS_EXCEPTION_FACTORY(NetworkException);
    DECLARE_JS_EXCEPTION_FACTORY(AbortException);
    DECLARE_JS_EXCEPTION_FACTORY(TimeoutException);
    DECLARE_JS_EXCEPTION_FACTORY(InvalidValuesException);
    DECLARE_JS_EXCEPTION_FACTORY(IOException);
    DECLARE_JS_EXCEPTION_FACTORY(QuotaExceededException);

  public:
    explicit JSDOMExceptionFactory(int code);

    /**
     * Creates exception object.
     * @param context JS context of the exception.
     * @param[out] exception Result object to which exception will be assigned.
     * @return JS undefined value.
     * @code
     * JSValueRef JSClass::getProperty(JSContextRef context,
     *                                 JSObjectRef object,
     *                                 JSStringRef propertyName,
     *                                 JSValueRef* exception)
     * {
     * ...
     *   return JSExceptionFactory::InvalidArgumentException.make(context,
     * exception);
     * ...
     * }
     * @endcode
     */
    JSValueRef make(JSContextRef context,
                    JSValueRef* exception,
                    const std::string& message = std::string());

    /**
     * Creates exception object.
     * @param context JS context of the exception.
     * @return JS exception object.
     * @code
     * JSObjectRef jsError =
     * JSExceptionFactory::InvalidArgumentException.make(context);
     * @endcode
     */
    JSObjectRef make(JSContextRef context,
                     const std::string& message = std::string());

  private:
    int m_code;
};
} // CommonsJavaScript
} // WrtDeviceApis

#endif //
