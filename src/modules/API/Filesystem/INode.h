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
#ifndef WRTDEVICEAPIS_FILESYSTEM_INODE_H_
#define WRTDEVICEAPIS_FILESYSTEM_INODE_H_

#include <string>
#include <vector>
#include <ctime>
#include <dpl/noncopyable.h>
#include <dpl/shared_ptr.h>
#include <Commons/EventReceiver.h>
#include "EventListNodes.h"
#include "EventOpen.h"
#include "EventReadText.h"
#include "Enums.h"
#include "IPath.h"
#include "IStream.h"
#include "INodeTypes.h"
#include "NodeFilter.h"

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class INode :
    public Commons::EventRequestReceiver<EventListNodes>,
    public Commons::EventRequestReceiver<EventOpen>,
    public Commons::EventRequestReceiver<EventReadText>
{
  public:
    typedef std::vector<std::string> NameList;
    typedef NameList::iterator NodeListIterator;

  public:
    virtual ~INode() = 0;

    /**
     * Gets direct child of this node.
     * @param path Path to the child node.
     * @return Ptr to the child node.
     * @remarks Ownership passed to the caller.
     */
    virtual INodePtr getChild(const IPathPtr& path) = 0;

    /**
     * Gets path of current node.
     * @return Node's path.
     */
    virtual IPathPtr getPath() const = 0;

    /**
     * Gets type of current node.
     * @return Node's type.
     */
    virtual NodeType getType() const = 0;

    /**
     * Gets permissions of the virtual node (not real filesystem node).
     * @return Node's permissions.
     */
    virtual int getPermissions() const = 0;

    /**
     * Sets permissions on the virtual node (not real filesystem node).
     * @param perms Node's permissions @see Api::Filesystem::Permissions.
     */
    virtual void setPermissions(int perms) = 0;

    /**
     * Gets list of names of direct child nodes.
     * @return Names of child nodes.
     */
    virtual NameList getChildNames() const = 0;

    /**
     * Gets list of direct child nodes.
     * @return Child nodes.
     * @remarks Ownership passed to the caller.
     * @deprecated
     */
    virtual NodeList getChildNodes(const NodeFilterPtr& filter =
                                       NodeFilterPtr()) const /*DEPREC*/ = 0;

    /**
     * Gets list of direct descendant nodes.
     * @param event Get child nodes event.
     */
    virtual void getChildNodes(const EventListNodesPtr& event) = 0;

    /**
     * Gets stream for this node.
     * @param mode @see Api::Filesystem::AccessMode.
     * @return Stream connected with current node.
     * @deprecated Use async version of thi method instead.
     */
    virtual IStreamPtr open(int mode) = 0;

    /**
     * Gets stream for this node.
     * @param mode @see Api::Filesystem::AccessMode.
     * @return Stream connected with current node.
     */
    virtual void open(const EventOpenPtr& event) = 0;

    /**
     * Removes underlying filesystem node.
     * @param options Removal options (by default removal is recursive).
     * @remarks Synchronous.
     * Valid options:
     * - OPT_RECURSIVE - remove node recursively.
     */
    virtual void remove(int options = OPT_RECURSIVE) = 0;

    /**
     * Creates child of current node.
     * @param path Path to the node to create.
     * @param type Type of the node @see Api::Filesystem::NodeType.
     * @param options Additional options see remarks (no options by default).
     * @return Ptr to newly created node.
     * @remarks
     * Valid options:
     * - OPT_RECURSIVE - attempt to create all necessary sub-directories
     */
    virtual INodePtr createChild(const IPathPtr& path,
                                 NodeType type,
                                 int options = OPT_NONE) = 0;

    /**
     * Gets size of this node.
     * @return Size.
     */
    virtual std::size_t getSize() const = 0;

    /**
     * Gets creation date of this node.
     * @return Date.
     */
    virtual std::time_t getCreated() const = 0;

    /**
     * Gets last modification date of this node.
     * @return Date.
     */
    virtual std::time_t getModified() const = 0;

    /**
     * Gets parent of this node.
     * @return Parent node or NULL if no parent (e.g. in case of a root node).
     */
    virtual INodePtr getParent() const = 0;

    /**
     * Gets platform permissions.
     * @return Platform permissions (set of flags from @see Permissions enum).
     */
    virtual int getMode() const = 0;

    /**
     * Reads whole file as text.
     * @param event Read file event.
     */
    virtual void read(const EventReadTextPtr& event) = 0;

    virtual std::string toUri(int widgetId) const = 0;

  protected:
    INode();
};
} // API
} // Filesystem
} // WrtDeviceApis

#endif /* WRTDEVICEAPIS_FILESYSTEM_INODE_H_ */
