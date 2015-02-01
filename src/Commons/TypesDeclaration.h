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
#ifndef WRTDEVICEAPIS_COMMONS_TYPES_DECLARATION_
#define WRTDEVICEAPIS_COMMONS_TYPES_DECLARATION_

#include <string>
#include <vector>

namespace WrtDeviceApis {
namespace Commons {
struct AceFeature
{
    AceFeature(const std::string& featName) : name(featName) {}
    std::string name;
};

typedef std::vector <AceFeature> AceFeatures;

struct AceDeviceCapParam
{
    AceDeviceCapParam(const std::string& parName,
                      const std::string& parVal) : name(parName),
        value(parVal)
    {}

    std::string name;
    std::string value;
};

typedef std::vector <AceDeviceCapParam> AceDeviceCapParams;

struct AceDeviceCapability
{
    AceDeviceCapability() {}
    AceDeviceCapability(const std::string& name,
                        const AceDeviceCapParams& params) :
        devCapName(name),
        devCapParams(params)
    {}

    std::string devCapName;
    AceDeviceCapParams devCapParams;
};

typedef std::vector <AceDeviceCapability> AceDeviceCaps;

struct AceFunction
{
    std::string name;
    AceFeatures features;
    AceDeviceCaps deviceCapabilities;
};
} // namespace Commons
} // namespace WrtDeviceApis

#endif
