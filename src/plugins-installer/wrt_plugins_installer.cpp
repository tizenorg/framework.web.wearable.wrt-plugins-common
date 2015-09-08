/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
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
/* @file    wrt_plugins_installer.cpp
 * @author  Andrzej Surdej(a.surdej@gmail.com)
 * @version 1.0
 * @brief   Main file for plugins installer
 */

#include <plugin_installer.h>
#include <dpl/log/log.h>
#include <string>
#include <cstdio>
#include <dpl/exception.h>

int main(int /*argc*/, char */*argv*/[])
{
    UNHANDLED_EXCEPTION_HANDLER_BEGIN
    {
        DPL::Log::LogSystemSingleton::Instance().SetTag("PLUGINS_INSTALLER");
        LogDebug("Plugins installation started.");

        printf("Installing plugins...\n");
        PluginsInstallerSingleton::Instance().initialize();
        int installed =
            PluginsInstallerSingleton::Instance().installAllPlugins();
        PluginsInstallerSingleton::Instance().deinitialize();
        printf("Completed: %d plugins installed.\n", installed);

        LogDebug("All plugins installed successfuly");
        return 0;
    }
    UNHANDLED_EXCEPTION_HANDLER_END
}
