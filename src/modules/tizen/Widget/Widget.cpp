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
 *
 * @file       Widget.h
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */

#include <dpl/log/log.h>
#include <Commons/StringUtils.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include "Widget.h"
#include <WidgetDB/IWidgetDB.h>
#include <WidgetDB/WidgetDBMgr.h>

namespace WrtDeviceApis {
namespace Widget {
using namespace std;
using namespace WidgetDB::Api;

Widget::Widget()
{
    using namespace WrtDeviceApis::Commons;
    m_widgetId = WrtAccessSingleton::Instance().getWidgetId();
}

Widget::~Widget()
{}

string Widget::getProperty(ConfigAttribute attr) const
{
    checkWidgetId();

    string value;
    Try {
        IWidgetDBPtr widgetDB = getWidgetDB(m_widgetId);
        value = widgetDB->getConfigValue(attr);
    }
    Catch(Commons::InvalidArgumentException){
        LogError("Invalid argument exception");
        Throw(Commons::PlatformException);
    }

    return value;
}

string Widget::getAuthor() const
{
    checkWidgetId();

    if (!m_author.isValid()) {
        m_author.setValue(getProperty(ConfigAttribute::AUTHOR_NAME));
    }
    return m_author.getValue();
}

string Widget::getAuthorEmail() const
{
    checkWidgetId();

    if (!m_authorEmail.isValid()) {
        m_authorEmail.setValue(getProperty(ConfigAttribute::AUTHOR_EMAIL));
    }
    return m_authorEmail.getValue();
}

string Widget::getAuthorHref() const
{
    checkWidgetId();

    if (!m_authorHref.isValid()) {
        m_authorHref.setValue(getProperty(ConfigAttribute::AUTHOR_HREF));
    }
    return m_authorHref.getValue();
}

string Widget::getDescription() const
{
    checkWidgetId();

    if (!m_description.isValid()) {
        m_description.setValue(getProperty(ConfigAttribute::DESCRIPTION));
    }
    return m_description.getValue();
}

string Widget::getId() const
{
    checkWidgetId();

    LogDebug("entered");
    if (!m_id.isValid()) {
        m_id.setValue(getProperty(ConfigAttribute::ID));
    }
    return m_id.getValue();
}

string Widget::getName() const
{
    checkWidgetId();

    if (!m_name.isValid()) {
        m_name.setValue(getProperty(ConfigAttribute::NAME));
    }
    return m_name.getValue();
}

string Widget::getShortName() const
{
    checkWidgetId();

    if (!m_shortName.isValid()) {
        m_shortName.setValue(getProperty(ConfigAttribute::SHORT_NAME));
    }
    return m_shortName.getValue();
}

string Widget::getVersion() const
{
    checkWidgetId();

    if (!m_version.isValid()) {
        m_version.setValue(getProperty(ConfigAttribute::VERSION));
    }
    return m_version.getValue();
}

unsigned int Widget::getHeight() const
{
    checkWidgetId();

    if (!m_height.isValid()) {
        m_height.setValue(Commons::String::convertTo<int>(
                              getProperty(ConfigAttribute::HEIGHT)));
    }
    return m_height.getValue();
}

unsigned int Widget::getWidth() const
{
    checkWidgetId();

    if (!m_width.isValid()) {
        m_width.setValue(Commons::String::convertTo<int>(
                             getProperty(ConfigAttribute::WIDTH)));
    }
    return m_width.getValue();
}

void Widget::checkWidgetId() const
{
    if (m_widgetId < 0) {
        LogError("Invalid widget id");
        Throw(Commons::InvalidArgumentException);
    }
    return;
}
}
}
