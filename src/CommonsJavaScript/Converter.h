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
#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_CONVERTER_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_CONVERTER_H_

#include <ctime>
#include <cstddef>
#include <string>
#include <vector>
#include <list>
#include <JavaScriptCore/JavaScript.h>
#include <dpl/noncopyable.h>
#include <dpl/shared_ptr.h>
#include <Commons/Exception.h>
#include <Commons/StaticAssert.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
class Converter : private DPL::Noncopyable
{
  public:
    explicit Converter(JSContextRef context);
    virtual ~Converter();

    /**
     * Converts JSValueRef to JSObjectRef.
     * @param arg JSValueRef object to convert.
     * @return JSObjectRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSObjectRef toJSObjectRef(const JSValueRef& arg);

    /**
     * Converts JSValueRef object to boolean value.
     * @param arg JSValueRef object to convert.
     * @return Boolean value.
     * @throw ConversionException Thrown when conversion fails.
     */
    bool toBool(const JSValueRef& arg);

    /**
     * Converts JSValueRef object to unsigned char value.
     * @param arg JSValueRef object to convert.
     * @return Unsigned char value.
     * @throw ConversionException Thrown when conversion fails.
     */
    unsigned char toUChar(const JSValueRef& arg);

    /**
     * Converts JSValueRef object to char value.
     * @param arg JSValueRef object to convert.
     * @return Char value.
     * @throw ConversionException Thrown when conversion fails.
     */
    char toChar(const JSValueRef& arg);

    /**
     * Converts JSValueRef to integer value.
     * @param arg JSValueRef object to convert.
     * @return Integer value.
     * @throw ConversionException Thrown when conversion fails.
     */
    int toInt(const JSValueRef& arg);

    /**
     * Converts string to integer value.
     * @param arg string to convert.
     * @return Integer value.
     * @throw ConversionException Thrown when conversion fails.
     */
    int toInt(const std::string& arg);

    /**
     * Converts JSvalueRef to long integer value.
     * @param arg JSValueRef object to convert.
     * @return Long integer value.
     * @throw ConversionException Thrown when conversion fails.
     */
    long toLong(const JSValueRef& arg);

    /**
     * Converts JSvalueRef to long long integer value.
     * @param arg JSValueRef object to convert.
     * @return Long long integer value.
     * @throw ConversionException Thrown when conversion fails.
     */
    long long toLongLong(const JSValueRef& arg);
    /**
     * Converts JSValueRef to unsigned long value.
     * @param arg JSValueRef object to convert.
     * @return Unsigned long value.
     * @throw ConversionException Thrown when conversion fails.
     */
    unsigned long toULong(const JSValueRef& arg);

    /**
     * Converts JSValueRef to double value.
     * @param arg JSValueRef object to convert.
     * @return Double value.
     * @throw ConversionException Thrown when conversion fails.
     */
    double toDouble(const JSValueRef& arg);

    /**
     * Converts JSValueRef to size_t value.
     * @param arg JSValueRef object to convert.
     * @return Size_t value.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::size_t toSizeT(const JSValueRef& arg);

    /**
     * Converts JSStringRef to size_t value.
     * @param arg JSStringRef object to convert.
     * @return Size_t value.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::size_t toSizeT(const JSStringRef& arg);

    /**
     * Converts int to STL string.
     * @param arg int to convert.
     * @return STL string.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::string toString(int arg);

    std::string toString(unsigned long arg);

    std::string toString(long arg);

    std::string toString(std::size_t arg);

    /**
     * Converts JSValueRef to STL string.
     * @param arg JSValueRef to convert.
     * @return STL string.
     * @throw ConversionException Thrown when conversion fails.
     * @remarks If one wants to convert JS Date object to a string then
     *          toDateString() should be used.
     */
    std::string toString(const JSValueRef& arg);

    /**
     * Converts JSStringRef to STL string.
     * @param arg JSStringRef to convert.
     * @return STL string.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::string toString(const JSStringRef& arg);

    /**
     * Converts JSSValueRef to STL string if
     * arg is one of: String, Number, Bool
     * @param arg JSValueReg to convert.
     * @return STL string.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::string tryString(const JSValueRef& arg);

    /**
     * Converts JS Date object to STL string.
     * @param arg JS Date object to convert.
     * @return STL string.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::string toDateString(const JSValueRef& arg);

    /**
     * Converts JSValueRef to time_t value.
     * @param arg JSValueRef to convert.
     * @return time_t value.
     * @throw ConversionException Thrown when conversion fails.
     */
    time_t toDateTimeT(const JSValueRef& arg);

    /**
     * Converts JSValueRef to tm struct.
     * @param arg JSValueRef to convert.
     * @return tm struct.
     * @throw ConversionException Thrown when conversion fails.
     */
    tm toDateTm(const JSValueRef& arg);

    /**
     * Converts JSValueRef to vector<unsigned char>.
     * @param arg JSValueRef to convert.
     * @return vector<unsigned char>.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::vector<unsigned char> toVectorOfUChars(const JSValueRef& arg);

    /**
     * Converts JSValueRef to vector<char>.
     * @param arg JSValueRef to convert.
     * @return vector<char>.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::vector<char> toVectorOfChars(const JSValueRef& arg);

    /**
     * Converts JSValueRef to vector<int>.
     * @param arg JSValueRef to convert.
     * @return vector<int>.
     * @throw ConversionException Thrown when conversion fails.
     */
    std::vector<int> toVectorOfInts(const JSValueRef& arg);

    /**
     * Converts JSValueRef to vector of string.
     * @param arg JSValueRef to convert.
     * @return std::vector<std::string>
     * @throw ConversionException Thrown when conversion fails.
     */
    std::vector<std::string> toVectorOfStrings(const JSValueRef& arg);

    /**
     * Converts JSValueRef to vector of time_t.
     * @param arg JSValueRef to convert.
     * @return std::vector<std::time_t>
     * @throw ConversionException Thrown when conversion fails.
     */
    std::vector<std::time_t> toVectorOfTimeT(const JSValueRef& arg);

    /**
     * Converts boolean value to JSValueRef.
     * @param arg Boolean value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(bool arg);

    /**
     * Converts integer value to JSValueRef.
     * @param arg Integer value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(int arg);

    /**
     * Converts unsigned integer value to JSValueRef.
     * @param arg Unsigned integer value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(unsigned int arg);

    /**
     * Converts double value to JSValueRef.
     * @param arg Double value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(double arg);

    /**
     * Converts unsigned long value to JSValueRef.
     * @param arg unsigned long value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(unsigned long arg);

    /**
     * Converts long value to JSValueRef.
     * @param arg long value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRefLong(const long arg);

    /**
     * Converts unsigned long value to JSValueRef.
     * @param arg long long int value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(long long int arg);

    /**
     * Converts STL string to JSValueRef.
     * @param arg STL string to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const std::string& arg);

    /**
     * Converts char sequence to JSValueRef.
     * @param arg char sequence to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const char* arg);

    /**
     * Converts time_t value to JSValueRef.
     * @param arg time_t value to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const time_t arg);

    /**
     * Converts tm struct to JSValueRef.
     * @param arg tm struct to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const tm& arg);

    /**
     * Converts STL vector of integer to JSValueRef.
     * @param arg STL vector of integer to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const std::vector<int>& arg);

    /**
     * Converts STL vector of time_t to JSValueRef.
     * @param arg STL vector of time_t to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const std::vector<time_t>& arg);

    /**
     * Converts STL vector of string to JSStringRef.
     * @param arg STL string to convert.
     * @return JSValueRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const std::vector<std::string>& arg);

    /**
     * Converts JSValueRef to JSValueRef
     * @param JSValueRef js value which is returned
     * @return JSValueRef - parameter
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(JSValueRef arg);

    /**
     * Converts std::vector to JSValueRef
     * @param arg vecotr of JSValueRefs
     * @return JSValueRef - js array
     * @throw ConversionException Thrown when conversion fails.
     */
    JSValueRef toJSValueRef(const std::vector<JSValueRef>& arg);

    /**
     * Template specialization to catch errors caused
     * by wrong usage of general template or
     * implicit conversions from SharedPtr to
     * specialized type
     * */
    template<class Class>
    JSValueRef toJSValueRef(const DPL::SharedPtr<Class>& c)
    {
        STATIC_ERROR(ERROR_INVALID_IMPLICIT_CONVERSION, c)
        //above macro causes compilation error
        return static_cast<JSValueRef>(NULL);
    }

    /**
     * Converts STL string to JSStringRef.
     * @param arg STL string to convert.
     * @return JSStringRef object.
     * @throw ConversionException Thrown when conversion fails.
     */
    JSStringRef toJSStringRef(const std::string& arg);

  protected:
    bool isNan(double value) const;

    double toNumber_(const JSValueRef& arg);

    double toNumber_(const JSStringRef& arg);

    double toNumber_(const std::string& arg);

    template<typename T, class Derived>
    std::vector<T> toVectorOfT_(const JSValueRef& arg,
                                T (Derived::*f)(const JSValueRef &),
                                Derived* object)
    {
        if (JSValueIsNull(m_context,
                          arg) || JSValueIsUndefined(m_context, arg))
        {
            return std::vector<T>();
        }

        if (!JSIsArrayValue(m_context, arg)) {
            ThrowMsg(Commons::ConversionException,
                     "Argument is not an JS array.");
        }

        std::vector<T> result;
        JSObjectRef objArg = toJSObjectRef(arg);
        for (std::size_t i = 0; i < JSGetArrayLength(m_context, objArg); ++i) {
            JSValueRef element = JSGetArrayElement(m_context, objArg, i);
            result.push_back((object->*f)(element));
        }
        return result;
    }

    template<typename T>
    std::vector<T> toVectorOfT_(const JSValueRef& arg,
                                T (Converter::*f)(const JSValueRef &))
    {
        if (JSValueIsNull(m_context,
                          arg) || JSValueIsUndefined(m_context, arg))
        {
            return std::vector<T>();
        }

        if (!JSIsArrayValue(m_context, arg)) {
            ThrowMsg(Commons::ConversionException,
                     "Argument is not an JS array.");
        }

        std::vector<T> result;
        JSObjectRef objArg = toJSObjectRef(arg);
        for (std::size_t i = 0; i < JSGetArrayLength(m_context, objArg); ++i) {
            JSValueRef element = JSGetArrayElement(m_context, objArg, i);
            result.push_back((this->*f)(element));
        }
        return result;
    }

    template<class T, class Derived>
    JSValueRef toJSValueRef_(const std::vector<T>& arg,
                             JSValueRef (Derived::*f)(const T &),
                             Derived* object)
    {
        JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);

        if (NULL == jsResult) {
            ThrowMsg(WrtDeviceApis::Commons::NullPointerException,
                     "Could not create js array object");
        }

        for (std::size_t i = 0; i < arg.size(); ++i) {
            JSValueRef tmpVal = (object->*f)(arg[i]);
            if (!JSSetArrayElement(m_context, jsResult, i, tmpVal)) {
                ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                         "Could not insert value into js array");
            }
        }

        return jsResult;
    }

    template<class T>
    JSValueRef toJSValueRef_(const std::vector<T>& arg)
    {
        JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);

        if (NULL == jsResult) {
            ThrowMsg(WrtDeviceApis::Commons::NullPointerException,
                     "Could not create js array object");
        }

        for (std::size_t i = 0; i < arg.size(); ++i) {
            JSValueRef tmpVal = toJSValueRef(arg[i]);
            if (!JSSetArrayElement(m_context, jsResult, i, tmpVal)) {
                ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                         "Could not insert value into js array");
            }
        }

        return jsResult;
    }

    template<class T>
    JSValueRef toJSValueRef_(const std::list<T>& arg)
    {
        JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);

        if (NULL == jsResult) {
            ThrowMsg(WrtDeviceApis::Commons::NullPointerException,
                     "Could not create js array object");
        }

        typename std::list<T>::const_iterator it = arg.begin();
        for (std::size_t i = 0; it != arg.end(); ++i, ++it) {
            JSValueRef tmpVal = toJSValueRef(*it);
            if (!JSSetArrayElement(m_context, jsResult, i, tmpVal)) {
                ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                         "Could not insert value into js array");
            }
        }

        return jsResult;
    }

    template<class T, class Derived>
    JSValueRef toJSValueRef_(const std::list<T>& arg,
                             JSValueRef (Derived::*f)(const T &),
                             Derived* object)
    {
        JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);

        if (NULL == jsResult) {
            ThrowMsg(WrtDeviceApis::Commons::NullPointerException,
                     "Could not create js array object");
        }

        typename std::list<T>::const_iterator it = arg.begin();
        for (std::size_t i = 0; it != arg.end(); ++i, ++it) {
            JSValueRef tmpVal = (object->*f)(*it);
            if (!JSSetArrayElement(m_context, jsResult, i, tmpVal)) {
                ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                         "Could not insert value into js array");
            }
        }

        return jsResult;
    }

    std::string toString_(const JSValueRef& arg);

    template<typename T>
    std::string toString_(const T& arg)
    {
        std::stringstream ss;
        if (!(ss << arg)) {
            ThrowMsg(WrtDeviceApis::Commons::ConversionException,
                     "Could not convert to string.");
        }
        return ss.str();
    }

  protected:
    JSContextRef m_context;
};

template<class C>
class ConverterFactory : private DPL::Noncopyable
{
  public:
    /**
     * Converter type which deletes itself when gets out of scope.
     */
    typedef DPL::SharedPtr<C> ConverterType;

  public:
    /**
     * Gets converter object.
     * @param context JavaScript context the conversion will be performed in.
     * @param[out] exception JavaScript value for storing exception.
     * @return Converter object.
     */
    static ConverterType getConverter(JSContextRef context)
    {
        C* convert = new C(context);
        return ConverterType(convert);
    }

  private:
    ConverterFactory();
};

typedef ConverterFactory<Converter> BasicConverterFactory;

typedef BasicConverterFactory::ConverterType BasicConverter;
} // CommonsJavaScript
} // WrtDeviceApis

#endif /* WRTDEVICEAPIS_COMMONSJAVASCRIPT_CONVERTER_H_ */
