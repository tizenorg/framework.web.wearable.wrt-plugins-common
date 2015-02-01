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
#include "RegexUtils.h"
#include <pcrecpp.h>
#include "StringUtils.h"

namespace WrtDeviceApis {
namespace Commons {
std::string addStartPositionMark(const std::string &value)
{
    if (!String::startsWith(value, "^") && !String::startsWith(value, ".*")) {
        return "^" + value;
    }
    return value;
}

std::string addEndPositionMark(const std::string &value)
{
    if ((!String::endsWith(value, "$") || String::endsWith(value, "\\$")) &&
        !String::endsWith(value, ".*"))
    {
        return value + "$";
    }
    return value;
}

std::string addStartAndEndPositionMark(const std::string &value)
{
    return addEndPositionMark(addStartPositionMark(value));
}

std::string preparePercent(const std::string& str)
{
    std::string result = escape(str);
    pcrecpp::RE("(([^\\\\])\\*|^()\\*)").GlobalReplace("\\2\\\\*", &result);
    pcrecpp::RE("(([^\\\\])%|^()%)").GlobalReplace("\\2.*", &result);
    return result;
}

std::string prepareAsterisk(const std::string& str)
{
    std::string result = escape(str);
    // Replaces single wildcard "*" with version more compliant with Perl
    // regular exrepssions, i.e. ".*".
    pcrecpp::RE("(([^\\\\])\\*|^()\\*)").GlobalReplace("\\2.*", &result);
    return result;
}

std::string escape(const std::string& str)
{
    std::string result = str;
    // Escape standard regular expressions' metacharacters,
    // i.e.: \, ., -, +, ?, (, ), [, ], ^, $, |
    const char* metas = "(\\.|\\-|\\+|\\?|\\(|\\)|\\[|\\]|\\^|\\$|\\|"
                        "|(\\\\[^\\*\\%]|\\\\$))";        // \*, \% won't get
                                                          // additional '\'
    pcrecpp::RE(metas).GlobalReplace("\\\\\\1", &result);
    return result;
}
}
} // WrtDeviceApisCommon
