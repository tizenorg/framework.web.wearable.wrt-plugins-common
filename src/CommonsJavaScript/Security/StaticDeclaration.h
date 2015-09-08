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

#ifndef _WRTPLUGINS_COMMONS_SRC_COMMONS_STATICDECLARATIOS_H_
#define _WRTPLUGINS_COMMONS_SRC_COMMONS_STATICDECLARATIOS_H_

#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <dpl/noncopyable.h>
#include <dpl/assert.h>
#include <dpl/foreach.h>
#include <Commons/TypesDeclaration.h>
#include <wrt-commons/wrt_plugin_export.h>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
template<typename ParamType,
         typename DeviceCapType,
         typename FunctionType>
class StaticDeclarations : public DPL::Noncopyable
{
    struct FunctionTriplet {
        const char* name;
        std::vector<DeviceCapType> devCaps;
        std::vector<const char*> features;
    };

  public:

    typedef typename std::map<ParamType, const char*>  ParamsMap;

    typedef std::map<DeviceCapType,
                     std::pair<const char*,
                               std::vector<ParamType> > >  DeviceCapsMaps;

    typedef std::map<FunctionType, FunctionTriplet >  FunctionsMap;

    typedef std::map<FunctionType, WrtDeviceApis::Commons::AceFunction>
    AceFunctionsMap;

    typedef std::map<std::string, std::vector<DeviceCapType> > FeaturesMap;

    static const std::string getParamName(const ParamType& paramId)
    {
        auto it = m_params.find(paramId);

        Assert(it != m_params.end() && "No such paramId");

        return it->second;
    }

    /**
     * Returns set of device capabilities WITHOUT params
     * for given device capability id
     * */
    static WrtDeviceApis::Commons::AceDeviceCapability
    getDeviceCapabilityWithoutParams(const DeviceCapType& devCapsId)
    {
        WrtDeviceApis::Commons::AceDeviceCapability deviceCap;
        auto it = m_deviceCaps.find(devCapsId);

        Assert(it != m_deviceCaps.end() && "No such device cap");

        deviceCap.devCapName = it->second.first;

        return deviceCap;
    }

    /**
     * Returns set of device capabilities with set params
     * for given device capability id
     * */
    static WrtDeviceApis::Commons::AceDeviceCapability
    getDeviceCapability(const DeviceCapType& devCapsId)
    {
        auto it = m_deviceCaps.find(devCapsId);

        Assert(it != m_deviceCaps.end() && "No such dev-cap found");

        WrtDeviceApis::Commons::AceDeviceCapability deviceCap;
        deviceCap.devCapName = it->second.first;

        FOREACH(paramIt, it->second.second)
        {
            WrtDeviceApis::Commons::AceDeviceCapParam param(
                getParamName(*paramIt),
                std::string());

            deviceCap.devCapParams.push_back(param);
        }

        return deviceCap;
    }

    static void addDeviceCapabilty(
        const DeviceCapType& devCapsId,
        WrtDeviceApis::Commons::AceFunction& aceFunction)
    {
        aceFunction.deviceCapabilities.push_back(
            getDeviceCapability(devCapsId));
    }

    /**
     * Returns names of device-capabilities base on capability id
     */
    static std::string getDevCapNameById(DeviceCapType devCapId)
    {
        auto it = m_deviceCaps.find(devCapId);
        Assert(it != m_deviceCaps.end() && "No such devcapid found!");
        return it->second.first;
    }

    /**
     * Sets parameter value for given paramId
     */
    static bool setParamValue(WrtDeviceApis::Commons::AceFunction& function,
                              ParamType paramId,
                              DeviceCapType devCapId,
                              const std::string& value)
    {
        //get name of the deviceCaps
        std::string devCapName = getDevCapNameById(devCapId);
        std::string paramName = getParamName(paramId);

        //search throw all the device capabilities
        FOREACH(devCapIt, function.deviceCapabilities) {
            if (devCapIt->devCapName == devCapName) {
                //device capability has been found
                //check params
                FOREACH(devParamIt, devCapIt->devCapParams) {
                    if (devParamIt->name == paramName) {
                        devParamIt->value = value;
                        return true;
                    }
                }
            }
        }
        return false;
    }
    /**
     * Return struct Commons::AceFunction with set function name
     *
     * To set device capabilities you may use setDeviceCap function
     * To set param value function you may use setParamValue Function
     * */
    static WrtDeviceApis::Commons::AceFunction getEmptyFunction(
        const FunctionType& functionId)
    {
        WrtDeviceApis::Commons::AceFunction function;
        auto it = m_functions.find(functionId);
        Assert(it != m_functions.end() && "No such a function");
        function.name = it->second.first;

        return function;
    }

    /**
     * The most useful Function
     * Return Commons::AceFunction with filled all required fields:
     * name, device caps and proper param namespace
     *
     * To set param value function you may use setParamValue function
     * */
    static WrtDeviceApis::Commons::AceFunction getSecurityFunction(
        const FunctionType& functionId)
    {
        WrtDeviceApis::Commons::AceFunction function;
        auto it = m_functions.find(functionId);
        Assert(it != m_functions.end() && "No such function found!");

        function.name = it->second.name;

        FOREACH(featIt, it->second.features)
        function.features.push_back(std::string(*featIt));

        FOREACH(devCapIt, it->second.devCaps) {
            function.deviceCapabilities.push_back(
                getDeviceCapability(*devCapIt));
        }
        return function;
    }

    /**
     * To create static map
     * */
    static void createStaticAceFunctions()
    {
        FOREACH(functionIt, m_functions)
        {
            m_aceFunctions[functionIt->first] =
                getSecurityFunction(functionIt->first);
        }
    }

    static feature_mapping_t* getFeaturesToDevCapMapping()
    {
        feature_mapping_t* mapping = new feature_mapping_t;

        mapping->featuresCount = m_features.size();
        mapping->features = new feature_devcaps_t[mapping->featuresCount];

        size_t i = 0;

        FOREACH(featureIt, m_features)
        {
            mapping->features[i].feature_name =
                strndup(featureIt->first.c_str(), featureIt->first.size() + 1);

            mapping->features[i].devCaps.devCapsCount =
                featureIt->second.size();

            mapping->features[i].devCaps.deviceCaps =
                new char*[mapping->features[i].devCaps.devCapsCount];

            for (size_t j = 0;
                 j < mapping->features[i].devCaps.devCapsCount;
                 ++j)
            {
                std::string dc = getDevCapNameById(featureIt->second[j]);

                mapping->features[i].devCaps.deviceCaps[j] =
                    strndup(dc.c_str(), dc.size() + 1);
            }

            ++i;
        }

        return mapping;
    }

    static const devcaps_t* devcapsGetter(pfeature_mapping_t feats,
                                          const char* featureName)
    {
        Assert(featureName && "Trying to extract info about NULL api feature");

        std::string feature(featureName);

        devcaps_t* ret = NULL;

        for (size_t i = 0; i < feats->featuresCount; ++i) {
            Assert(feats->features &&
                   feats->features[i].feature_name &&
                   "NULL api feature in feature mapping");

            std::string feat(feats->features[i].feature_name);

            if (feature == feat) {
                ret = &(feats->features[i].devCaps);
                break;
            }
        }

        return ret;
    }

    static void featuresDeinitializer(feature_mapping_t* mapping)
    {
        if (mapping) {
            if (mapping->features) {
                for (size_t i = 0; i < mapping->featuresCount; ++i) {
                    free(mapping->features[i].feature_name);

                    devcaps_t& dc = mapping->features[i].devCaps;

                    if (dc.deviceCaps) {
                        for (size_t j = 0; j < dc.devCapsCount; ++j) {
                            free(dc.deviceCaps[j]);
                        }

                        delete[] dc.deviceCaps;
                    }
                }
                delete[] mapping->features;
            }
            delete mapping;
        }
    }

    static void getMappingInterface(feature_mapping_interface_t *mapping)
    {
        if (mapping) {
            mapping->featGetter =
                StaticDeclarations::getFeaturesToDevCapMapping;
            mapping->dcGetter = StaticDeclarations::devcapsGetter;
            mapping->release = StaticDeclarations::featuresDeinitializer;
        }
    }

  private:
    static ParamsMap m_params;
    static DeviceCapsMaps m_deviceCaps;
    static FunctionsMap m_functions;
    static AceFunctionsMap m_aceFunctions;

    static FeaturesMap m_features;
};
}
}

#endif
