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
#ifndef WRTDEVICEAPIS_COMMONS_REGEX_UTILS_H_
#define WRTDEVICEAPIS_COMMONS_REGEX_UTILS_H_

#include <string>

namespace WrtDeviceApis {
namespace Commons {
/**
 * Adds ^ character at the beginning in proper way
 * @param value Regexp pattern
 * @return Proper regular expression's pattern.
 */
std::string addStartPositionMark(const std::string &value);

/**
 * Adds $ character at the end in proper way
 * @param value Regexp pattern
 * @return Proper regular expression's pattern.
 */
std::string addEndPositionMark(const std::string &value);

/**
 * Adds ^ character at the beginning and $ the end in proper way
 * @param value Regexp pattern
 * @return Proper regular expression's pattern.
 */
std::string addStartAndEndPositionMark(const std::string &value);

/**
 * Converts typical string with wildcard '%' to proper perl-like regular
 * expression's pattern.
 * @param str String that came from JS.
 * @return Proper regular expression's pattern.
 * @remarks Calling it twice will (probably?) cause double escaping.
 */
std::string preparePercent(const std::string& str);

/**
 * Converts typical string with wildcard '*' to proper perl-like regular
 * expression's pattern.
 * @param str String that came from JS.
 * @return Proper regular expression's pattern.
 * @remarks Calling it twice will (probably?) cause double escaping.
 */
std::string prepareAsterisk(const std::string& str);

/**
 * Escapes regex special characters.
 * @param str String to escape.
 * @return String with regex special characters escaped.
 */
std::string escape(const std::string& str);
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_REGEX_UTILS_H_
