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
#ifndef WRTDEVICEAPIS_COMMONS_FUNCTION_DEFINITION_
#define WRTDEVICEAPIS_COMMONS_FUNCTION_DEFINITION_

#include <map>
#include <string>
#include <Commons/FunctionDeclaration.h>

namespace WrtDeviceApis {
namespace Commons {
/**
 * Creates an empty device cap list. Additional device-caps may be added by
 * ACE_CREATE_DEVICE_CAP and ACE_ADD_DEVICE_CAP macros
 */
#define ACE_CREATE_DEVICE_CAPS_LIST(device) \
    WrtDeviceApis::Commons::AceDeviceCaps ace_##device

/**
 * Creates an empty api features list. Additional api features may be added by
 * ACE_CREATE_FEATURE and ACE_ADD_API_FEATURE macros
 */
#define ACE_CREATE_FEATURE_LIST(apifeature) \
    WrtDeviceApis::Commons::AceFeatures ace_##apifeature

/**
 * Creates a device capability which should be later added to device cap list
 */
#define ACE_CREATE_DEVICE_CAP(device_cap_id, cap_id)                      \
    WrtDeviceApis::Commons::AceDeviceCapability ace_##device_cap_id = \
        AceDeviceCapability(cap_id, WrtDeviceApis::Commons::AceDeviceCapParams());

/**
 * Create an api feature which should be later added to api features list
 */
#define ACE_CREATE_FEATURE(feature_id, feature_api) \
    WrtDeviceApis::Commons::AceFeature ace_##feature_id = { feature_api \
    };

/**
 * Adds an existing device-cap created by ACE_CREATE_DEVICE_CAP macro to
 * device cap list
 */
#define ACE_ADD_DEVICE_CAP(device, device_cap_id)   \
    WrtDeviceApis::Commons::aceAddDeviceCap(ace_##device_cap_id, ace_##device)

/**
 * Adds an existing api feature created by ACE_CREATE_FEATURE macro to
 * api feature list
 */
#define ACE_ADD_API_FEATURE(apifeature, feature_id)     \
    WrtDeviceApis::Commons::aceAddApiFeature(ace_##feature_id, \
                                             ace_##apifeature)

/**
 * Creates a function definition with given id that uses provided api feature
 * and device capability lists
 */
#define ACE_CREATE_FUNCTION(function_id, function_name, feature_id, device) \
    { function_name, ace_##feature_id, ace_##device }

// Support for param:name begin
#define ACE_CREATE_PARAM(param_definition, param_name, position)            \
    static WrtDeviceApis::Commons::AceDeviceCapParam ace_param_## \
    param_definition = \
    { std::string(param_name), position };

// Support for param:name end

typedef std::map <std::string, AceFunction> FunctionMapping;

int aceAddDeviceCap(const AceDeviceCapability &cap,
                    AceDeviceCaps & instance);
int aceAddApiFeature(const AceFeature &apiFeature,
                     AceFeatures & instance);
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_FUNCTION_DEFINITION
