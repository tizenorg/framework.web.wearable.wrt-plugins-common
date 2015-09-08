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
#ifndef WRTDEVICEAPIS_FILESYSTEM_MANAGER_H_
#define WRTDEVICEAPIS_FILESYSTEM_MANAGER_H_

#include <map>
#include <vector>
#include <cstddef>
#include <dpl/shared_ptr.h>
#include <Filesystem/IManager.h>
#include <Filesystem/IPath.h>
#include "Node.h"
#include "Path.h"

namespace WrtDeviceApis {
namespace Filesystem {
class Manager : public Api::IManager
{
  public:
    /**
     * Checks if file exists.
     * @param real file path.
     * @return true when file exists, false otherwise.
     * @throw PlatformException If unable to validate if file exists.
     */
    static bool fileExists(const std::string &file);

  public:
    Manager();
    ~Manager();

    Api::IPathPtr getBasePath() const;

    Api::IPathPtr getLocationPath(Api::LocationType type) const;

    Api::LocationPaths getLocationPaths() const;

    Api::LocationTypes getLocations() const;

    void getNode(const Api::EventResolvePtr& event);

    std::size_t getMaxPathLength() const;

    void copy(const Api::EventCopyPtr& event);

    void move(const Api::EventMovePtr& event);

    void remove(const Api::EventRemovePtr& event);

    void find(const Api::EventFindPtr& event);

    /**
     * Finds files in the filesystem whish match specified filters.
     * @param path Starting path.
     * @param filter Filters @see FindFilters.
     * @param result List with found files.
     * @param event DPL event, if set then search process can be cancelled
     * @remarks For filter FF_NAME value is treated as perl like regex pattern.
     *          Use @Regex::escpae() if you want to treat it as normal string.
     */
    void find(const Api::IPathPtr& path,
              const Api::FiltersMap& filter,
              Api::NodeList& result,
              const Api::EventFindPtr& event = Api::EventFindPtr(NULL));

    /**
     * @warning Operates on specified as it is - it DOES NOT take its real path.
     *          This is because all filesystem operation will probably change
     *          their approach to real path and will allow to refer to nodes
     *          only by virtual roots.
     */
    bool access(const Api::IPathPtr& path,
                int accessType) const;

    void addOpenedNode(const Api::INodePtr& node);
    void removeOpenedNode(const Api::INodePtr& node);
    bool checkIfOpened(const Api::IPathPtr& path) const;

  protected:
    bool matchFilters(const std::string& name,
                      const struct stat& info,
                      const Api::FiltersMap& filter);

    void OnRequestReceived(const Api::EventResolvePtr& event);
    void OnRequestReceived(const Api::EventCopyPtr& event);
    void OnRequestReceived(const Api::EventMovePtr& event);
    void OnRequestReceived(const Api::EventRemovePtr& event);
    void OnRequestReceived(const Api::EventFindPtr& event);

  private:
    typedef std::map<Api::LocationType, Api::IPathPtr> Locations;

  private:
    /**
     * Initializes statics of Manager class.
     * Used only internally.
     * @return True on success, false otherwsie.
     */
    static bool init();

    static void setupLocation(Api::LocationType location,
                              const char* path);

    void copyElement(const std::string &src,
                     const std::string &dest,
                     bool recursive = true) const;

    /**
     * Check two paths to copy/move. Checks if they are not the same and
     * required permissions.
     * @param src
     * @param dest
     */
    void checkPaths(Api::IPathPtr &src, Api::IPathPtr &dest);
    bool pathExists(const std::string &path);

  private:
    static Locations m_locations; ///< Paths to default locations.
    static const std::size_t m_maxPathLength; ///< Maximum path length.
    static Api::NodeList m_openedNodes; ///< List of nodes that has opened
                                        // streams.
};
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_MANAGER_H_
