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
#ifndef WRTDEVICEAPIS_FILESYSTEM_NODE_H_
#define WRTDEVICEAPIS_FILESYSTEM_NODE_H_

#include <ctime>
#include <cstddef>
#include <sys/stat.h>
#include <set>
#include <dpl/shared_ptr.h>
#include <dpl/enable_shared_from_this.h>
#include <dpl/mutex.h>

#include <Filesystem/Enums.h>
#include <Filesystem/INode.h>
#include "Path.h"
#include "Stream.h"

namespace WrtDeviceApis {
namespace Filesystem {
class Node :
    public Api::INode,
    public DPL::EnableSharedFromThis<Node>
{
  public:
    static Api::INodePtr resolve(const Api::IPathPtr& path);

  public:
    Api::IPathPtr getPath() const;
    Api::NodeType getType() const;
    int getPermissions() const;
    void setPermissions(int perms);
    std::size_t getSize() const;
    std::time_t getCreated() const;
    std::time_t getModified() const;
    Api::INodePtr getParent() const;
    int getMode() const;

    Api::INodePtr getChild(const Api::IPathPtr& path);
    NameList getChildNames() const;
    Api::NodeList getChildNodes(
        const Api::NodeFilterPtr& filter = Api::NodeFilterPtr()) const;
    void getChildNodes(const Api::EventListNodesPtr& event);
    Api::INodePtr createChild(const Api::IPathPtr & path,
                              Api::NodeType,
                              int options);
    Api::IStreamPtr open(int mode);
    void open(const Api::EventOpenPtr& event);
    void remove(int options);
    void read(const Api::EventReadTextPtr& event);

    void onStreamClose(const StreamPtr& stream);

  private:
    typedef std::set<StreamPtr> StreamList;

  private:
    static bool exists(const Api::IPathPtr& path);
    static struct stat stat(const Api::IPathPtr& path);

  private:
    Node(const Api::IPathPtr& path, Api::NodeType type);

    Node* createAsFile(const Api::IPathPtr& path,
                       int options);
    void createAsFileInternal(const Api::IPathPtr& path);

    Node* createAsDirectory(const Api::IPathPtr& path,
                            int options);
    void createAsDirectoryInternal(const Api::IPathPtr& path);

    void removeAsFile(const Api::IPathPtr& path);
    void removeAsDirectory(const Api::IPathPtr& path,
                           bool recursive);

    void OnRequestReceived(const Api::EventListNodesPtr& event);
    void OnRequestReceived(const Api::EventOpenPtr& event);

    void OnRequestReceived(const Api::EventReadTextPtr& event);
    std::string readText();

    std::string toUri(int widgetId) const;

  private:
    Api::IPathPtr m_path;
    Api::NodeType m_type;
    int m_perms;
    StreamList m_openStreams;
    mutable DPL::Mutex m_openStreamsMutex;
};

typedef DPL::SharedPtr<Node> NodePtr;
} // Filesystem
} // WrtDeviceApis

#endif /* WRTDEVICEAPIS_FILESYSTEM_NODE_H_ */
