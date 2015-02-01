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
 * @file    plugin_installer.cpp
 * @author  Andrzej Surdej(a.surdej@samgsung.com)
 * @version
 * @brief
 */

#include "plugin_installer.h"

#include <sys/stat.h>
#include <cstdio>
#include <dlfcn.h>
#include <string>

#include <vcore/VCore.h>
#include <libxml/parser.h>

#include "plugin_objects.h"
#include "plugin_metafile_reader.h"
#include "plugin_installer_errors.h"

#include <Plugin.h>
#include <IObject_cast.h>
#include <plugin_utils.h>
#include <ExportedApi.h>

#include <wrt-commons/wrt_plugin_export.h>

#include <dpl/log/log.h>
#include <dpl/exception.h>
#include <dpl/wrt-dao-ro/global_config.h>
#include <dpl/wrt-dao-rw/plugin_dao.h>
#include <dpl/wrt-dao-rw/feature_dao.h>
#include <dpl/foreach.h>
#include <dpl/wrt-dao-ro/WrtDatabase.h>

#include <dpl/singleton_impl.h>

using namespace WrtDB;
using namespace WrtPluginsApi;

IMPLEMENT_SINGLETON(PluginsInstaller)

namespace {
const std::string DIRECTORY_SEPARATOR = std::string("/");
}

const int PluginsInstaller::INSTALLATION_ERROR = -1;

PluginsInstaller::PluginsInstaller() :
    m_initialized(false)
{
    LogDebug("PluginsInstaller created.");
}
PluginsInstaller::~PluginsInstaller()
{
    LogDebug("PluginsInstaller destroyed.");
}

void PluginsInstaller::checkDatabaseTablesExistance()
{
    if (!WrtDB::WrtDatabase::CheckTableExist("FeaturesList") ||
        !WrtDB::WrtDatabase::CheckTableExist("PluginProperties") ||
        !WrtDB::WrtDatabase::CheckTableExist("PluginDependencies") ||
        !WrtDB::WrtDatabase::CheckTableExist("PluginImplementedObjects") ||
        !WrtDB::WrtDatabase::CheckTableExist("PluginRequiredObjects") ||
        !WrtDB::WrtDatabase::CheckTableExist("DeviceCapabilities") ||
        !WrtDB::WrtDatabase::CheckTableExist("FeatureDeviceCapProxy"))
    {
        LogError("Wrong database. Required tables not exist.");
        deinitialize();
        Assert(false && "Wrong database. Required tables not exist.");
    }
}

bool PluginsInstaller::initialize()
{
    LogDebug("Initializing required systems.");

    // Check paths
    if (!PluginUtils::checkPaths()) {
        return false;
    }
    // Initialize ValidationCore - this must be done before AttachDatabases
    ValidationCore::VCoreInit(
        std::string(GlobalConfig::GetFingerprintListFile()),
        std::string(GlobalConfig::GetFingerprintListSchema()),
        std::string(GlobalConfig::GetVCoreDatabaseFilePath()));

    xmlInitParser();
    WrtDB::WrtDatabase::attachToThreadRW();
    ValidationCore::AttachToThreadRW();
    checkDatabaseTablesExistance();
    LogDebug("Initialized.");
    m_initialized = true;
    return true;
}

void PluginsInstaller::deinitialize()
{
    LogDebug("Shuting systems down.");
    m_initialized = false;
    ValidationCore::DetachFromThread();
    WrtDB::WrtDatabase::detachFromThread();
    ValidationCore::VCoreDeinit();
    xmlCleanupParser();
    LogDebug("Done");
}

PluginsInstaller::ReturnStatus PluginsInstaller::installPlugin(
    const std::string& libPath)
{
    if (!m_initialized) {
        LogError("Plugins installer not initialized.");
        return ReturnStatus::NotInitialized;
    }
    LogDebug("Plugin installation started. Checking path: " << libPath);

    if (!PluginUtils::checkPath(libPath)) {
        return ReturnStatus::WrongPluginPath;
    }

    LogDebug("Plugin path ok. Searching for config file...");

    std::string metaFileName = libPath + DIRECTORY_SEPARATOR +
        std::string(WrtDB::GlobalConfig::GetPluginMetafileName());
    if (PluginUtils::checkFileExistance(metaFileName)) {
        return installPluginFromMetafile(libPath, metaFileName);
    }

    PluginMetafileData pluginInfo;
    pluginInfo.m_libraryName = getLibraryName(libPath);

    LogDebug(
        "Config file done. Lib name: " << pluginInfo.m_libraryName
                                       <<
        ". Searching for installed plugin...");

    if (WrtDB::PluginDAO::isPluginInstalled(pluginInfo.m_libraryName)) {
        LogDebug("Plugin already installed.");
        return ReturnStatus::AlreadyInstalled;
    }
    LogDebug("Plugin not installed. Loading library file...");

    PluginObjectsPtr libraryObjects;
    PluginHandle pluginHandle;

    std::string filename = libPath + DIRECTORY_SEPARATOR +
        pluginInfo.m_libraryName;

    LogDebug("Loading plugin: " << filename);

    Plugin* plugin;
    Try
    {
        plugin = m_registry.GetPlugin(filename);
    }
    Catch(DPL::Exception) {
        LogError("Loading library failed " << filename);
        return ReturnStatus::LoadingLibraryError;
    }
    libraryObjects = PluginObjectsPtr(new PluginObjects());

    LogDebug("#####");
    LogDebug("##### Plugin: " << filename << " supports new plugin API");
    LogDebug("#####");

    FOREACH(o, *plugin->GetObjects()) {
        libraryObjects->addObjects(CAST(*o)->GetParentName(),
                                   CAST(*o)->GetName());

        LogDebug("[Parent << Object] " << CAST(*o)->GetParentName()
                                       << " << "
                                       << CAST(*o)->GetName());

        registerObjects(libraryObjects, plugin->GetObjects());
    }

    if (!fillMappingInterfaces(pluginInfo, filename)) {
        m_registry.RemovePlugin(filename, *plugin);
        return ReturnStatus::LoadingLibraryError;
    }

    LogDebug("Library loaded. Registering plugin...");

    Try
    {
        pluginHandle =
            PluginDAO::registerPlugin(pluginInfo, libPath);

        LogDebug("Plugin registered. Registering features...");

        FOREACH(it, pluginInfo.m_featureContainer)
        {
            LogError("PluginHandle: " << pluginHandle);
            FeatureDAO::RegisterFeature(*it, pluginHandle);
        }

        LogDebug("Features registered. Registering objects...");

        registerPluginObjects(pluginHandle, libraryObjects);

        LogDebug("Registration done. Resolving dependencies...");

        //TODO: can it be replaced with resolvePluginDependencies(handle)
        if (!registerAndUpdateInstallation(pluginHandle, libraryObjects)) {
            return ReturnStatus::InstallationWaiting;
        }
    } Catch(DPL::Exception) {
        LogError("Failed to make database entry.");
        return ReturnStatus::DatabaseError;
    }

    LogDebug("Plugin installed successfully.");
    return ReturnStatus::Success;
}

PluginObjectsPtr PluginsInstaller::loadLibraryFromMetafile(
    const std::string& libName) const
{
    LogDebug("Loading library: " << libName);

    void *dlHandle = dlopen(libName.c_str(), RTLD_NOW);
    if (dlHandle == NULL) {
        const char* error = (const char*)dlerror();
        LogError(
            "Failed to load plugin: " << libName <<
            ". Reason: " << (error != NULL ? error : "unknown"));
        ThrowMsg(PluginInstall::Exceptions::LibraryException, "Library error");
    }

    const js_entity_definition_t *rawEntityList = NULL;
    get_widget_entity_map_proc *getWidgetEntityMapProcPtr = NULL;

    getWidgetEntityMapProcPtr =
        reinterpret_cast<get_widget_entity_map_proc *>(dlsym(dlHandle,
                                                             PLUGIN_GET_CLASS_MAP_PROC_NAME));

    if (getWidgetEntityMapProcPtr) {
        rawEntityList = (*getWidgetEntityMapProcPtr)();
    } else {
        rawEntityList =
            static_cast<const js_entity_definition_t *>(dlsym(dlHandle,
                                                              PLUGIN_CLASS_MAP_NAME));
    }

    if (rawEntityList == NULL) {
        dlclose(dlHandle);
        LogError("Failed to read class name" << libName);
        ThrowMsg(PluginInstall::Exceptions::LibraryException, "Library error");
    }

    PluginObjectsPtr libraryObjects = PluginObjectsPtr(new PluginObjects());
    const js_entity_definition_t *rawEntityListIterator = rawEntityList;

    LogDebug("#####");
    LogDebug("##### Plugin: " << libName << " is using deprecated API");
    LogDebug("#####");

    while (rawEntityListIterator->parent_name != NULL &&
           rawEntityListIterator->object_name != NULL)
    {
        LogDebug("#####     [" << rawEntityListIterator->object_name << "]: ");
        LogDebug("#####     Parent: " << rawEntityListIterator->parent_name);
        LogDebug("#####");

        libraryObjects->addObjects(rawEntityListIterator->parent_name,
                                   rawEntityListIterator->object_name);

        ++rawEntityListIterator;
    }

    // Unload library
    if (dlclose(dlHandle) != 0) {
        LogError("Cannot close plugin handle");
    } else {
        LogDebug("Library is unloaded");
    }

    // Load export table
    LogDebug("Library successfuly loaded and parsed");

    return libraryObjects;
}

PluginsInstaller::ReturnStatus PluginsInstaller::installPluginFromMetafile(
    const std::string& path, const std::string& metaFilePath)
{
    if (!m_initialized) {
        LogError("Plugins installer not initialized.");
        return ReturnStatus::NotInitialized;
    }
    OptionalPluginMetafileData pluginData;
    Try
    {
        pluginData = parseMetafile(metaFilePath);
    }
    Catch(PluginInstall::Exceptions::XMLFileParsingException)
    {
        LogError("Parsing metafile failed.");
        return ReturnStatus::MetafileError;
    }
    if (!pluginData) {
        return ReturnStatus::MetafileError;
    }

    if (WrtDB::PluginDAO::isPluginInstalled(pluginData->m_libraryName)) {
        LogDebug("Plugin already installed.");
        return ReturnStatus::AlreadyInstalled;
    }
    Try {
        LogError(
            "path is: " << path << ", libraryName: " <<
            pluginData->m_libraryName);
        PluginObjectsPtr objects = loadLibraryFromMetafile(
                path + DIRECTORY_SEPARATOR + pluginData->m_libraryName);

        PluginHandle pluginHandle =
            PluginDAO::registerPlugin(*pluginData, path);

        LogDebug("Plugin registered. Registering features...");

        FOREACH(it, pluginData->m_featureContainer)
        {
            LogError("PluginHandle: " << pluginHandle);
            FeatureDAO::RegisterFeature(*it, pluginHandle);
        }

        LogDebug("Features registered. Registering objects...");

        registerPluginObjects(pluginHandle, objects);

        LogDebug("Objects registered. Finishing...");

        if (!registerAndUpdateInstallation(pluginHandle, objects)) {
            return ReturnStatus::InstallationWaiting;
        }
    } Catch(DPL::Exception) {
        LogError("Failed to make database entry.");
        return ReturnStatus::DatabaseError;
    }

    LogDebug("Plugin installed successfully.");
    return ReturnStatus::Success;
}

int PluginsInstaller::installAllPlugins()
{
    if (!m_initialized) {
        LogError("Plugins installer not initialized.");
        return INSTALLATION_ERROR;
    }
    LogDebug("Installing plugins ...");

    std::string PLUGIN_PATH = std::string(GlobalConfig::GetDevicePluginPath());

    DIR *dir;
    dir = opendir(PLUGIN_PATH.c_str());

    if (!dir) {
        return INSTALLATION_ERROR;
    }

    LogDebug("Plugin DIRECTORY IS" << PLUGIN_PATH);
    int return_code;
    struct dirent libdir;
    struct dirent* result;

    errno = 0;

    std::list<std::string> pluginsPaths;

    for (return_code = readdir_r(dir, &libdir, &result);
            result != NULL && return_code == 0;
            return_code = readdir_r(dir, &libdir, &result)) {
        if (strcmp(libdir.d_name, ".") == 0 ||
            strcmp(libdir.d_name, "..") == 0)
        {
            continue;
        }

        std::string path = PLUGIN_PATH;
        path += "/";
        path += libdir.d_name;

        struct stat tmp;

        if (stat(path.c_str(), &tmp) == -1) {
            LogError("Failed to open file" << path);
            continue;
        }

        if (!S_ISDIR(tmp.st_mode)) {
            LogError("Not a directory" << path);
            continue;
        }

        LogDebug("Found plugin at " << path);
        pluginsPaths.push_back(path);
    }

    if (0 != return_code)
        LogError("Error while reading directory.");

    if (-1 == closedir(dir)) {
        LogError("Failed to close dir: " << PLUGIN_PATH);
    }

    LogDebug("Plugins to install: " << pluginsPaths.size());

    for (size_t k = 0; k <= pluginsPaths.size(); ++k) {
        printf(" ");
    }
    printf("]\r[");
    int installedPluginsCount = 0;
    ReturnStatus ret = ReturnStatus::Unknown;
    FOREACH(it, pluginsPaths) {
        LogDebug("Preparing to plugin installation: " << *it);
        ret = installPlugin(*it);
        if (ReturnStatus::Success == ret) {
            ++installedPluginsCount;
            LogDebug("Plugin " << *it << " installed.");
        } else if (ReturnStatus::InstallationWaiting == ret) {
            LogWarning("Plugin not installed. Waiting for dependency");
        } else {
            LogError("Plugin installation failed");
        }
        printf("#");
        fflush(stdout);
    }
    printf("\n");
    installedPluginsCount += installWaitingPlugins();
    m_registry.UnloadAll();
    LogDebug("Installed " << installedPluginsCount
                         << " plugins of total: " << pluginsPaths.size());
    return installedPluginsCount;
}

int PluginsInstaller::installWaitingPlugins()
{
    PluginHandleSetPtr waitingPlugins;

    waitingPlugins =
        PluginDAO::getPluginHandleByStatus(PluginDAO::INSTALLATION_WAITING);

    int pluginsInstalled = 0;
    FOREACH(it, *waitingPlugins)
    {
        if (resolvePluginDependencies(*it)) {
            ++pluginsInstalled;
        }
    }
    return pluginsInstalled;
}

bool PluginsInstaller::resolvePluginDependencies(PluginHandle handle)
{
    PluginHandleSetPtr dependencies(new PluginHandleSet);

    PluginObjects::ObjectsPtr requiredObjects =
        PluginDAO::getRequiredObjectsForPluginHandle(handle);

    PluginHandle depHandle = INVALID_PLUGIN_HANDLE;

    FOREACH(requiredObject, *requiredObjects)
    {
        depHandle =
            PluginDAO::getPluginHandleForImplementedObject(*requiredObject);

        if (INVALID_PLUGIN_HANDLE == depHandle) {
            LogError("Library implementing: " <<
                     *requiredObject << " NOT FOUND");
            return false;
        }
        dependencies->insert(depHandle);
    }

    PluginDAO::registerPluginLibrariesDependencies(handle, dependencies);
    PluginDAO::setPluginInstallationStatus(handle,
                                           PluginDAO::INSTALLATION_COMPLETED);

    return true;
}

void PluginsInstaller::registerObjects(const PluginObjectsPtr& libObj,
                                       const IObjectsListPtr& objects) const
{
    LogDebug("registerObjects invoked");

    FOREACH(o, *objects)
    {
        auto children = CAST(*o)->GetChildren();

        if (children) {
            FOREACH(c, *children)
            {
                libObj->addObjects(CAST(*o)->GetName(), CAST(*c)->GetName());

                LogDebug("[Parent << Object] " << CAST(*c)->GetName()
                                               << " << "
                                               << CAST(*o)->GetName());
            }

            registerObjects(libObj, children);
        }
    }
}

PluginsInstaller::OptionalPluginMetafileData PluginsInstaller::parseMetafile(
    const std::string& path) const
{
    LogDebug("Plugin Config file::" << path);
    Try
    {
        PluginMetafileData pluginInfo;
        PluginMetafileReader reader;
        reader.initialize(path);
        reader.read(pluginInfo);

        FOREACH(it, pluginInfo.m_featureContainer) {
            LogDebug("Parsed feature : " << it->m_name);

            FOREACH(devCap, it->m_deviceCapabilities) {
                LogDebug("  |  DevCap : " << *devCap);
            }
        }
        return OptionalPluginMetafileData(pluginInfo);
    }
    Catch(ValidationCore::ParserSchemaException::Base) {
        LogError("Error during file processing " << path);
        ThrowMsg(PluginInstall::Exceptions::XMLFileParsingException,
                 "Parsing metafile failed");
    }
}

std::string PluginsInstaller::getLibraryName(const std::string& dirPath) const
{
    std::string pluginPath = dirPath;
    size_t indexpos = pluginPath.find_last_of('/');

    if (std::string::npos == indexpos) {
        indexpos = 0;
    } else {
        indexpos += 1;  // move after '/'
    }

    std::string libName = pluginPath.substr(indexpos);
    libName = WrtDB::GlobalConfig::GetPluginPrefix() + libName
        + WrtDB::GlobalConfig::GetPluginSuffix();
    LogDebug("Plugin .so: " << libName);
    return libName;
}

bool PluginsInstaller::registerAndUpdateInstallation(
    const WrtDB::DbPluginHandle& pluginHandle,
    const PluginObjectsPtr& libraries)
{
    PluginHandleSetPtr handles = PluginHandleSetPtr(new PluginHandleSet);

    DbPluginHandle handle = INVALID_PLUGIN_HANDLE;

    //register requiredObjects
    FOREACH(it, *(libraries->getDependentObjects()))
    {
        if (libraries->hasObject(*it)) {
            LogDebug("Dependency from the same library. ignored");
            continue;
        }

        handle = PluginDAO::getPluginHandleForImplementedObject(*it);
        if (handle == INVALID_PLUGIN_HANDLE) {
            LogError("Library implementing: " << *it << " NOT FOUND");
            PluginDAO::setPluginInstallationStatus(
                pluginHandle,
                PluginDAO::INSTALLATION_WAITING);
            return false;
        }

        handles->insert(handle);
    }

    PluginDAO::registerPluginLibrariesDependencies(pluginHandle, handles);

    PluginDAO::setPluginInstallationStatus(pluginHandle,
                                           PluginDAO::INSTALLATION_COMPLETED);
    return true;
}

bool PluginsInstaller::fillMappingInterfaces(PluginMetafileData& pluginData,
                                             const std::string& filename)
{
    void *dlHandle = dlopen(filename.c_str(), RTLD_NOW);
    if (dlHandle == NULL) {
        const char* error = (const char*)dlerror();
        LogError(
            "Failed to load plugin: " << filename << ". Reason: " << (error != NULL ? error : "unknown"));
        return false;
    }
    Try
    {
        ExportedApi* entryPoint =
            static_cast<ExportedApi*>(dlsym(dlHandle, GetExportedSymbolName()));
        if (NULL == entryPoint) {
            const char* error = (const char*)dlerror();
            LogError("Error: " << (error != NULL ? error : "unknown"));
            ThrowMsg(PluginInstall::Exceptions::LibraryException,
                     "Library error");
        }

        // obtain feature -> dev-cap mapping
        feature_mapping_interface_t mappingInterface = { NULL, NULL, NULL };
        entryPoint->GetProvidedFeatures(&mappingInterface);

        if (!mappingInterface.featGetter || !mappingInterface.release ||
            !mappingInterface.dcGetter)
        {
            LogError("Failed to obtain mapping interface from .so");
            ThrowMsg(PluginInstall::Exceptions::LibraryException,
                     "Library error");
        }

        feature_mapping_t* devcapMapping = mappingInterface.featGetter();

        LogDebug("Getting mapping from features to device capabilities");

        for (size_t i = 0; i < devcapMapping->featuresCount; ++i) {
            PluginMetafileData::Feature feature;
            feature.m_name = devcapMapping->features[i].feature_name;

            LogDebug("Feature: " << feature.m_name);

            const devcaps_t* dc =
                mappingInterface.dcGetter(
                    devcapMapping,
                    devcapMapping->features[i].
                        feature_name);

            LogDebug("device=cap: " << dc);

            if (dc) {
                LogDebug("devcaps count: " << dc->devCapsCount);

                for (size_t j = 0; j < dc->devCapsCount; ++j) {
                    LogDebug("devcap: " << dc->deviceCaps[j]);
                    feature.m_deviceCapabilities.insert(dc->deviceCaps[j]);
                }
            }

            pluginData.m_featureContainer.insert(feature);
        }

        mappingInterface.release(devcapMapping);
    } Catch(PluginInstall::Exceptions::PluginsInstallerException)
    {
        LogError("Exception while feature mapping");
        dlclose(dlHandle);
        return false;
    }

    // Unload library
    if (dlclose(dlHandle) != 0) {
        LogError("Cannot close plugin handle");
    } else {
        LogDebug("Library is unloaded");
    }
    return true;
}

void PluginsInstaller::registerPluginObjects(
    const WrtDB::DbPluginHandle& handle,
    const PluginObjectsPtr libObjects)
const
{
    //register implemented objects
    PluginObjects::ObjectsPtr objects =
        libObjects->getImplementedObject();

    FOREACH(it, *objects)
    {
        WrtDB::PluginDAO::registerPluginImplementedObject(*it, handle);
    }

    //register requiredObjects
    objects = libObjects->getDependentObjects();

    FOREACH(it, *objects)
    {
        if (libObjects->hasObject(*it)) {
            LogDebug("Dependency from the same library. ignored");
            continue;
        }
        WrtDB::PluginDAO::registerPluginRequiredObject(*it, handle);
    }
}

