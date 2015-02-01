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
#include <cstring>
#include <algorithm>
#include <iterator>
#include <new>
#include "StringUtils.h"

namespace WrtDeviceApis {
namespace Commons {
namespace String {
std::string lTrim(const std::string& str)
{
    std::string result = str;
    result.erase(result.begin(),
                 std::find_if(result.begin(), result.end(),
                              std::not1(std::ptr_fun<int, int>(std::isspace))));
    return result;
}

std::string rTrim(const std::string& str)
{
    std::string result = str;
    result.erase(std::find_if(result.rbegin(), result.rend(),
                              std::not1(std::ptr_fun<int, int>(
                                            std::isspace))).base(), result.end());
    return result;
}

std::string trim(const std::string& str)
{
    return lTrim(rTrim(str));
}

bool startsWith(const std::string& str,
                const std::string& startStr)
{
    return str.find(startStr) == 0;
}

bool endsWith(const std::string& str,
              const std::string& endStr)
{
    std::size_t pos = str.rfind(endStr);
    if (pos == std::string::npos) {
        return false;
    }
    return pos == str.length() - endStr.length();
}

std::string unique(const std::string& str,
                   const char needle)
{
    std::string result = str;

    std::string::size_type pos = 0;
    while ((pos = result.find_first_of(needle, pos)) < std::string::npos) {
        std::string::size_type last = result.find_first_not_of(needle, pos);
        if (last == std::string::npos) {
            last = result.size();
        }
        if (last - pos > 1) {
            result.erase(pos + 1, last - pos - 1);
        }
        pos = last;
    }

    return result;
}

bool inCStringArray(const std::string& needle,
                    const char* stack[])
{
    while (*stack) {
        if (needle == *stack) {
            return true;
        }
        ++stack;
    }
    return false;
}

char* strdup(const char* str)
{
    char* result = ::strdup(str);
    if (NULL == result) {
        throw std::bad_alloc();
    }
    return result;
}

std::string merge(const std::vector<std::string>& strs,
                  std::string::value_type delim)
{
    typedef std::vector<std::string> Strings;

    std::ostringstream ss;
    if (!strs.empty()) {
        for (Strings::size_type i = 0; i < strs.size() - 1; ++i) {
            ss << strs[i] << delim;
        }
        ss << strs.at(strs.size() - 1);
    }
    return ss.str();
}

std::vector<std::string> split(const std::string& str,
                               std::string::value_type delim)
{
    std::vector<std::string> result;
    std::string::size_type begin = 0, end = 0;
    while ((end = str.find_first_of(delim, begin))) {
        std::string part = (std::string::npos == end ?
                            str.substr(begin) :
                            str.substr(begin, end - begin));
        if (!part.empty()) {
            result.push_back(part);
        }
        if (std::string::npos == end) {
            break;
        }
        begin = end + 1;
    }
    return result;
}

int toInt(const std::string& str)
{
    return convertTo<int>(str);
}
}
}
}
