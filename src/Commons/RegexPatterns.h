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
#ifndef WRTDEVICEAPIS_COMMONS_REGEX_PATTERNS_H_
#define WRTDEVICEAPIS_COMMONS_REGEX_PATTERNS_H_

#include <string>

namespace WrtDeviceApis {
namespace Commons {
const std::string PATTERN_PHONE_NUMBER_FILTER("[^0-9+#*Pp]");
const std::string PATTERN_PHONE_NUMBER("^[0-9#+*]+$");
const std::string PATTERN_PHONE_NUMBER_P("^\\+?[0-9#*Pp]*[0-9][0-9#*Pp]*$");
const std::string PATTERN_PHONE_NUMBER_PW("^[0-9#+*PpWw]+$");
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_REGEX_PATTERNS_H_
