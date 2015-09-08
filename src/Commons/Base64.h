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
#ifndef WRTDEVICEAPIS_COMMONS_BASE64_H_
#define WRTDEVICEAPIS_COMMONS_BASE64_H_

#include <string>
#include <cstddef>

namespace WrtDeviceApis {
namespace Commons {
class Base64
{
  public:
    static std::string encode(unsigned char* data,
                              std::size_t num);
    static std::string decode(const std::string& str);

  private:
    static bool is_base64(unsigned char c);
    static bool is_base64_string(const std::string& str);

  private:
    static const std::string chars;
}; // Base64
}
} // WrtDeviceApisCommon

#endif /* WRTDEVICEAPIS_COMMONS_BASE64_H_ */
