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
/*
 * @author      Lukasz Marek (l.marek@samsung.com)
 * @version     0.1
 * @brief
 */

#include "WidgetInterface.h"
#include <string>
#include <dpl/exception.h>
#include <dpl/log/log.h>
#include <dpl/string.h>
#include <wrt-commons/widget-interface-dao/widget_interface_dao.h>
#include <Commons/Exception.h>

namespace WrtDeviceApis {
using namespace WidgetInterfaceDB;

WidgetInterfaceObject::WidgetInterfaceObject(int widgetHandle)
{
    Try
    {
        m_dao.reset(new WidgetInterfaceDAO(widgetHandle));
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        ReThrow(Commons::PlatformException);
    }
}

WidgetInterfaceObject::~WidgetInterfaceObject()
{
    m_dao.reset();
}

void WidgetInterfaceObject::setItem(const std::string& key,
                                    const std::string& value,
                                    bool readOnly)
{
    Try
    {
        m_dao->setItem(key, value, readOnly);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        ReThrow(Commons::PlatformException);
    }
    Catch(WidgetInterfaceDAO::Exception::LocalStorageValueNoModifableException)
    {
        LogError("Cannot delete item. Item is readonly");
        ReThrow(Commons::LocalStorageValueNoModifableException);
    }
}

void WidgetInterfaceObject::removeItem(const std::string& key)
{
    Try
    {
        m_dao->removeItem(key);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        ReThrow(Commons::PlatformException);
    }
    Catch(WidgetInterfaceDAO::Exception::LocalStorageValueNoModifableException)
    {
        LogError("Cannot delete item. Item is readonly");
        ReThrow(Commons::LocalStorageValueNoModifableException);
    }
}

DPL::OptionalStdString WidgetInterfaceObject::getValue(
    const std::string& key) const
{
    Try
    {
        return m_dao->getValue(key);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        ReThrow(Commons::PlatformException);
    }
}

void WidgetInterfaceObject::clear(bool removeReadOnly)
{
    Try
    {
        m_dao->clear(removeReadOnly);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        ReThrow(Commons::PlatformException);
    }
}

size_t WidgetInterfaceObject::getStorageSize() const
{
    Try
    {
        return m_dao->getStorageSize();
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        ReThrow(Commons::PlatformException);
    }
}

std::string WidgetInterfaceObject::getKeyByIndex(size_t index) const
{
    Try
    {
        return m_dao->getKeyByIndex(index);
    }
    Catch(WidgetInterfaceDAO::Exception::DatabaseError)
    {
        ReThrow(Commons::PlatformException);
    }
    Catch(WidgetInterfaceDAO::Exception::InvalidArgumentException)
    {
        ReThrow(Commons::InvalidArgumentException);
    }
}
}
