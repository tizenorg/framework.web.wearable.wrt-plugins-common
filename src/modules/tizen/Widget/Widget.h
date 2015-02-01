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

#ifndef PLATFORM_WIDGET_CONFIGURATION_H_
#define PLATFORM_WIDGET_CONFIGURATION_H_

#include <string>

#include <Commons/WrtAccess/WrtAccess.h>

#include <Widget/IWidget.h>
#include <WidgetDB/IWidgetDB.h>
#include "Property.h"

namespace WrtDeviceApis {
namespace Widget {
class Widget : public Api::IWidget
{
  public:

    explicit Widget();
    ~Widget();

    virtual std::string getAuthor() const;
    virtual std::string getAuthorEmail() const;
    virtual std::string getAuthorHref() const;
    virtual std::string getDescription() const;
    virtual std::string getId() const;
    virtual std::string getName() const;
    virtual std::string getShortName() const;
    virtual std::string getVersion() const;
    virtual unsigned int getHeight() const;
    virtual unsigned int getWidth() const;

  protected:

    mutable StringProperty m_author;
    mutable StringProperty m_authorEmail;
    mutable StringProperty m_authorHref;
    mutable StringProperty m_description;
    mutable StringProperty m_id;
    mutable StringProperty m_name;
    mutable StringProperty m_shortName;
    mutable StringProperty m_version;
    mutable UIntProperty m_height;
    mutable UIntProperty m_width;

  private:

    std::string getProperty(WidgetDB::Api::ConfigAttribute) const;
    void checkWidgetId() const;

    int m_widgetId;
};
}
}

#endif
