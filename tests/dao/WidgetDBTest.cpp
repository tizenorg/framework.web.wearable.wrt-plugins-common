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
 * @file    WidgetDBTest.cpp
 * @author  Andrzej Surdej (a.surdej@samsung.com)
 * @version 1.0
 * @brief   This file contains tests for widgetDB commons module
 */

#include <dpl/test/test_runner.h>
#include <dpl/foreach.h>
#include <dpl/log/log.h>
#include <WidgetDB/IWidgetDB.h>
#include <WidgetDB/WidgetDBMgr.h>
#include <Commons/Exception.h>

using namespace WrtDeviceApis::WidgetDB::Api;

#define RUNNER_ASSERT_WHAT_EQUALS(in, test)                                    \
    do                                                                         \
    {                                                                          \
        std::string temp(in);                                                  \
        RUNNER_ASSERT_MSG(temp == (test), "Equals: [" << temp << "]");         \
    }                                                                          \
    while (0)

RUNNER_TEST_GROUP_INIT(WidgetDB)

/*
 * Name: widgetDB_test_get_language1
 * Description: check default widget language
 * Expected: default should be "en"
 */
RUNNER_TEST(widgetDB_test_get_language1)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string lang = widget->getLanguage();

        LogDebug("language is: " << lang);
        RUNNER_ASSERT_WHAT_EQUALS(lang, "en");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_language2
 * Description: check language returned by WidgetDB if widget does nto exists
 * Expected: result should be ""
 */
RUNNER_TEST(widgetDB_test_get_language2)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(1000); //widget not exist
        std::string lang = widget->getLanguage();

        LogDebug("language is: " << lang);
        RUNNER_ASSERT_WHAT_EQUALS(lang, "");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_id
 * Description: check GUID returned by WidgetDB
 * Expected: GUID should match this, which was inserted into widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_id)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::ID);

        LogDebug("id is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "w_id_2000");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_version
 * Description: check version returned by WidgetDB
 * Expected: version should match this, which was inserted into widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_version)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::VERSION);

        LogDebug("version is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "1.0.0");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_description
 * Description: check description returned by WidgetDB
 * Expected: description should match this, which was inserted into widget
 * database
 */
RUNNER_TEST(widgetDB_test_get_config_value_description)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::DESCRIPTION);

        LogDebug("description is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "w_desc_2000_en");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_license
 * Description: check license returned by WidgetDB
 * Expected: license should match this, which was inserted into widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_license)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::LICENSE);

        LogDebug("licence is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "w_lic_2000_en");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_license_href
 * Description: check license href returned by WidgetDB
 * Expected: license href should match this, which was inserted into widget
 * database
 */
RUNNER_TEST(widgetDB_test_get_config_value_license_href)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::LICENSE_HREF);

        LogDebug("license href is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "w_lic_href_2000_en");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_author_name
 * Description: check author name href returned by WidgetDB
 * Expected: author name should match this, which was inserted into widget
 * database
 */
RUNNER_TEST(widgetDB_test_get_config_value_author_name)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::AUTHOR_NAME);

        LogDebug("author name is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "a_name_2000");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_author_href
 * Description: check author href returned by WidgetDB
 * Expected: author href should match this, which was inserted into widget
 * database
 */
RUNNER_TEST(widgetDB_test_get_config_value_author_href)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::AUTHOR_HREF);

        LogDebug("author href is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "a_href_2000");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_author_email
 * Description: check author email returned by WidgetDB
 * Expected: author email should match this, which was inserted into widget
 * database
 */
RUNNER_TEST(widgetDB_test_get_config_value_author_email)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::AUTHOR_EMAIL);

        LogDebug("author email is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "a_email_2000");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_name
 * Description: check localized widget name returned by WidgetDB
 * Expected: localized widget name should match this, which was inserted into
 * widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_name)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::NAME);

        LogDebug("name is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "w_name_2000_en");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_short_name
 * Description: check localized widget short name returned by WidgetDB
 * Expected: localized widget short name should match this, which was inserted
 * into widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_short_name)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::SHORT_NAME);

        LogDebug("short name is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "w_shortname_2000_en");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_width
 * Description: check widget prefered width returned by WidgetDB
 * Expected: widget prefered width should match this, which was inserted into
 * widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_width)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::WIDTH);

        LogDebug("width is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "100");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_height
 * Description: check widget prefered height returned by WidgetDB
 * Expected: widget prefered height should match this, which was inserted into
 * widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_height)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::HEIGHT);

        LogDebug("height is: " << tmp);
        RUNNER_ASSERT_WHAT_EQUALS(tmp, "200");
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_paths
 * Description: check widget installation path returned by WidgetDB
 * Expected: widget installation path should match this, which was inserted into
 * widget database
 */
RUNNER_TEST(widgetDB_test_get_config_value_paths)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        std::string tmp = widget->getConfigValue(ConfigAttribute::INSTALL_PATH);

        LogDebug("install path is: " << tmp);

        tmp = widget->getConfigValue(
                ConfigAttribute::PUBLIC_STORAGE_PATH);

        LogDebug("public storage path is: " << tmp);
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogDebug("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_get_config_value_empty
 * Description: check if requesting GUID from WidgetDB throw excpetion if widget
 * does not exists
 * Expected: accessing GUID should throw
 * WrtDeviceApis::Commons::SecurityException
 */
RUNNER_TEST(widgetDB_test_get_config_value_empty)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2005);
        widget->getConfigValue(ConfigAttribute::ID);

        //exception should be thrown
        RUNNER_ASSERT(false);
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogDebug("OK. Exception thrown." << e.DumpToString());
    }
}

/*
 * Name: widgetDB_test_check_installation_status_installed
 * Description: check if widget is installed by WidgetDB
 * Expected: plugin should be notified that widget is installed
 */
RUNNER_TEST(widgetDB_test_check_installation_status_installed)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        InstallationStatus tmp = widget->checkInstallationStatus(
                "w_id_2000", "w_name_2000_en", "2.0.0");

        RUNNER_ASSERT(tmp == InstallationStatus::STATUS_INSTALLED);
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_check_installation_status_uninstalled
 * Description: check if widget is not installed by WidgetDB
 * Expected: plugin should be notified that widget is not installed
 */
RUNNER_TEST(widgetDB_test_check_installation_status_uninstalled)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        InstallationStatus tmp = widget->checkInstallationStatus(
                "w_id_2011", "w_name_2000_en", "2.0.0");

        RUNNER_ASSERT(tmp == InstallationStatus::STATUS_UNINSTALLED);
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

/*
 * Name: widgetDB_test_check_installation_status_latest
 * Description: check if widget installation information is taken from latest
 * version
 * Expected: plugin installation status should be same is both calls of WidgetDB
 */
RUNNER_TEST(widgetDB_test_check_installation_status_latest)
{
    try {
        IWidgetDBPtr widget = getWidgetDB(2000);
        InstallationStatus tmp1 = widget->checkInstallationStatus(
                "w_id_2000", "w_name_2000_en", "0.1.0");
        InstallationStatus tmp2 = widget->checkInstallationStatus(
                "w_id_2000", "w_name_2000_en", "1.0.0");

        RUNNER_ASSERT(tmp1 == InstallationStatus::STATUS_LATEST);
        RUNNER_ASSERT(tmp2 == InstallationStatus::STATUS_LATEST);
    } catch (const WrtDeviceApis::Commons::SecurityException &e) {
        LogError("Exception thrown." << e.DumpToString());
        RUNNER_ASSERT(false);
    }
}

#undef RUNNER_ASSERT_WHAT_EQUALS
