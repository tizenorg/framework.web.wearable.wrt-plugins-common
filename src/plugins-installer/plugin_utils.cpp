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
 * @file    plugin-utils.cpp
 * @author
 * @version 1.0
 * @brief   Header file for plugin util
 */

#include "plugin_utils.h"
#include <dpl/semaphore.h>
#include <dpl/exception.h>
#include <dpl/log/log.h>
#include <dpl/wrt-dao-ro/global_config.h>

using namespace WrtDB;

namespace PluginUtils {
//checks if file exists and is regular file
bool checkFileExistance(const std::string& filename)
{
    struct stat tmp;

    if (0 == stat(filename.c_str(), &tmp) && S_ISREG(tmp.st_mode)) {
        return true;
    }
    return false;
}

std::string cutOffFileName(const std::string& path)
{
    size_t found = path.find_last_of("/");
    if (found == std::string::npos) {
        return path;
    } else {
        return path.substr(0, found);
    }
}

bool checkPath(const std::string& path)
{
    struct stat st;
    if (0 == stat(path.c_str(), &st) && S_ISDIR(st.st_mode)) {
        return true;
    }
    LogError("Cannot access directory [ " << path << " ]");
    return false;
}

bool checkPaths()
{
    bool if_ok = true;
    if_ok &= (checkPath(cutOffFileName(
                            GlobalConfig::GetWrtDatabaseFilePath())));
    if (!if_ok) {
        LogError(
            "Path <" << GlobalConfig::GetWrtDatabaseFilePath() <<
            "> does not exist.");
    }

    if_ok &= (checkPath(GlobalConfig::GetDevicePluginPath()));
    if (!if_ok) {
        LogError(
            "Path <" << GlobalConfig::GetDevicePluginPath() <<
            "> does not exist.");
    }

    if_ok &= (checkPath(GlobalConfig::GetUserInstalledWidgetPath()));
    if (!if_ok) {
        LogError(
            "Path <" << GlobalConfig::GetUserInstalledWidgetPath() <<
            "> does not exist.");
    }
    return if_ok;
}
} //namespace PluginUtils
