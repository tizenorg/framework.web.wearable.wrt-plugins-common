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
 * @file   StringUtils.h
 * @author Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef WRTDEVICEAPIS_COMMONS_STRINGUTILS_H_
#define WRTDEVICEAPIS_COMMONS_STRINGUTILS_H_

#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <Commons/Exception.h>
#include <Commons/Deprecated.h>

namespace WrtDeviceApis {
namespace Commons {
namespace String {
/**
 * Removes spaces from begining of the string.
 * @param str String to remove spaces from.
 * @return String with spaces removed.
 */
std::string lTrim(const std::string& str);

/**
 * Removes spaces from end of the string.
 * @param str String to remove spaces from.
 * @return String with spaces removed.
 */
std::string rTrim(const std::string& str);

/**
 * Removes spaces from begining and end of the string.
 * @param str String to remove spaces from.
 * @return String with spaces removed.
 */
std::string trim(const std::string& str);

/**
 * Checks if str starts with startStr
 * @param str String to be searched.
 * @param startStr String to be found at the beginning of str.
 * @return true when str starts with startStr, false otherwise.
 */
bool startsWith(const std::string& str, const std::string& startStr);

/**
 * Checks if str ends with startStr
 * @param str String to be searched.
 * @param endStr String to be found at the end of str.
 * @return true when str starts with startStr, false otherwise.
 */
bool endsWith(const std::string& str, const std::string& endStr);

/**
 * Merges consecutive groups of duplicate characters into one.
 * @param str String to remove characters from.
 * @param needle Character for which groups of duplicates to merge.
 * @return String with group of duplicate characters merged.
 * @code
 * ...
 * std::string str = "Aala maa kota";
 * std::string uniq = unique(str, 'a');
 * uniq == "Ala ma kota";
 * ...
 * @endcode
 */
std::string unique(const std::string& str, const char needle);

/**
 * Checks if supplied string exists in specified array of c-strings.
 * @param needle String to verify.
 * @param stack Array of c-strings which last element should be NULL.
 * @return True if string exists in array, false otherwise.
 * @remarks Last element of array should be NULL, otherwise there is no
 *          way to calculate its number of elements.
 */
bool inCStringArray(const std::string& needle, const char* stack[]);

/**
 * Duplicates a string.
 * @param str String to duplicate.
 * @return String copy.
 * @throw std::bad_alloc If allocation fails.
 * @remarks Ownership passed to the caller. String has to be deallocated using
 *          free() function.
 */
char* strdup(const char* str);
inline char* strdup(const std::string& str)
{
    return strdup(str.c_str());
}

std::string merge(const std::vector<std::string>& strs,
                  std::string::value_type delim);

std::vector<std::string> split(const std::string& str,
                               std::string::value_type delim);

/**
 * @deprecated Use convertTo<int>().
 */
int toInt(const std::string& str) WRT_PLUGINS_DEPRECATED;

/**
 * Converts string to specified type.
 *
 * @tparam T Type to convert to.
 * @param str String to convert.
 * @return Converted value.
 * @throw ConversionException If conversion fails.
 *
 * @remarks T must implement default constructor and an implementation
 *          of input string operator for T must exist.
 */
template<typename T>
T convertTo(const std::string& str)
{
    T result;
    std::istringstream iss(str);
    if (!(iss >> result)) {
        Throw(ConversionException);
    }
    return std::move(result);
}

/**
 * Converts a given value to string.
 *
 * @tparam T Type of the value to convert. It is deduced by the compiler.
 * @param data Value to convert.
 * @return Value converted to string.
 * @throw ConversionException If operation fails.
 *
 * @remarks Output stream operator for type T must exist.
 */
template<typename T>
std::string parse(const T& data)
{
    std::ostringstream oss;
    if (!(oss << data)) {
        Throw(ConversionException);
    }
    return oss.str();
}
}
}
}

#endif
