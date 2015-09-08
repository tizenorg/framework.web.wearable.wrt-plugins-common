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
#include <pcrecpp.h>
#include "Regex.h"

namespace WrtDeviceApis {
namespace Commons {
bool validate(const std::string &pattern,
              const std::string &value,
              unsigned int options)
{
    pcrecpp::RE_Options pcreOpt;
    if (options & VALIDATE_MATCH_CASELESS) {
        pcreOpt.set_caseless(true);
    }
    pcrecpp::RE re(pattern, pcreOpt);
    if (options & VALIDATE_MATCH_FULL) {
        return re.FullMatch(value);
    }
    return re.PartialMatch(value);
}

std::string filter(const std::string &pattern,
                   const std::string &value)
{
    pcrecpp::RE re(pattern);
    std::string ret = static_cast<std::string>(value);
    re.GlobalReplace("", &ret);
    return ret;
}

std::string toUpper(const std::string &value)
{
    pcrecpp::RE re(LOWER_P);
    std::string ret = static_cast<std::string>(value);
    re.GlobalReplace(UPPER_P, &ret);
    return ret;
}
}
} // WrtDeviceApisCommon
