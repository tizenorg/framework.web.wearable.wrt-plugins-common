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
/**
 *
 * @file        TizenServiceEvent.cpp
 * @author      Yunchan Cho (yunchan.cho@samsung.com)
 * @version     0.1
 * @brief       Tizen appservice event class implementation
 */

#include "TizenServiceEvent.h"

namespace WrtDeviceApis {
namespace TizenServiceEvent {
namespace Api {
TizenServiceEvent::TizenServiceEvent() :
        m_scale(0)
{}

TizenServiceEvent::~TizenServiceEvent()
{}

float TizenServiceEvent::getScale() const
{
    return m_scale;
}

std::string TizenServiceEvent::getBundle() const
{
    return m_bundle;
}

void TizenServiceEvent::setScale(const float scale)
{
    m_scale = scale;
}

void TizenServiceEvent::setBundle(const std::string& bundle)
{
    m_bundle = bundle;
}
} // Api
} // TizenServiceEvent
} // WrtDeviceApis
