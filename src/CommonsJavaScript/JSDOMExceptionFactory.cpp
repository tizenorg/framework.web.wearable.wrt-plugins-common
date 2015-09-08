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
#include "JSDOMExceptionFactory.h"

#include <dpl/assert.h>
#include <dpl/log/log.h>

#include "JSUtils.h"
#include "DOMExceptionData.h"
#include "JSDOMException.h"

namespace WrtDeviceApis {
namespace CommonsJavaScript {
#define DEFINE_JS_EXCEPTION_FACTORY(Class, Code) \
    JSDOMExceptionFactory JSDOMExceptionFactory::Class(Code)

DEFINE_JS_EXCEPTION_FACTORY(UnknownException, JSDOMException::UNKNOWN_ERR);
DEFINE_JS_EXCEPTION_FACTORY(IndexSizeException, JSDOMException::INDEX_SIZE_ERR);
DEFINE_JS_EXCEPTION_FACTORY(DomstringSizeException,
                            JSDOMException::DOMSTRING_SIZE_ERR);
DEFINE_JS_EXCEPTION_FACTORY(HierarchyRequestException,
                            JSDOMException::HIERARCHY_REQUEST_ERR);
DEFINE_JS_EXCEPTION_FACTORY(WrongDocumentException,
                            JSDOMException::WRONG_DOCUMENT_ERR);
DEFINE_JS_EXCEPTION_FACTORY(InvalidCharacterException,
                            JSDOMException::INVALID_CHARACTER_ERR);
DEFINE_JS_EXCEPTION_FACTORY(NoDataAllowedException,
                            JSDOMException::NO_DATA_ALLOWED_ERR);
DEFINE_JS_EXCEPTION_FACTORY(NoModificationAllowedException,
                            JSDOMException::NO_MODIFICATION_ALLOWED_ERR);
DEFINE_JS_EXCEPTION_FACTORY(NotFoundException, JSDOMException::NOT_FOUND_ERR);
DEFINE_JS_EXCEPTION_FACTORY(NotSupportedException,
                            JSDOMException::NOT_SUPPORTED_ERR);
DEFINE_JS_EXCEPTION_FACTORY(InuseAttributeException,
                            JSDOMException::INUSE_ATTRIBUTE_ERR);
DEFINE_JS_EXCEPTION_FACTORY(InvalidStateException,
                            JSDOMException::INVALID_STATE_ERR);
DEFINE_JS_EXCEPTION_FACTORY(SyntaxException, JSDOMException::SYNTAX_ERR);
DEFINE_JS_EXCEPTION_FACTORY(InvalidModificationException,
                            JSDOMException::INVALID_MODIFICATION_ERR);
DEFINE_JS_EXCEPTION_FACTORY(NamespaceException, JSDOMException::NAMESPACE_ERR);
DEFINE_JS_EXCEPTION_FACTORY(InvalidAccessException,
                            JSDOMException::INVALID_ACCESS_ERR);
DEFINE_JS_EXCEPTION_FACTORY(ValidationException, JSDOMException::VALIDATION_ERR);
DEFINE_JS_EXCEPTION_FACTORY(TypeMismatchException,
                            JSDOMException::TYPE_MISMATCH_ERR);
DEFINE_JS_EXCEPTION_FACTORY(SecurityException, JSDOMException::SECURITY_ERR);
DEFINE_JS_EXCEPTION_FACTORY(NetworkException, JSDOMException::NETWORK_ERR);
DEFINE_JS_EXCEPTION_FACTORY(AbortException, JSDOMException::ABORT_ERR);
DEFINE_JS_EXCEPTION_FACTORY(TimeoutException, JSDOMException::TIMEOUT_ERR);
DEFINE_JS_EXCEPTION_FACTORY(InvalidValuesException,
                            JSDOMException::INVALID_VALUES_ERR);
DEFINE_JS_EXCEPTION_FACTORY(IOException, JSDOMException::IO_ERR);
DEFINE_JS_EXCEPTION_FACTORY(QuotaExceededException,
                            JSDOMException::QUOTA_EXCEEDED_ERR);

JSDOMExceptionFactory::JSDOMExceptionFactory(int code) : m_code(code)
{}

JSValueRef JSDOMExceptionFactory::make(JSContextRef context,
                                       JSValueRef* exception,
                                       const std::string& message)
{
    AssertMsg(exception, "Exception object can't be NULL.");
    JSDOMException::PrivateObject::ObjectType data(new DOMExceptionData(m_code,
                                                                        message));
    *exception = JSUtils::makeObject(context,
                                     JSDOMException::getClassRef(),
                                     data);
    return JSValueMakeUndefined(context);
}

JSObjectRef JSDOMExceptionFactory::make(JSContextRef context,
                                        const std::string& message)
{
    JSDOMException::PrivateObject::ObjectType data(new DOMExceptionData(m_code,
                                                                        message));
    return JSUtils::makeObject(context,
                               JSDOMException::getClassRef(), data);
}
} // CommonsJavaScript
} // WrtDeviceApis
