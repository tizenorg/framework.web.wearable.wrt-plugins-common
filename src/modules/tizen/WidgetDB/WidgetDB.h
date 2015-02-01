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
 * @file        WidgetDB.h
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 * @brief
 */

#ifndef WRTDEVICEAPIS_WIDGETDB_WIDGETDB_H_
#define WRTDEVICEAPIS_WIDGETDB_WIDGETDB_H_

#include <string>
#include <WidgetDB/IWidgetDB.h>

namespace WrtDeviceApis {
namespace WidgetDB {
class WidgetDB : public Api::IWidgetDB
{
  public:

    explicit WidgetDB(int widgetId);

    virtual int getWidgetId() const;

    virtual std::string getLanguage() const;

    virtual std::string getConfigValue(Api::ConfigAttribute attribute) const;

    virtual std::string getUserAgent() const;

    virtual Api::InstallationStatus checkInstallationStatus(
        const std::string& gid,
        const std::string& name,
        const std::string& version) const;

    virtual Api::Features getWidgetFeatures() const;

    virtual Api::Features getRegisteredFeatures() const;

    virtual std::string getWidgetInstallationPath() const;

    virtual std::string getWidgetPersistentStoragePath() const;

    virtual std::string getWidgetTemporaryStoragePath() const;

  private:

    int m_widgetId;
};
} // WidgetDB
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_WIDGETDB_WIDGETDB_H

