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
 * @file        WidgetDB.cpp
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 * @brief
 */

#include "WidgetDB.h"

#include <string>
#include <set>

#include <Commons/Exception.h>
#include <WidgetDB/IWidgetDB.h>
#include <WidgetDB/IWidgetFeature.h>
#include <dpl/wrt-dao-ro/property_dao_read_only.h>
#include <dpl/wrt-dao-ro/widget_dao_read_only.h>
#include <dpl/wrt-dao-ro/global_config.h>
#include <dpl/wrt-dao-ro/widget_config.h>
#include <dpl/wrt-dao-ro/feature_dao_read_only.h>
#include <dpl/wrt-dao-ro/widget_dao_read_only.h>
#include <dpl/wrt-dao-ro/feature_model.h>
#include <dpl/wrt-dao-ro/common_dao_types.h>

#include <dpl/localization/w3c_file_localization.h>

#include "WidgetFeature.h"

namespace WrtDeviceApis {
namespace WidgetDB {
WidgetDB::WidgetDB(int widgetId) : m_widgetId(widgetId) {}

int WidgetDB::getWidgetId() const
{
    return m_widgetId;
}

std::string WidgetDB::getLanguage() const
{
    LogDebug("getting language");

    WrtDB::LanguageTagList languageTags;
    std::string outLanguage;
    Try
    {
        WrtDB::WidgetDAOReadOnly dao(getWidgetId());
        languageTags = dao.getLanguageTags();
    }
    Catch(DPL::Exception)
    {
        ReThrow(Commons::SecurityException);
    }

    // get 'en' language or first from the list if 'en' does not exist
    if (languageTags.size() > 1) {
        FOREACH(lang, languageTags)
        {
            if ("en" == DPL::ToUTF8String(*lang)) {
                return "en";
            }
        }
    }

    if (!languageTags.empty()) {
        // get first language tag
        outLanguage = DPL::ToUTF8String(*(languageTags.begin()));
    }
    return outLanguage;
}

/*License,LicenseHref,Description,Name can be \
 *  returned with incorect language*/
std::string WidgetDB::getConfigValue(Api::ConfigAttribute attribute) const
{
    LogDebug("entered");
    std::string value;
    Try
    {
        WrtDB::WidgetDAOReadOnlyPtr dao(new WrtDB::WidgetDAOReadOnly(getWidgetId()));
        WrtDB::WidgetLocalizedInfo widgetInfo =
            W3CFileLocalization::getLocalizedInfo(dao);

        std::ostringstream out;

        switch (attribute) {
        case Api::ConfigAttribute::ID: {
            DPL::OptionalString val = dao->getGUID();
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::VERSION: {
            DPL::OptionalString val = dao->getVersion();
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::DESCRIPTION: {
            DPL::OptionalString val = widgetInfo.description;
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::LICENSE: {
            DPL::OptionalString val = widgetInfo.license;
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::LICENSE_HREF: {
            DPL::OptionalString val = widgetInfo.licenseHref;
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::AUTHOR_NAME: {
            DPL::OptionalString val = dao->getAuthorName();
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::AUTHOR_EMAIL: {
            DPL::OptionalString val = dao->getAuthorEmail();
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::AUTHOR_HREF: {
            DPL::OptionalString val = dao->getAuthorHref();
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::NAME: {
            DPL::OptionalString val = widgetInfo.name;
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::SHORT_NAME: {
            DPL::OptionalString val = widgetInfo.shortName;
            value = !val ? "" : DPL::ToUTF8String(*val);
            break;
        }

        case Api::ConfigAttribute::WIDTH: {
            DPL::OptionalInt width = dao->getPreferredSize().width;
            !width ? out << "0" : out << *width;
            value = out.str();
            break;
        }

        case Api::ConfigAttribute::HEIGHT: {
            DPL::OptionalInt height = dao->getPreferredSize().height;
            !height ? out << "0" : out << *height;
            value = out.str();
            break;
        }

        case Api::ConfigAttribute::INSTALL_PATH:
            value = DPL::ToUTF8String(dao->getPath());
            break;

        case Api::ConfigAttribute::PUBLIC_STORAGE_PATH:
            value = WrtDB::GlobalConfig::GetPublicVirtualRootPath();
            break;

        default:
            LogWarning("Unknown attribute requested");
            Throw(Commons::InvalidArgumentException);
        }
    }
    Catch(Commons::InvalidArgumentException)
    {
        ReThrow(Commons::InvalidArgumentException);
    }
    Catch(DPL::Exception)
    {
        ReThrow(Commons::SecurityException);
    }

    LogDebug("exit");

    return value;
}

std::string WidgetDB::getUserAgent() const
{
    LogDebug("Widget User_Agent data get");

    std::string outUA("not implemented");
    return outUA;
}

Api::InstallationStatus WidgetDB::checkInstallationStatus(
    const std::string& gid,
    const std::string& name,
    const std::string& version) const
{
    LogDebug("Checking widget ( gid(" << gid << "), name(" << name <<
             "), version(" << version << ")) installation status");

    Api::InstallationStatus outStatus;
    Try
    {
        const WrtDB::WidgetGUID w_guid =
            WrtDB::WidgetGUID(DPL::FromUTF8String(gid));

        try {
            WrtDB::DbWidgetHandle widgetHandle =
                WrtDB::WidgetDAOReadOnly::getHandle(w_guid);

            WrtDB::WidgetDAOReadOnly dao(widgetHandle);
            DPL::OptionalString ver = dao.getVersion();
            if (!ver || version > DPL::ToUTF8String(*ver)) {
                // if there is no version in DB
                // or DB version is lover or equal than provided
                outStatus = Api::InstallationStatus::STATUS_INSTALLED;
            } else {
                outStatus = Api::InstallationStatus::STATUS_LATEST;
            }
        } catch (WrtDB::WidgetDAOReadOnly::Exception::WidgetNotExist) {
            outStatus = Api::InstallationStatus::STATUS_UNINSTALLED;
        }
    }
    Catch(DPL::Exception)
    {
        ReThrow(Commons::SecurityException);
    }

    return outStatus;
}

Api::Features WidgetDB::getWidgetFeatures() const
{
    LogDebug("entered");
    WrtDB::DbWidgetFeatureSet features;
    Try
    {
        WrtDB::WidgetDAOReadOnly doa(getWidgetId());
        features = doa.getFeaturesList();
    }
    Catch(DPL::Exception)
    {
        // exception handle
        ReThrow(Commons::SecurityException);
    }

    Api::Features outFeatures;

    LogDebug("There is no exception");

    FOREACH(it, features)
    {
        Api::IWidgetFeaturePtr feature(
            new WidgetFeature(DPL::ToUTF8String((*it).name),
                              true));      // requested (WidgetFeature param)
        outFeatures.push_back(feature);
    }

    return outFeatures;
}

Api::Features WidgetDB::getRegisteredFeatures() const
{
    Api::Features outRegisteredFeatures;
    Try
    {
        // initialize with our widget features (they are required)
        outRegisteredFeatures = getWidgetFeatures();

        // searching for another features

        WrtDB::FeatureHandleList features =
            WrtDB::FeatureDAOReadOnly::GetHandleList();

        bool featureFound = false;

        // throught all features
        FOREACH(registeredFeature, features)
        {
            WrtDB::FeatureHandle handle = *registeredFeature;
            WrtDB::FeatureDAOReadOnly dao(handle);

            // check if this feature is aleready on outRegisteredFeatures list
            // and if not, add it

            FOREACH(it, outRegisteredFeatures)
            {
                if (dao.GetName() == (*it)->getName()) {    // *it = shared ptr
                    // this name is found on list, do not add
                    featureFound = true;
                    break;
                }
            }

            if (!featureFound) {
                // feature is not on outRegisteredFeature so add it with
                // 'required' property false
                Api::IWidgetFeaturePtr newFeature(new WidgetFeature(
                                                      dao.GetName(),
                                                      false));

                // TODO what about feature params? aren't they needed
                outRegisteredFeatures.push_back(newFeature);
            }
            // reset the flag and check another feature
            featureFound = false;
        }
    }
    Catch(DPL::Exception)
    {
        ReThrow(Commons::SecurityException);
    }

    return outRegisteredFeatures;
}

std::string WidgetDB::getWidgetInstallationPath() const
{
    Try
    {
        WrtDB::WidgetDAOReadOnly dao(getWidgetId());
        std::string result(DPL::ToUTF8String(dao.getFullPath()));
        result.erase(0, strlen("file://"));
        std::string::size_type pos = result.size();
        std::string separator("/");
        pos = result.rfind(separator, pos);
        if ((result.size() - pos) > 0 && (result.size() - pos) < 2) {
            result.erase(pos, result.size());
        }
        return result;
    }
    Catch(DPL::Exception)
    {
        ReThrow(Commons::PlatformException);
    }
}

std::string WidgetDB::getWidgetPersistentStoragePath() const
{
    Try
    {
        WrtDB::WidgetDAOReadOnly dao(getWidgetId());
        return
            WrtDB::WidgetConfig::GetWidgetPersistentStoragePath(
                dao.getTizenPkgId());
    }
    Catch(DPL::Exception)
    {
        ReThrow(Commons::PlatformException);
    }
}

std::string WidgetDB::getWidgetTemporaryStoragePath() const
{
    Try
    {
        WrtDB::WidgetDAOReadOnly dao(getWidgetId());
        return
            WrtDB::WidgetConfig::GetWidgetTemporaryStoragePath(
                dao.getTizenPkgId());
    }
    Catch(DPL::Exception)
    {
        ReThrow(Commons::PlatformException);
    }
}
} // WidgetDB
} // WrtDeviceApis
