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
#include "Node.h"
#include <algorithm>
#include <memory>
#include <typeinfo>
#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <fts.h>
#include <dirent.h>
#include <errno.h>
#include <pcrecpp.h>
#include <sstream>
#include <Commons/Exception.h>
#include <Filesystem/PathUtils.h>
#include <Filesystem/Enums.h>
#include "Manager.h"
#include "NodeFilterMatcher.h"

namespace WrtDeviceApis {
namespace Filesystem {
using namespace Api;

INodePtr Node::resolve(const IPathPtr& path)
{
    struct stat info;
    if (lstat(path->getFullPath().c_str(), &info) != 0) {
        LogError("File: " << path->getFullPath().c_str());
        ThrowMsg(Commons::PlatformException,
                 "Node does not exist or access denied.");
    }

    if (!S_ISDIR(info.st_mode) && !S_ISREG(info.st_mode)) {
        ThrowMsg(Commons::PlatformException,
                 "Platform node is of unsupported type.");
    }

    NodeType type = S_ISDIR(info.st_mode) ? NT_DIRECTORY : NT_FILE;
    NodePtr result(new Node(path, type));
    return DPL::StaticPointerCast<INode>(result);
}

IPathPtr Node::getPath() const
{
    return IPath::create(m_path->getFullPath());
}

INodePtr Node::getChild(const IPathPtr& path)
{
    if (m_type != NT_DIRECTORY) {
        ThrowMsg(Commons::PlatformException, "Not a directory.");
    }
    return Node::resolve(*m_path + *path);
}

NodeType Node::getType() const
{
    return m_type;
}

int Node::getPermissions() const
{
    return m_perms;
}

void Node::setPermissions(int perms)
{
    m_perms = perms;
}

Node::NameList Node::getChildNames() const
{
    if (m_type != NT_DIRECTORY) {
        ThrowMsg(Commons::PlatformException, "Node is not directory.");
    }

    if ((m_perms & PERM_READ) == 0) {
        ThrowMsg(Commons::SecurityException, "No permission.");
    }

    DIR* dir = opendir(m_path->getFullPath().c_str());
    if (!dir) {
        ThrowMsg(Commons::PlatformException,
                 "Node has been deleted from platform.");
    }

    NameList result;
    errno = 0;
    int return_code;
    struct dirent entry;
    struct dirent *entry_result;
    for (return_code = readdir_r(dir, &entry, &entry_result);
            entry_result != NULL && return_code == 0;
            return_code = readdir_r(dir, &entry, &entry_result)) {
        if (!strncmp(entry.d_name, ".", 1) ||
            !strncmp(entry.d_name, "..", 2))
        {
            continue;
        }
        result.push_back(entry.d_name);
    }
    if (return_code != 0 || errno != 0) {
        ThrowMsg(Commons::PlatformException, "Error while reading directory.");
    }

    if (closedir(dir) != 0) {
        ThrowMsg(Commons::PlatformException, "Could not close platform node.");
    }

    return result;
}

NodeList Node::getChildNodes(const NodeFilterPtr& filter) const
{
    if (m_type != NT_DIRECTORY) {
        ThrowMsg(Commons::PlatformException, "Node is not directory.");
    }

    if ((m_perms & PERM_READ) == 0) {
        ThrowMsg(Commons::SecurityException, "No permission.");
    }

    DIR* dir = opendir(m_path->getFullPath().c_str());
    if (!dir) {
        ThrowMsg(Commons::PlatformException,
                 "Node has been deleted from platform.");
    }

    errno = 0;
    NodeList result;
    int return_code;
    struct dirent entry;
    struct dirent *entry_result;
    for (return_code = readdir_r(dir, &entry, &entry_result);
            entry_result != NULL && return_code == 0;
            return_code = readdir_r(dir, &entry, &entry_result)) {
        if (!strncmp(entry.d_name, ".", 1) ||
            !strncmp(entry.d_name, "..", 2))
        {
            continue;
        }
        Try {
            Assert(m_path);
            INodePtr node = Node::resolve(*m_path + entry.d_name);
            node->setPermissions(getPermissions()); // inherit access rights
            if (NodeFilterMatcher::match(node, filter)) {
                result.push_back(node);
            }
        }
        Catch(Commons::PlatformException) {}
    }

    if (return_code != 0 || errno != 0) {
        ThrowMsg(Commons::PlatformException, "Error while reading directory.");
    }

    if (closedir(dir) != 0) {
        ThrowMsg(Commons::PlatformException, "Could not close platform node.");
    }

    return result;
}

void Node::getChildNodes(const EventListNodesPtr& event)
{
    LogDebug("ENTER");
    EventRequestReceiver<EventListNodes>::PostRequest(event);
}

INodePtr Node::createChild(
    const IPathPtr& path,
    NodeType type,
    int options)
{
    if (m_type != NT_DIRECTORY) {
        ThrowMsg(Commons::PlatformException, "Parent node is not a directory.");
    }

    if ((m_perms & PERM_WRITE) == 0) {
        ThrowMsg(Commons::SecurityException, "Not enough permissions.");
    }

    Assert(m_path);
    Assert(path);
    IPathPtr childPath = *m_path + *path;
    if (exists(childPath)) {
        ThrowMsg(Commons::PlatformException, "Node already exists.");
    }

    NodePtr result;
    switch (type) {
    case NT_FILE:
        result.Reset(createAsFile(childPath, options));
        break;
    case NT_DIRECTORY:
        result.Reset(createAsDirectory(childPath, options));
        break;
    default:
        ThrowMsg(Commons::PlatformException, "Unsupported node type.");
    }
    if (!!result) {
        result->m_perms = m_perms;
    } else {
        ThrowMsg(Commons::PlatformException, "Node creation error");
    }

    return DPL::StaticPointerCast<INode>(result);
}

IStreamPtr Node::open(int mode)
{
    if (m_type == NT_DIRECTORY) {
        ThrowMsg(Commons::PlatformException,
                 "Cannot attach stream to directory.");
    }

    if (((mode & AM_READ) && ((m_perms & PERM_READ) == 0)) ||
        (((mode & AM_WRITE) ||
          (mode & AM_APPEND)) && ((m_perms & PERM_WRITE) == 0)))
    {
        ThrowMsg(Commons::SecurityException, "Not enough permissions.");
    }

    DPL::Mutex::ScopedLock lock(&m_openStreamsMutex);
    StreamPtr stream(new Stream(SharedFromThis(), mode));
    m_openStreams.insert(stream);
    IManager::getInstance().addOpenedNode(DPL::StaticPointerCast<INode>(
                                              SharedFromThis()));
    return DPL::StaticPointerCast<IStream>(stream);
}

void Node::open(const EventOpenPtr& event)
{
    LogDebug("ENTER");
    EventRequestReceiver<EventOpen>::PostRequest(event);
}

void Node::remove(int options)
{
    switch (m_type) {
    case NT_FILE:
        removeAsFile(m_path);
        break;
    case NT_DIRECTORY:
        removeAsDirectory(m_path, (options & OPT_RECURSIVE));
        break;
    }
}

std::size_t Node::getSize() const
{
    if (m_type == NT_DIRECTORY) {
        ThrowMsg(Commons::PlatformException,
                 "Getting size for directories is not supported.");
    }

    struct stat info = stat(m_path);
    if (!S_ISREG(info.st_mode)) {
        ThrowMsg(Commons::PlatformException,
                 "Specified node is not a regular file.");
    }

    return info.st_size;
}

std::time_t Node::getCreated() const
{
    return stat(m_path).st_ctime;
}

std::time_t Node::getModified() const
{
    return stat(m_path).st_mtime;
}

// TODO Optimize it, maybe store a flag indicating that node is a root.
INodePtr Node::getParent() const
{
    LocationPaths roots = IManager::getInstance().getLocationPaths();
    for (LocationPaths::iterator it = roots.begin(); it != roots.end(); ++it) {
        Assert(*it);
        Assert(m_path);
        if (*(*it) == *m_path) {
            return INodePtr();
        }
    }
    return Node::resolve(IPath::create(m_path->getPath()));
}

int Node::getMode() const
{
    int result = 0;
    struct stat info = stat(m_path);
    if (info.st_mode & S_IRUSR) {
        result |= PM_USER_READ;
    }
    if (info.st_mode & S_IWUSR) {
        result |= PM_USER_WRITE;
    }
    if (info.st_mode & S_IXUSR) {
        result |= PM_USER_EXEC;
    }
    if (info.st_mode & S_IRGRP) {
        result |= PM_GROUP_READ;
    }
    if (info.st_mode & S_IWGRP) {
        result |= PM_GROUP_WRITE;
    }
    if (info.st_mode & S_IXGRP) {
        result |= PM_GROUP_EXEC;
    }
    if (info.st_mode & S_IROTH) {
        result |= PM_OTHER_READ;
    }
    if (info.st_mode & S_IWOTH) {
        result |= PM_OTHER_WRITE;
    }
    if (info.st_mode & S_IXOTH) {
        result |= PM_OTHER_EXEC;
    }
    return result;
}

void Node::read(const EventReadTextPtr& event)
{
    LogDebug("ENTER");
    EventRequestReceiver<EventReadText>::PostRequest(event);
}

void Node::onStreamClose(const StreamPtr& stream)
{
    {
        DPL::Mutex::ScopedLock lock(&m_openStreamsMutex);
        m_openStreams.erase(stream);
    }
    if (m_openStreams.empty()) {
        IManager::getInstance().removeOpenedNode(DPL::StaticPointerCast<INode>(
                                                     SharedFromThis()));
    }
}

bool Node::exists(const IPathPtr& path)
{
    struct stat info;
    memset(&info, 0, sizeof(struct stat));
    int status = lstat(path->getFullPath().c_str(), &info);
    if ((status == 0) || ((status != 0) && (errno != ENOENT))) {
        return true;
    }
    return false;
}

struct stat Node::stat(const IPathPtr& path)
{
    struct stat result;
    memset(&result, 0, sizeof(struct stat));
    if (::stat(path->getFullPath().c_str(),
               &result) != 0)
    {
        LogError("File: " << path->getFullPath().c_str());
        ThrowMsg(Commons::PlatformException, "Node does not exist or no access");
    }
    return result;
}

Node::Node(const IPathPtr& path,
           NodeType type) :
    m_path(path),
    m_type(type),
    m_perms(PERM_NONE)
{}

Node* Node::createAsFile(const IPathPtr& path,
                         int /* options */)
{
    LogDebug("ENTER");
    createAsFileInternal(path);
    return new Node(path, NT_FILE);
}

void Node::createAsFileInternal(const IPathPtr& path)
{
    LogDebug("ENTER");
    FILE* file = std::fopen(path->getFullPath().c_str(), "wb");
    if (!file) {
        ThrowMsg(Commons::PlatformException,
                 "Platform node could not be created.");
    }
    std::fclose(file);
}

Node* Node::createAsDirectory(const IPathPtr& path,
                              int options)
{
    if (options & OPT_RECURSIVE) {
        PathUtils::PathList parts = PathUtils::getParts(path);
        PathUtils::PathListIterator it = parts.begin();
        for (; it != parts.end(); ++it) {
            if (!exists(*it)) {
                createAsDirectoryInternal(*it);
            }
        }
    }
    createAsDirectoryInternal(path);
    return new Node(path, NT_DIRECTORY);
}

void Node::createAsDirectoryInternal(const IPathPtr& path)
{
    if (mkdir(path->getFullPath().c_str(), S_IRWXU | S_IRWXG | S_IROTH |
              S_IXOTH) != 0)
    {
        ThrowMsg(Commons::PlatformException,
                 "Platform node could not be created.");
    }
}

void Node::removeAsFile(const IPathPtr& path)
{
    DPL::Mutex::ScopedLock lock(&m_openStreamsMutex);
    if (!m_openStreams.empty()) {
        ThrowMsg(Commons::PlatformException, "Node is locked for I/O.");
    }
    if (IManager::getInstance().checkIfOpened(path)) {
        ThrowMsg(Commons::PlatformException, "Node is locked for I/O.");
    }

    if (unlink(path->getFullPath().c_str()) != 0) {
        ThrowMsg(Commons::PlatformException,
                 "Error while removing platform node.");
    }
}

void Node::removeAsDirectory(const IPathPtr& path,
                             bool recursive)
{
    Assert(path);
    if (recursive) {
        FTS *fts;
        FTSENT *ftsent;
        int error = 0;
        std::string pth = path->getFullPath();
        char * const paths[] = { const_cast<char * const>(pth.c_str()), NULL };

        if ((fts =
                 fts_open(paths, FTS_PHYSICAL | FTS_NOCHDIR, NULL)) == NULL)
        {
            //ERROR
            error = errno;
            LogError(__PRETTY_FUNCTION__ << ": fts_open on "
                                         << pth
                                         << " failed with error: "
                                         << strerror(error));
            ThrowMsg(Commons::PlatformException, "Failed to traverse Node");
        }

        while ((ftsent = fts_read(fts)) != NULL) {
            switch (ftsent->fts_info) {
            case FTS_D:
                //directory in preorder - do nothing
                break;
            case FTS_DP:
                //directory in postorder - remove
                errno = 0;
                if (rmdir(ftsent->fts_accpath) != 0) {
                    if (errno == EEXIST) {
                        ThrowMsg(Commons::PlatformException,
                                 "Node has child nodes.");
                    }
                    ThrowMsg(Commons::PlatformException,
                             "Error while removing platform node.");
                }
                break;
            case FTS_DC:
            case FTS_F:
            case FTS_NSOK:
            case FTS_SL:
            case FTS_SLNONE:
            case FTS_DEFAULT:
            {
                //regular files and other objects that can safely be removed
                IPathPtr file_path = IPath::create(ftsent->fts_path);
                removeAsFile(file_path);
                break;
            }
            case FTS_NS:
            case FTS_DOT:
            case FTS_DNR:
            case FTS_ERR:
            default:
                LogWarning(__PRETTY_FUNCTION__
                           << ": traversal failed with error: "
                           << strerror(ftsent->fts_errno));
                break;
            }
        }

        if (fts_close(fts) == -1) {
            error = errno;
            LogWarning(__PRETTY_FUNCTION__ << ": fts_close failed with error: "
                                           << strerror(error));
        }
    } else {
        if (rmdir(path->getFullPath().c_str()) != 0) {
            if (errno == EEXIST) {
                ThrowMsg(Commons::PlatformException, "Node has child nodes.");
            }
            ThrowMsg(Commons::PlatformException,
                     "Error while removing platform node.");
        }
    }
}

void Node::OnRequestReceived(const EventListNodesPtr& event)
{
    try {
        NodeList list = event->getNode()->getChildNodes(event->getFilter());
        event->setResult(list);
    } catch (const Commons::PlatformException& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    } catch (const Commons::SecurityException& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::SecurityException);
    }
}

void Node::OnRequestReceived(const EventOpenPtr& event)
{
    if (!event->checkCancelled()) {
        try {
            IStreamPtr result = open(event->getMode());
            event->setResult(result);
        } catch (const Commons::PlatformException& ex) {
            LogError("Exception: " << ex.GetMessage());
            event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
        } catch (const Commons::SecurityException& ex) {
            LogError("Exception: " << ex.GetMessage());
            event->setExceptionCode(Commons::ExceptionCodes::SecurityException);
        }
        //event can be cancelled before executing this code.
        //when it comes here we doesn't allow it anymore
        event->setCancelAllowed(false);
    } else {
        event->setCancelAllowed(true);
    }
}

void Node::OnRequestReceived(const EventReadTextPtr& event)
{
    Try {
        event->setResult(readText());
        LogDebug("LEAVIN GRACEFULLY");
    }
    Catch(Commons::PlatformException) {
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
    Catch(Commons::SecurityException) {
        event->setExceptionCode(Commons::ExceptionCodes::SecurityException);
    }
    //this function doesn't change state of the platform,
    //so we can allow to cancel it and discard results.
    event->setCancelAllowed(true);
}

std::string Node::readText()
{
    if (m_type != NT_FILE) {
        ThrowMsg(Commons::PlatformException, "Node is not a file.");
    }

    if ((m_perms & PERM_READ) == 0) {
        ThrowMsg(Commons::SecurityException, "No permission.");
    }

    std::stringstream result;
    DPL::SharedPtr<Stream> stream(new Stream(SharedFromThis(), AM_READ));
    while (!stream->isEof()) {
        result << stream->getLine();
        if (!stream->isEof()) {
            result << '\n';
        }
    }
    stream->close();
    return result.str();
}

std::string Node::toUri(int /*widgetId*/) const
{
    // TODO I believe moving this feature to WrtWrapper would make more sense.
    return "file://" + m_path->getFullPath();
}
} // Filesystem
} // WrtDeviceApis
