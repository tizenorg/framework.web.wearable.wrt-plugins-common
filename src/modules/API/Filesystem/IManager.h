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
#ifndef WRTDEVICEAPIS_FILESYSTEM_IMANAGER_H_
#define WRTDEVICEAPIS_FILESYSTEM_IMANAGER_H_

#include <vector>
#include <string>
#include <cstddef>
#include <dpl/noncopyable.h>
#include <Commons/EventReceiver.h>
#include <Filesystem/EventResolve.h>
#include <Filesystem/EventCopy.h>
#include <Filesystem/EventMove.h>
#include <Filesystem/EventRemove.h>
#include <Filesystem/EventFind.h>
#include <Filesystem/Enums.h>
#include <Filesystem/INode.h>
#include <Filesystem/IPath.h>

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
typedef std::vector<IPathPtr> LocationPaths;
typedef std::vector<LocationType> LocationTypes;

class IManager :
    public Commons::EventRequestReceiver<EventResolve>,
    public Commons::EventRequestReceiver<EventCopy>,
    public Commons::EventRequestReceiver<EventMove>,
    public Commons::EventRequestReceiver<EventRemove>,
    public Commons::EventRequestReceiver<EventFind>
{
  public:
    static IManager& getInstance();
    static bool fileExists(const std::string &file);

    virtual ~IManager() = 0;

    /**
     * Gets base path.
     * @return Valid path or empty shared pointer.
     */
    virtual IPathPtr getBasePath() const = 0;

    /**
     * Gets path for specified location type.
     * @param type Location type @see
     * WrtDeviceApis::Api::Filesystem::LocationType.
     * @return Valid path or empty shared pointer.
     */
    virtual IPathPtr getLocationPath(LocationType type) const = 0;

    /**
     * Gets paths to default locations.
     * @return Paths to predefined virtual locations.
     */
    virtual LocationPaths getLocationPaths() const = 0;

    /**
     * Gets locations supported by platform.
     * @return Supported locations.
     */
    virtual LocationTypes getLocations() const = 0;

    /**
     * Gets filesystem node.
     * @param event Get node event @see Api::Filesystem::EventGetNode.
     * @remarks Asynchronous.
     */
    virtual void getNode(const EventResolvePtr& event) = 0;

    /**
     * Gets maximum length of filesystem path.
     * @return Maximum path length.
     */
    virtual std::size_t getMaxPathLength() const = 0;

    /**
     * Copies node to specified destination.
     * @param event Copy node event @see Api::Filesystem::EventCopy.
     * @remarks Asynchronous.
     */
    virtual void copy(const EventCopyPtr& event) = 0;

    /**
     * Moves node to specified destination.
     * @param event Move node event @see Api::Filesystem::EventMove.
     * @remarks Asynchronous.
     */
    virtual void move(const EventMovePtr& event) = 0;

    /**
     * Removes node.
     * @param event Remove node event @see Api::Filesystem::EventRemove.
     * @remarks Asynchronous.
     */
    virtual void remove(const EventRemovePtr& event) = 0;

    /**
     * Finds nodes.
     * @param event Find nodes event @see Api::Filesystem::EventFind.
     * @remarks Asynchronous.
     */
    virtual void find(const EventFindPtr& event) = 0;

    /**
     * Checks if node at specified path has supplied access rights.
     * @param path Path to the node.
     * @param accessType Access right(s) to check @see AccessType. Multiple
     * values
     *                   can be passed using OR operator.
     * @return True if specified node has supplied access rights, false
     * otherwise.
     */
    virtual bool access(const IPathPtr& path,
                        int accessType) const = 0;

    virtual void addOpenedNode(const INodePtr& node) = 0;
    virtual void removeOpenedNode(const INodePtr& node) = 0;
    virtual bool checkIfOpened(const IPathPtr& path) const = 0;

  protected:
    IManager();

    virtual void OnRequestReceived(const EventResolvePtr& event) = 0;
    virtual void OnRequestReceived(const EventCopyPtr& event) = 0;
    virtual void OnRequestReceived(const EventMovePtr& event) = 0;
    virtual void OnRequestReceived(const EventRemovePtr& event) = 0;
    virtual void OnRequestReceived(const EventFindPtr& event) = 0;
}; // IManager
} // API
} // Filesystem
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_FILESYSTEM_IMANAGER_H_
