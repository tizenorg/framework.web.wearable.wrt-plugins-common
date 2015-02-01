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
#include <sstream>
#include <limits>
#include <dpl/scoped_array.h>
#include <dpl/scoped_free.h>
#include "ScopedJSStringRef.h"
#include "Converter.h"
#include <dpl/log/log.h>

using namespace WrtDeviceApis::Commons;

namespace WrtDeviceApis {
namespace CommonsJavaScript {
Converter::Converter(JSContextRef context) : m_context(context)
{}

Converter::~Converter()
{}

JSObjectRef Converter::toJSObjectRef(const JSValueRef& arg)
{
    if (!arg) {
        Throw(NullPointerException);
    }
    JSValueRef exception = NULL;
    JSObjectRef result = JSValueToObject(m_context, arg, &exception);
    if ((NULL == result) || (NULL != exception)) {
        ThrowMsg(ConversionException, "Couldn't convert to a JS Object.");
    }
    return result;
}

bool Converter::toBool(const JSValueRef& arg)
{
    return JSValueToBoolean(m_context, arg);
}

unsigned char Converter::toUChar(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<unsigned char>(static_cast<unsigned long>(static_cast<long long>(tmp))));
}

char Converter::toChar(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<char>(static_cast<long>(static_cast<long long>(tmp))));
}

int Converter::toInt(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<int>(static_cast<long long>(tmp)));
}

int Converter::toInt(const std::string &arg)
{
    return static_cast<int>(static_cast<long long>(toNumber_(arg)));
}

long Converter::toLong(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<long>(static_cast<long long>(tmp)));
}

long long Converter::toLongLong(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<long long>(tmp));
}

unsigned long Converter::toULong(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<unsigned long>(static_cast<long long>(tmp)));
}

std::size_t Converter::toSizeT(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<std::size_t>(tmp));
}

std::size_t Converter::toSizeT(const JSStringRef& arg)
{
    return static_cast<std::size_t>(toNumber_(arg));
}

double Converter::toDouble(const JSValueRef& arg)
{
    return toNumber_(arg);
}

std::string Converter::toString(int arg)
{
    return toString_(arg);
}

std::string Converter::toString(unsigned long arg)
{
    return toString_(arg);
}

std::string Converter::toString(long arg)
{
    return toString_(arg);
}

std::string Converter::toString(std::size_t arg)
{
    return toString_(arg);
}

std::string Converter::toString(const JSValueRef& arg)
{
    return toString_(arg);
}

std::string Converter::tryString(const JSValueRef& arg)
{
    if (!JSValueIsString(m_context, arg) &&
        !JSValueIsNumber(m_context, arg) &&
        !JSValueIsBoolean(m_context, arg))
    {
        ThrowMsg(ConversionException,
                 "Argument is not convertable to JS String.");
    }

    return toString_(arg);
}

std::string Converter::toDateString(const JSValueRef& arg)
{
    if (!JSValueIsObject(m_context, arg)) {
        ThrowMsg(ConversionException, "Argument is not JS Date.");
    }

    return toString_(arg);
}

std::string Converter::toString(const JSStringRef& arg)
{
    std::string result;
    size_t jsSize = JSStringGetMaximumUTF8CStringSize(arg);
    if (jsSize > 0) {
        jsSize = jsSize + 1;
        DPL::ScopedArray<char> buffer(new char[jsSize]);
        size_t written = JSStringGetUTF8CString(arg, buffer.Get(), jsSize);
        if (written > jsSize) {
            ThrowMsg(ConversionException,
                     "Conversion could not be fully performed.");
        }
        result = buffer.Get();
    }

    return result;
}

time_t Converter::toDateTimeT(const JSValueRef& arg)
{
    struct tm tm = toDateTm(arg);
    return mktime(&tm);
}

tm Converter::toDateTm(const JSValueRef& arg)
{
    std::string stringDate = toDateString(arg);
    struct tm result;
    char* currentLocale = setlocale(LC_TIME, NULL);
    if (currentLocale == NULL) {
        ThrowMsg(ConversionException, "Couldn't get current locale.");
    }
    DPL::ScopedFree<char> currentLocaleCopy(strdup(currentLocale));
    if (setlocale(LC_TIME, "C") == NULL) {
        ThrowMsg(ConversionException, "Couldn't set POSIX locale.");
    }
    if (strptime(stringDate.c_str(), "%a %b %d %Y %T", &result) == NULL) {
        ThrowMsg(ConversionException, "Couldn't convert supplied date.");
    }
    if (setlocale(LC_TIME, currentLocaleCopy.Get()) == NULL) {
        ThrowMsg(ConversionException, "Couldn't set previous locale back.");
    }
    //strptime function doesn't affect tm_isdst flag.
    //It may cause -1hr conversion error. Setting it to -1 informs that
    //there is no information about daylight saving time.
    result.tm_isdst = -1;
    return result;
}

std::vector<unsigned char> Converter::toVectorOfUChars(const JSValueRef& arg)
{
    return toVectorOfT_(arg, &Converter::toUChar);
}

std::vector<char> Converter::toVectorOfChars(const JSValueRef& arg)
{
    return toVectorOfT_(arg, &Converter::toChar);
}

std::vector<int> Converter::toVectorOfInts(const JSValueRef& arg)
{
    return toVectorOfT_(arg, &Converter::toInt);
}

std::vector<std::string> Converter::toVectorOfStrings(const JSValueRef& arg)
{
    return toVectorOfT_(arg, &Converter::toString);
}

std::vector<time_t> Converter::toVectorOfTimeT(const JSValueRef& arg)
{
    return toVectorOfT_(arg, &Converter::toDateTimeT);
}

JSValueRef Converter::toJSValueRef(bool arg)
{
    return JSValueMakeBoolean(m_context, arg);
}

JSValueRef Converter::toJSValueRef(int arg)
{
    return JSValueMakeNumber(m_context, arg);
}

JSValueRef Converter::toJSValueRef(unsigned int arg)
{
    return JSValueMakeNumber(m_context, arg);
}

JSValueRef Converter::toJSValueRef(unsigned long arg)
{
    return JSValueMakeNumber(m_context, arg);
}

JSValueRef Converter::toJSValueRefLong(const long arg)
{
    return JSValueMakeNumber(m_context, arg);
}

JSValueRef Converter::toJSValueRef(long long int arg)
{
    return JSValueMakeNumber(m_context, arg);
}

JSValueRef Converter::toJSValueRef(double arg)
{
    return JSValueMakeNumber(m_context, arg);
}

JSValueRef Converter::toJSValueRef(const std::string& arg)
{
    JSValueRef result = NULL;
    JSStringRef jsString = JSStringCreateWithUTF8CString(arg.c_str());
    result = JSValueMakeString(m_context, jsString);
    JSStringRelease(jsString);
    return result;
}

JSValueRef Converter::toJSValueRef(const tm& arg)
{
    JSValueRef args[6];
    args[0] = JSValueMakeNumber(m_context, arg.tm_year + 1900);
    args[1] = JSValueMakeNumber(m_context, arg.tm_mon);
    args[2] = JSValueMakeNumber(m_context, arg.tm_mday);
    args[3] = JSValueMakeNumber(m_context, arg.tm_hour);
    args[4] = JSValueMakeNumber(m_context, arg.tm_min);
    args[5] = JSValueMakeNumber(m_context, arg.tm_sec);

    JSValueRef exception = NULL;
    JSObjectRef result = JSObjectMakeDate(m_context, 6, args, &exception);
    if (NULL != exception) {
        ThrowMsg(ConversionException, "Couldn't convert to a JS Date.");
    }

    return result;
}

JSValueRef Converter::toJSValueRef(const time_t arg)
{
    struct tm tminfo;
    memset(&tminfo,0,sizeof(tminfo));
    localtime_r(&arg, &tminfo);
    return toJSValueRef(tminfo);
}

JSValueRef Converter::toJSValueRef(const std::vector<std::string>& arg)
{
    return toJSValueRef_(arg);
}

JSValueRef Converter::toJSValueRef(const char* arg)
{
    return toJSValueRef(std::string(arg));
}

JSValueRef Converter::toJSValueRef(const std::vector<int>& arg)
{
    return toJSValueRef_(arg);
}

JSValueRef Converter::toJSValueRef(const std::vector<time_t>& arg)
{
    return toJSValueRef_(arg);
}

JSValueRef Converter::toJSValueRef(JSValueRef arg)
{
    return arg;
}

JSValueRef Converter::toJSValueRef(const std::vector<JSValueRef>& arg)
{
    return toJSValueRef_(arg);
}

JSStringRef Converter::toJSStringRef(const std::string& arg)
{
    return JSStringCreateWithUTF8CString(arg.c_str());
}

double Converter::toNumber_(const JSStringRef& arg)
{
    return toNumber_(toString(arg));
}

double Converter::toNumber_(const std::string& arg)
{
    double result;
    std::stringstream ss(arg);
    if (!(ss >> result)) {
        Throw(ConversionException);
    }
    return result;
}

double Converter::toNumber_(const JSValueRef& arg)
{
    JSValueRef exception = NULL;
    double result = JSValueToNumber(m_context, arg, &exception);
    if (NULL != exception) {
        ThrowMsg(ConversionException, "Couldn't convert to a number.");
    }
    return result;
}

std::string Converter::toString_(const JSValueRef& arg)
{
    std::string result;

    JSValueRef exception = NULL;
    JSStringRef str = JSValueToStringCopy(m_context, arg, &exception);
    if (NULL == str) {
        ThrowMsg(ConversionException, "Couldn't cast to a string.");
    }

    size_t jsSize = JSStringGetMaximumUTF8CStringSize(str);
    if (jsSize > 0) {
        DPL::ScopedArray<char> buffer(new char[jsSize]);
        JSStringGetUTF8CString(str, buffer.Get(), jsSize);
        result = buffer.Get();
    }
    JSStringRelease(str);
    return result;
}

bool Converter::isNan(double value) const
{
    // This should work unless -ffast-math (GCC) option is used.
    STATIC_CHECK(std::numeric_limits<double>::is_iec559,
                 NOT_IEEE754_REPRESENTATION);
    return (value != value);
}
} // CommonsJavaScript
} // WrtDeviceApis

