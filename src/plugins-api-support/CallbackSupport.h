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
 * @file   CallbackSupport.h
 * @author Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_CALLBACK_SUPPORT_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_CALLBACK_SUPPORT_H_

#include <map>
#include <vector>
#include <string>
#include <dpl/foreach.h>

namespace WrtPluginsApi {
template<typename Sig>
class CallbackSupport
{
  public:
    typedef typename Sig::Signature SlotSignature;
    typedef typename Sig::Type SlotType;
    typedef std::string GroupType;
    typedef std::vector<SlotType> SlotList;

    void Connect(const GroupType& group, const SlotType& slot)
    {
        auto groupIt = m_slots.find(group);
        if (m_slots.end() == groupIt) {
            groupIt = m_slots.insert(std::make_pair(group, SlotList())).first;
        }
        groupIt->second.push_back(slot);
    }

    void Disconnect(const GroupType& group)
    {
        m_slots.erase(group);
    }

    template<typename ... Args>
    void Invoke(const Args& ... args)
    {
        FOREACH(groupIt, m_slots)
        {
            FOREACH(slotIt, groupIt->second)
            {
                (*slotIt)(args ...);
            }
        }
    }

    template<typename ... Args>
    void InvokeGroup(const GroupType& group, const Args& ... args)
    {
        auto groupIt = m_slots.find(group);

        if (m_slots.end() != groupIt) {
            FOREACH(slotIt, groupIt->second)
            {
                (*slotIt)(args ...);
            }
        }
    }

  private:
    std::map<GroupType, SlotList> m_slots;
};
}
#endif
