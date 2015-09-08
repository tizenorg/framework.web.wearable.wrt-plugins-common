#!/bin/sh
#
# Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#
#

WRT_DB=/opt/dbspace/.wrt.db
WRT_DB_BCK=/tmp/wrt.db_backup

if [ "x$1" == "xstart" ]; then
    echo start;
    cp $WRT_DB $WRT_DB_BCK
    wrt_commons_create_clean_db.sh;

    #simple plugins
    INS_MIN_PLUGINPROP="insert into PluginProperties(PluginPropertiesId, InstallationState, PluginLibraryName"
    INS_ALL_PLUGINPROP="insert into PluginProperties(PluginPropertiesId, InstallationState, PluginLibraryName, PluginLibraryPath)"

    sqlite3 $WRT_DB "${INS_MIN_PLUGINPROP}) VALUES(1, 1, 'plugin1')";
    sqlite3 $WRT_DB "${INS_MIN_PLUGINPROP}, PluginLibraryPath) VALUES(2, 1, 'plugin2', 'path_to_plugin2')";
    sqlite3 $WRT_DB "${INS_MIN_PLUGINPROP}) VALUES(3, 1, 'plugin3')";
    sqlite3 $WRT_DB "${INS_ALL_PLUGINPROP} VALUES(4, 1, 'p4', 'path_to_p4')";
    sqlite3 $WRT_DB "${INS_ALL_PLUGINPROP} VALUES(5, 1, 'p5', 'path_to_p5')";

    #simple features
    INS_ALL_FEATURESLIST="insert into FeaturesList(FeatureUUID, FeatureName, PluginPropertiesId)"
    sqlite3 $WRT_DB "${INS_ALL_FEATURESLIST} VALUES(1, 'feature1', 1)";
    sqlite3 $WRT_DB "${INS_ALL_FEATURESLIST} VALUES(2, 'feature2', 4)";
    sqlite3 $WRT_DB "${INS_ALL_FEATURESLIST} VALUES(3, 'feature3', 4)";
    sqlite3 $WRT_DB "${INS_ALL_FEATURESLIST} VALUES(4, 'feature4', 4)";

    #Device capab
    #TODO

    #Widgets
    INS_ALL_WIDGETEXT="insert into WidgetExtendedInfo(app_id)"
    INS_ALL_WIDGET="insert into WidgetInfo(app_id, widget_id, widget_version, widget_width, widget_height, author_name, author_email, author_href, base_folder, webkit_plugins_required, wac_signed, min_version, back_supported, access_network, defaultlocale, tizen_appid)"
    INS_ALL_WIDGET_LOC="insert into LocalizedWidgetInfo(app_id, widget_locale, widget_name, widget_shortname, widget_description, widget_license, widget_license_file, widget_license_href)"
    INS_ALL_WIDGET_ICONS="insert into WidgetIcon(app_id, icon_src, icon_width, icon_height)"
    INS_ALL_WIDGET_LOC_ICONS="insert into WidgetLocalizedIcon(app_id, icon_id, widget_locale)"
    INS_ALL_WIDGET_STARTFILE="insert into WidgetStartFile(app_id, src)"
    INS_ALL_WIDGET_LOC_STARTFILE="insert into WidgetLocalizedStartFile(app_id, start_file_id, widget_locale, type, encoding)"
    INS_ALL_WIDGET_DEFPREF="insert into WidgetDefaultPreference(app_id, key_name, key_value, readonly)"
    INS_ALL_WIDGET_PREF="insert into WidgetPreference(app_id, tizen_appid, key_name, key_value, readonly)"
    INS_ALL_WIDGET_FEATURE="insert into WidgetFeature(widget_feature_id, app_id, name,  rejected)"
    INS_ALL_WIDGET_WINMODES="insert into WidgetWindowModes(app_id, window_mode)"
    INS_ALL_WIDGET_WARP="insert into WidgetWARPInfo(app_id, iri, subdomain_access)"
    INS_ALL_WIDGET_CERT="insert into WidgetCertificateFingerprint(app_id, owner, chainid, type, md5_fingerprint, sha1_fingerprint, common_name)"
    INS_ALL_WIDGET_POWDERLEV="insert into PowderLevels(app_id, category, level)"
    INS_ALL_WIDGET_POWDERLEV_CONT="insert into PowderLevelContexts(levelid, context)"


    sqlite3 $WRT_DB "${INS_ALL_WIDGET} VALUES(2000, 'w_id_2000', '1.0.0', 100, 200, 'a_name_2000', 'a_email_2000', 'a_href_2000', 'basef_2000', 1, 1, '1.0', 0, 0, 'en', 'tid2000')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET} VALUES(2001, 'w_id_2001', '2.0.0', 100, 200, 'a_name_2001', 'a_email_2001', 'a_href_2001', 'basef_2001', 1, 1, '0.5', 0, 0, 'en', 'tid2001')";
    sqlite3 $WRT_DB "insert into WidgetInfo(app_id, back_supported, tizen_appid) VALUES(2002, 0, 'tid2002')";
    sqlite3 $WRT_DB "insert into WidgetInfo(app_id, back_supported, tizen_appid) VALUES(2003, 0, 'NOTUSEDPKGNAME3')"; # for properties tests

    sqlite3 $WRT_DB "${INS_ALL_WIDGETEXT} VALUES(2000)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGETEXT} VALUES(2001)";
    sqlite3 $WRT_DB "insert into WidgetExtendedInfo(app_id) VALUES(2002)";
    sqlite3 $WRT_DB "insert into WidgetExtendedInfo(app_id) VALUES(2003)";

    sqlite3 $WRT_DB "${INS_ALL_WIDGET_LOC} VALUES(2000, 'en', 'w_name_2000_en', 'w_shortname_2000_en', 'w_desc_2000_en', 'w_lic_2000_en', 'w_licf_2000_en', 'w_lic_href_2000_en')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_LOC} VALUES(2000, 'pl', 'w_name_2000_pl', 'w_shortname_2000_pl', 'w_desc_2000_pl', 'w_lic_2000_pl', 'w_licf_2000_pl', 'w_lic_href_2000_pl')";
    sqlite3 $WRT_DB "insert into LocalizedWidgetInfo(app_id, widget_locale) VALUES(2002, 'en')";
    sqlite3 $WRT_DB "insert into LocalizedWidgetInfo(app_id, widget_locale) VALUES(2003, 'en')";

#Widget features
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(1,2000, 'first_2000', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(2,2000, 'second_2000', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(3,2000, 'third_2000', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(8,2000, 'fourth_2000', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(9,2000, 'fifth_2000', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(4,2001, 'first_2001', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(5,2001, 'second_2001', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(6,2001, 'third_2001', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_FEATURE} VALUES(7,2001, 'fourth_2001', 0)";

    sqlite3 $WRT_DB "${INS_ALL_WIDGET_ICONS} VALUES(2000, 'icon_src_2000', 50, 50)";
    sqlite3 $WRT_DB "insert into WidgetIcon(app_id, icon_src) VALUES(2002, 'icon_src_2002')";

    sqlite3 $WRT_DB "${INS_ALL_WIDGET_LOC_ICONS} VALUES(2000, 1, 'en')";

    sqlite3 $WRT_DB "${INS_ALL_WIDGET_STARTFILE} VALUES(2000, 'start_file_2000')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_STARTFILE} VALUES(2001, 'start_file_2001')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_STARTFILE} VALUES(2002, 'start_file_2002')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_STARTFILE} VALUES(2003, 'start_file_2003')";

    sqlite3 $WRT_DB "${INS_ALL_WIDGET_LOC_STARTFILE} VALUES(2000, 1, 'en', '', '')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_LOC_STARTFILE} VALUES(2001, 2, 'en', '', '')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_LOC_STARTFILE} VALUES(2002, 3, 'en', '', '')";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_LOC_STARTFILE} VALUES(2003, 4, 'en', '', '')";

    #widget properties
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_PREF} VALUES(2000, 'tid2000', 'key1_for_2000', 'value_for_key1_2000', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_PREF} VALUES(2000, 'tid2000', 'key2_for_2000', 'value_for_key2_2000', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_PREF} VALUES(2001, 'tid2001', 'key1_for_2001', 'value1_for_key_2001', 1)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_PREF} VALUES(2002, 'tid2002', 'key1_for_2002', 'value1_for_key_2002', 0)";
    sqlite3 $WRT_DB "${INS_ALL_WIDGET_PREF} VALUES(2002, 'tid2002', 'key2_for_2002', 'value2_for_key_2002', 1)";


    exit $?

elif [ "x$1" == "xstop" ]; then
    echo stop;
    cp $WRT_DB_BCK $WRT_DB
    exit $?
else
    exit 1
fi
