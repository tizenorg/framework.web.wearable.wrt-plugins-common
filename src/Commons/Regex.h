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
#ifndef WRTDEVICEAPIS_COMMONS_REGEX_H_
#define WRTDEVICEAPIS_COMMONS_REGEX_H_

#include <string>

namespace WrtDeviceApis {
namespace Commons {
const unsigned int VALIDATE_MATCH_CASELESS = 0x0001;     //use when you want to
                                                         // make caseless match
const unsigned int VALIDATE_MATCH_FULL = 0x0002;         //use when you want
                                                         // supplied text
                                                         // matches a supplied
                                                         // pattern exactly
const std::string LOWER_P = "p";
const std::string UPPER_P = "P";

/**
 * Validates value against pattern
 * @param pattern Regexp pattern
 * @param value String to validate
 * @param options Modifiers for a match.
 * @return True when value is matched against pattern
 */
bool validate(const std::string &pattern,
              const std::string &value,
              unsigned int options = 0);

/**
 * Filter value against pattern(Any character except "0-9+#*Pp" will be removed
 * from value)
 * @param pattern Regexp pattern
 * @param value String to be filtered
 * @return filtered value
 */
std::string filter(const std::string &pattern,
                   const std::string &value);

/**
 * Replace character "p" in value by "P"
 * @param value String to be replaced
 * @return replaced value
 */
std::string toUpper(const std::string &value);
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_REGEX_H_
