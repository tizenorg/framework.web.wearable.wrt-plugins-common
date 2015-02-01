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
 * @file        IWidgetDB.h
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 * @brief       Widget interface to access WRT DB
 */

#ifndef WRTDEVICEAPIS_WIDGETDB_IWIDGETDB_H_
#define WRTDEVICEAPIS_WIDGETDB_IWIDGETDB_H_

#include "IWidgetFeature.h"
#include <vector>
#include <dpl/shared_ptr.h>

namespace WrtDeviceApis {
namespace WidgetDB {
namespace Api {
class IWidgetDB;
typedef DPL::SharedPtr<IWidgetDB> IWidgetDBPtr;

typedef std::vector<IWidgetFeaturePtr> Features;

enum class ConfigAttribute
{
    ID,
    VERSION,
    DESCRIPTION,
    LICENSE,
    LICENSE_HREF,
    AUTHOR_NAME,
    AUTHOR_HREF,
    AUTHOR_EMAIL,
    NAME,
    SHORT_NAME,
    ICON,
    WIDTH,
    HEIGHT,
    INSTALL_PATH,
    PUBLIC_STORAGE_PATH
};

enum class InstallationStatus
{
    STATUS_INSTALLED,
    STATUS_LATEST,
    STATUS_UNINSTALLED
};

class IWidgetDB
{
  public:

    virtual int getWidgetId() const = 0;

    virtual std::string getLanguage() const = 0;

    virtual std::string getConfigValue(ConfigAttribute attribute) const = 0;

    virtual std::string getUserAgent() const = 0;

    virtual InstallationStatus checkInstallationStatus(
        const std::string& gid,
        const std::string& name,
        const std::string& version) const = 0;

    virtual Features getWidgetFeatures() const = 0;

    virtual Features getRegisteredFeatures() const = 0;

    virtual std::string getWidgetInstallationPath() const = 0;

    virtual std::string getWidgetPersistentStoragePath() const = 0;

    virtual std::string getWidgetTemporaryStoragePath() const = 0;

    virtual ~IWidgetDB()
    {}
};
} // Api
} // WidgetDB
} // WrtDeviceApis

#endif

