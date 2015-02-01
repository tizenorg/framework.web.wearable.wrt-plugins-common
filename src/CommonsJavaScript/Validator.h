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
#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_VALIDATOR_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_VALIDATOR_H_

#include <ctime>
#include <string>
#include <vector>
#include <JavaScriptCore/JavaScript.h>
#include <dpl/noncopyable.h>
#include <dpl/shared_ptr.h>
#include <Commons/Exception.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
class Validator : private DPL::Noncopyable
{
  public:
    explicit Validator(JSContextRef context,
                       JSValueRef* exception = NULL);
    virtual ~Validator();

    /**
     * Checks if arg is a Date.
     * @param arg JSValueRef to check.
     * @return true when object is a date, false otherwise
     */
    bool isDate(const JSValueRef& arg);

    /**
     * Checks if arg is a callback function.
     * @param arg JSValueRef to check.
     * @return true when object is a callback function, false otherwise
     */
    bool isCallback(const JSValueRef& arg);

    /**
     * Checks if arg have allowed properties.
     * @param allowed allowed property names.
     * @param arg JSValueRef to check.
     * @return true when object's properties are subset of allowed
     */
    bool checkArrayKeys(const std::vector<std::string> &allowed,
                        JSValueRef argument);

    bool isNullOrUndefined(const JSValueRef& arg);

  protected:
    JSContextRef m_context;
    JSValueRef* m_exception;
};

template<class C>
class ValidatorFactory : private DPL::Noncopyable
{
  public:
    /**
     * Validator type which deletes itself when gets out of scope.
     */
    typedef DPL::SharedPtr<C> ValidatorType;

  public:
    /**
     * Gets converter object.
     * @param context JavaScript context the conversion will be performed in.
     * @param[out] exception JavaScript value for storing exception.
     * @return Converter object.
     */
    static ValidatorType getValidator(JSContextRef context,
                                      JSValueRef* exception = NULL)
    {
        C* convert = new C(context, exception);
        return ValidatorType(convert);
    }

  private:
    ValidatorFactory();
};

typedef ValidatorFactory<Validator> BasicValidatorFactory;
typedef BasicValidatorFactory::ValidatorType BasicValidator;
} // CommonsJavaScript
} // WrtDeviceApis

#endif /* _VALIDATOR_H_ */
