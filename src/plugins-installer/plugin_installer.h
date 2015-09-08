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
/**
 * @file    plugin_installer.h
 * @author  Andrzej Surdej(a.surdej@samgsung.com)
 * @version
 * @brief
 */

#ifndef WRT_PLUGINS_INSTALLER_PLUGIN_INSTALLER_H
#define WRT_PLUGINS_INSTALLER_PLUGIN_INSTALLER_H

#include <string>
#include <boost/optional.hpp>
#include <plugin_installer_types.h>
#include <plugin_objects.h>
#include <IObject.h>
#include <PluginRegistry.h>

#include <dpl/singleton.h>

class PluginsInstaller
{
  public:
    static const int INSTALLATION_ERROR;
    enum class ReturnStatus
    {
        Success,                    ///< Success

        NotInitialized,             ///< Singleton not initialized
        WrongPluginPath,            ///< ?
        MetafileError,              ///< ?
        AlreadyInstalled,           ///< ?
        LoadingLibraryError,        ///< Loading library by dlopen failed.
                                    /// It may be caused by missing symbols
        InstallationWaiting,        /// Installation failed due to dependencies
        DatabaseError,              /// Unable to update database
        Unknown                     ///< Temporary error. Try to not use this.
    };

    bool initialize();
    int installAllPlugins();
    void deinitialize();

    ReturnStatus installPlugin(const std::string& libPath);
    ReturnStatus installPluginFromMetafile(const std::string& libPath,
                                           const std::string& metaFilePath);

    int installWaitingPlugins();
    bool resolvePluginDependencies(PluginHandle handle);

    bool isInitialized();

  private:
    friend class DPL::Singleton<PluginsInstaller>;

    PluginsInstaller();
    virtual ~PluginsInstaller();

    void registerObjects(const PluginObjectsPtr& libObj,
                         const WrtPluginsApi::IObjectsListPtr& objects) const;

    typedef boost::optional<WrtDB::PluginMetafileData> OptionalPluginMetafileData;
    OptionalPluginMetafileData parseMetafile(const std::string& path) const;

    std::string getLibraryName(const std::string& dirPath) const;
    bool registerAndUpdateInstallation(const PluginHandle& pluginHandle,
                                       const PluginObjectsPtr& libraries);
    bool fillMappingInterfaces(WrtDB::PluginMetafileData& pluginData,
                               const std::string& filename);
    void registerPluginObjects(const PluginHandle& handle,
                               const PluginObjectsPtr libObjects) const;
    void checkDatabaseTablesExistance();
    PluginObjectsPtr loadLibraryFromMetafile(const std::string& libName) const;

    WrtPluginsApi::PluginRegistry m_registry;
    bool m_initialized;
    bool m_reinstall;
};

typedef DPL::Singleton<PluginsInstaller> PluginsInstallerSingleton;

#endif  /* WRT_PLUGINS_INSTALLER_PLUGIN_INSTALLER_H */

