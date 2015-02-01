/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
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
/**
 * @file   PluginRegistration.cpp
 * @author Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#include "PluginRegistration.h"

#include "PluginRegistrationImpl.h"
#include "Plugin.h"
#include <dpl/assert.h>

namespace WrtPluginsApi {
PluginRegistration::PluginRegistration(Impl* impl) : m_impl(impl)
{
    AssertMsg(impl != 0, "impl is NULL");
}

template<typename SignalSignature>
void PluginRegistration::Connect(const typename SignalSignature::Type& slot)
{
    m_impl->Connect<SignalSignature>(slot);
}

void PluginRegistration::DisconnectAll()
{
    m_impl->DisconnectAll();
}

void PluginRegistration::AddPlugin(Plugin& plugin)
{
    m_impl->AddPlugin(plugin);
}

#define EXPLICIT_INSTATIATE_PLUGIN_REGISTRATION(SignalSignature)               \
    template void PluginRegistration::Connect<SignalSignature>(            \
        const typename SignalSignature::Type &)

EXPLICIT_INSTATIATE_PLUGIN_REGISTRATION(OnWidgetStart);
EXPLICIT_INSTATIATE_PLUGIN_REGISTRATION(OnWidgetStop);
EXPLICIT_INSTATIATE_PLUGIN_REGISTRATION(OnFrameLoad);
EXPLICIT_INSTATIATE_PLUGIN_REGISTRATION(OnFrameUnload);
}
