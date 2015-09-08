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
 * @file    IPluginRegistry.h
 * @author  Grzegorz Krawczyk (g.krawczyk@samgsung.com)
 * @version
 * @brief
 */

#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_SIGNALS_SUPPORT_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_SIGNALS_SUPPORT_H_

#include <tuple>
#include <string>
#include "CallbackSupport.h"
#include "tuple.h"
#include "PluginSignals.h"
#include "Plugin.h"

namespace WrtPluginsApi {
class SignalsSupport
{
  public:
    virtual ~SignalsSupport() {}

    template<typename T>
    void Connect(const std::string& libraryName, const typename T::Type& slot)
    {
        Tuple::get_by_type<CallbackSupport<T> >(m_slots).Connect(libraryName,
                                                                 slot);
    }

    void Disconnect(const std::string& libraryName)
    {
        DisconnectGroup(m_slots, libraryName);
    }

    virtual void AddPlugin(const std::string& libraryName, Plugin& plugin) = 0;

  protected:
    template<typename T, typename ... Args>
    void Invoke(const Args& ... args)
    {
        Tuple::get_by_type<CallbackSupport<T> >(m_slots).Invoke(args ...);
    }

    template<typename T, typename ... Args>
    void InvokeGroup(const std::string& libraryName, const Args& ... args)
    {
        Tuple::get_by_type<CallbackSupport<T> >(m_slots).InvokeGroup(
            libraryName,
            args ...);
    }

  private:
    template<int N, typename ... Args>
    void DisconnectSlot(std::tuple<Args ...>& slots,
                        const std::string& libraryName,
                        typename std::enable_if<(N >= 0)>::type* = NULL)
    {
        std::get<N>(slots).Disconnect(libraryName);
        DisconnectSlot<N - 1>(slots, libraryName);
    }

    template<int N, typename ... Args>
    void DisconnectSlot(std::tuple<Args ...>& /*slots*/,
                        const std::string& /*libraryName*/,
                        typename std::enable_if<(N == -1)>::type* = NULL)
    {}

    template<typename ... Args>
    void DisconnectGroup(std::tuple<Args ...>& slots,
                         const std::string& libraryName)
    {
        DisconnectSlot<sizeof ... (Args)-1>(slots, libraryName);
    }

    std::tuple<CallbackSupport<OnWidgetStart>,
               CallbackSupport<OnWidgetStop>,
               CallbackSupport<OnFrameLoad>,
               CallbackSupport<OnFrameUnload> > m_slots;
};
}

#endif
