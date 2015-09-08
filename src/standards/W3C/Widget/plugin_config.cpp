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
#include "plugin_config.h"

namespace WrtPlugins {
namespace W3C {
template <>
WidgetDeclarations::ParamsMap WidgetDeclarations::m_params =
    WrtPlugins::W3C::WidgetDeclarations::ParamsMap();

template <>
WidgetDeclarations::DeviceCapsMaps WidgetDeclarations::m_deviceCaps =
    WrtPlugins::W3C::WidgetDeclarations::DeviceCapsMaps();

template <>
WidgetDeclarations::FeaturesMap WidgetDeclarations::m_features = {
    {
        "http://w3cWidgetInterface.com/w3c-widget-interface",
        {}
    }
};

template <>
WidgetDeclarations::FunctionsMap WidgetDeclarations::m_functions =
    WrtPlugins::W3C::WidgetDeclarations::FunctionsMap();
} // namespace Wac2_0
} // namespace WrtPlugins
