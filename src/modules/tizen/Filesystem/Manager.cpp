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
#include "Manager.h"

#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <pcrecpp.h>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <fts.h>
#include <ecore-1/Ecore_File.h>
#include <eina-1/eina/eina_list.h>
#include <dpl/log/log.h>
#include <dpl/scoped_ptr.h>
#include <dpl/errno_string.h>
#include <Commons/Exception.h>
#include <Commons/Regex.h>
#include <Filesystem/PathUtils.h>
#include "Node.h"
#include "Utils.h"

namespace {
const char* PATH_ROOT = "/opt/media";
const char* PATH_DOWNLOADS = "/opt/media/Downloads";
const char* PATH_DOCUMENTS = "/opt/media/Documents";
const char* PATH_SOUNDS = "/opt/media/Music";
const char* PATH_IMAGES = "/opt/media/Images";
const char* PATH_VIDEOS = "/opt/media/Videos";
const char* PATH_SDCARD = "/opt/storage/sdcard";
}

namespace WrtDeviceApis {
namespace Filesystem {
using namespace Api;

Manager::Locations Manager::m_locations;
const std::size_t Manager::m_maxPathLength = 256;
NodeList Manager::m_openedNodes;

bool Manager::fileExists(const std::string &file)
{
    errno = 0;
    struct stat info;
    memset(&info, 0, sizeof(struct stat));
    int status = lstat(file.c_str(), &info);
    if (status == 0) {
        return true;
    } else if (errno == ENOENT) {
        return false;
    }
    ThrowMsg(Commons::PlatformException, "Cannot stat file.");
}

void Manager::addOpenedNode(const INodePtr& node)
{
    NodeListIterator it = m_openedNodes.begin();
    for (; it != m_openedNodes.end(); ++it) {
        if (node.Get() == (*it).Get()) {
            //node is added already
            return;
        }
    }
    m_openedNodes.push_back(node);
}

void Manager::removeOpenedNode(const INodePtr& node)
{
    NodeListIterator it = m_openedNodes.begin();
    for (; it != m_openedNodes.end(); ++it) {
        if ((*it).Get() == node.Get()) {
            m_openedNodes.erase(it);
            break;
        }
    }
}

bool Manager::checkIfOpened(const IPathPtr& path) const
{
    Assert(path);
    NodeListIterator it = m_openedNodes.begin();
    for (; it != m_openedNodes.end(); ++it) {
        Assert(*it);
        if (*path == *(*it)->getPath()) {
            return true;
        }
    }
    return false;
}

Manager::Manager()
{
    static bool initialized = init();
    (void) initialized;
}

Manager::~Manager()
{}

IPathPtr Manager::getBasePath() const
{
    Locations::const_iterator it = m_locations.find(LT_ROOT);
    if (it == m_locations.end()) {
        ThrowMsg(Commons::PlatformException, "Base path not available.");
    }
    return it->second;
}

IPathPtr Manager::getLocationPath(LocationType type) const
{
    Locations::const_iterator it = m_locations.find(type);
    if (it != m_locations.end()) {
        return it->second->clone();
    }
    return IPathPtr();
}

LocationPaths Manager::getLocationPaths() const
{
    LocationPaths result;
    Locations::const_iterator it = m_locations.begin();
    for (; it != m_locations.end(); ++it) {
        result.push_back(it->second->clone());
    }
    return result;
}

LocationTypes Manager::getLocations() const
{
    LocationTypes result;
    Locations::const_iterator it = m_locations.begin();
    for (; it != m_locations.end(); ++it) {
        result.push_back(it->first);
    }
    return result;
}

void Manager::getNode(const EventResolvePtr& event)
{
    EventRequestReceiver<EventResolve>::PostRequest(event);
}

std::size_t Manager::getMaxPathLength() const
{
    return m_maxPathLength;
}

void Manager::copy(const EventCopyPtr& event)
{
    EventRequestReceiver<EventCopy>::PostRequest(event);
}

void Manager::move(const EventMovePtr& event)
{
    EventRequestReceiver<EventMove>::PostRequest(event);
}

void Manager::remove(const EventRemovePtr& event)
{
    EventRequestReceiver<EventRemove>::PostRequest(event);
}

void Manager::find(const EventFindPtr& event)
{
    EventRequestReceiver<EventFind>::PostRequest(event);
}

void Manager::find(const IPathPtr& path,
                   const FiltersMap& filters,
                   NodeList& result,
                   const EventFindPtr& event)
{
    Try {
        AssertMsg(path, "path is NULL");
        FTS *fts;
        FTSENT *ftsent;
        std::string pth = path->getFullPath();
        char * const paths[] = { const_cast<char * const>(pth.c_str()), NULL };

        if ((fts =
                 fts_open(paths, FTS_PHYSICAL | FTS_NOCHDIR, NULL)) == NULL)
        {
            //ERROR
            int error = errno;
            LogError(__PRETTY_FUNCTION__ << ": fts_open on "
                                         << pth
                                         << " failed with error: "
                                         << strerror(error));
            return;
        }

        while ((ftsent = fts_read(fts)) != NULL) {
            if (event && event->checkCancelled()) {
                break;
            }
            switch (ftsent->fts_info) {
            case FTS_DP:
                //directory in postorder - do nothing
                break;
            case FTS_D:
            case FTS_DC:
            case FTS_F:
            case FTS_SL:
            case FTS_SLNONE:
            case FTS_DEFAULT:
                //regular files, symbolic links, directories in preorder
                //and other file entries that can be processed further
                if (matchFilters(ftsent->fts_name, *ftsent->fts_statp,
                                 filters))
                {
                    IPathPtr childPath = IPath::create(ftsent->fts_path);
                    result.push_back(Node::resolve(childPath));
                }
                break;
            case FTS_NS:
            case FTS_NSOK:
            case FTS_DOT:
            case FTS_DNR:
            case FTS_ERR:
            default:
                LogWarning(__PRETTY_FUNCTION__
                           << ": traversal failed with error: "
                           << strerror(ftsent->fts_errno));
                ThrowMsg(Commons::PlatformException,
                         "Error reading directory");
            }
        }

        if (fts_close(fts) == -1) {
            int error = errno;
            LogWarning(__PRETTY_FUNCTION__ << ": fts_close failed with error: "
                                           << strerror(error));
            ThrowMsg(Commons::PlatformException,
                     "Could not close platform node.");
        }
    }
    Catch(Commons::Exception) {}
}

void Manager::copyElement(
    const std::string &src, const std::string &dest, bool recursive) const
{
    LogDebug("Copying src: " << src << " to: " << dest);

    //element is a file:
    if (EINA_TRUE != ecore_file_is_dir(src.c_str())) {
        if (EINA_TRUE != ecore_file_cp(src.c_str(), dest.c_str())) {
            ThrowMsg(Commons::PlatformException, "Failed to copy file");
        }
        return;
    }
    //element is a directory -> create it:
    if (EINA_TRUE != ecore_file_mkdir(dest.c_str())) {
        LogDebug("Failed to create destination directory");
        ThrowMsg(Commons::PlatformException, "Failed to copy directory");
    }
    //copy all elements of directory:
    if (recursive) {
        Eina_List* list = ecore_file_ls(src.c_str());
        void* data;
        EINA_LIST_FREE(list, data)
        {
            Try
            {
                copyElement((src + '/' + static_cast<char*>(data)).c_str(),
                            (dest + '/' + static_cast<char*>(data)).c_str());
            }
            Catch(Commons::PlatformException)
            {
                //remove rest of the list
                EINA_LIST_FREE(list, data)
                {
                    free(data);
                }
                ReThrowMsg(Commons::PlatformException, "Failed to copy element");
            }
            free(data);
        }
    }
}

bool Manager::access(const IPathPtr& path,
                     int accessType) const
{
    int amode = 0;
    if (accessType & AT_EXISTS) {
        amode |= F_OK;
    }
    if (accessType & AT_READ) {
        amode |= R_OK;
    }
    if (accessType & AT_WRITE) {
        amode |= W_OK;
    }
    if (accessType & AT_EXEC) {
        amode |= X_OK;
    }
    return (::access(path->getFullPath().c_str(), amode) == 0);
}

bool Manager::matchFilters(const std::string& name,
                           const struct stat& info,
                           const FiltersMap& filters)
{
    FiltersMap::const_iterator it = filters.begin();
    for (; it != filters.end(); ++it) {
        if (it->first == FF_NAME) {
            if (!pcrecpp::RE(it->second).PartialMatch(name)) {
                return false;
            }
        } else if (it->first == FF_SIZE) {
            std::size_t size;
            std::stringstream ss(it->second);
            ss >> size;
            if (!ss ||
                (size != static_cast<size_t>(info.st_size)))
            {
                return false;
            }
        } else if (it->first == FF_CREATED) {
            std::time_t created;
            std::stringstream ss(it->second);
            ss >> created;
            if (!ss || (created != info.st_ctime)) {
                return false;
            }
        } else if (it->first == FF_MODIFIED) {
            std::time_t modified;
            std::stringstream ss(it->second);
            ss >> modified;
            if (!ss || (modified != info.st_mtime)) {
                return false;
            }
        }
    }
    return true;
}

void Manager::OnRequestReceived(const EventResolvePtr& event)
{
    try {
        event->setResult(Node::resolve(event->getPath()));
    } catch (const Commons::PlatformException& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
    event->setCancelAllowed(true);
}

void Manager::checkPaths(
    Api::IPathPtr &src,
    Api::IPathPtr &dest)
{
    Assert(dest);
    Assert(src);
    if (!dest->isAbsolute()) {
        dest = src->getPath() + *dest;
    }

    if (src == dest) {
        ThrowMsg(Commons::PlatformException,
                 "Destination is same as source: " << src->getFullPath());
    }

    INodePtr parent;
    Try {
        parent = Node::resolve(IPath::create(dest->getPath()));
    }
    Catch(Commons::PlatformException) {
        ReThrowMsg(Commons::PlatformException,
                   "Could not get destination's parent node.");
    }

    if (parent->getType() != NT_DIRECTORY) {
        ThrowMsg(Commons::PlatformException,
                 "Destination's parent node is not directory.");
    }

    if (!access(parent->getPath(), AT_WRITE)) {
        ThrowMsg(Commons::SecurityException,
                 "Not enough permissions to write to destination.");
    }
}

bool Manager::pathExists(const std::string &path)
{
    errno = 0;
    struct stat info;
    memset(&info, 0, sizeof(struct stat));
    int status = lstat(path.c_str(), &info);
    if ((status != 0) && (errno != ENOENT)) {
        ThrowMsg(Commons::PlatformException,
                 "No access to platform destination node.");
    }
    return 0 == status;
}

void Manager::OnRequestReceived(const EventCopyPtr& event)
{
    Try {
        INodePtr srcNode = Node::resolve(event->getSource());
        int requiredAccess;
        switch (srcNode->getType()) {
        case NT_DIRECTORY:
            requiredAccess = AT_EXEC;
            break;
        case NT_FILE:
            requiredAccess = AT_READ;
            break;
        }
        if (!access(srcNode->getPath(), requiredAccess)) {
            ThrowMsg(Commons::SecurityException,
                     "Not enough permissions to copy source node.");
        }

        IPathPtr src = event->getSource();
        IPathPtr dest = event->getDestination();

        checkPaths(src, dest);

        std::string realSrc = src->getFullPath();
        std::string realDest = dest->getFullPath();

        if (pathExists(realDest)) {
            //no owerwrite flag setted -> exception
            if ((event->getOptions() & OPT_OVERWRITE) == 0) {
                ThrowMsg(Commons::PlatformException, "Overwrite is not set.");
            }

            if (event->checkCancelled()) {
                //file is not copied yet, so we can cancel it now.
                event->setCancelAllowed(true);
                return;
            }

            //destination exist. Need to be removed
            Try {
                INodePtr node = Node::resolve(event->getDestination());
                node->remove(event->getOptions());
            }
            Catch(Commons::PlatformException) {
                LogError("Exception: " << _rethrown_exception.GetMessage());
                event->setExceptionCode(
                    Commons::ExceptionCodes::PlatformException);
            }
            Catch(Commons::SecurityException) {
                event->setExceptionCode(
                    Commons::ExceptionCodes::SecurityException);
            }
        }
        //Destination is not exist. Start copy now.
        copyElement(realSrc, realDest);

        event->setResult(Node::resolve(dest));
    } catch (const Commons::PlatformException& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    } catch (const Commons::SecurityException& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::SecurityException);
    }
    //file is copied already so we don't allow cancelling anymore.
    event->setCancelAllowed(false);
}

void Manager::OnRequestReceived(const EventMovePtr& event)
{
    try {
        IPathPtr src = event->getSource();
        IPathPtr dest = event->getDestination();

        INodePtr srcNode = Node::resolve(src);
        if (!access(srcNode->getParent()->getPath(), AT_WRITE)) {
            ThrowMsg(Commons::SecurityException,
                     "Not enough permissions to move source node.");
        }

        checkPaths(src, dest);

        bool destExists = pathExists(dest->getFullPath());

        if (destExists && (0 == (event->getOptions() & OPT_OVERWRITE))) {
            ThrowMsg(Commons::PlatformException, "Overwrite is not set.");
        }

        if (event->checkCancelled()) {
            //file is not moved yet, so we can cancel it now.
            event->setCancelAllowed(true);
            return;
        }

        errno = 0;
        if (0 != ::rename(src->getFullPath().c_str(),
                          dest->getFullPath().c_str()))
        {
            int error = errno;
            switch (error) {
            case EXDEV:
            {
                if (destExists) {
                    //destination exist. Need to be removed
                    Try {
                        INodePtr node = Node::resolve(
                                event->getDestination());
                        node->remove(event->getOptions());
                    }
                    Catch(Commons::PlatformException) {
                        LogError("Exception while removing dest directory");
                        event->setExceptionCode(
                            Commons::ExceptionCodes::PlatformException);
                    }
                    Catch(Commons::SecurityException) {
                        event->setExceptionCode(
                            Commons::ExceptionCodes::SecurityException);
                    }
                }

                copyElement(src->getFullPath(),
                            dest->getFullPath());
                //remove source files
                Try {
                    INodePtr node = Node::resolve(event->getSource());
                    node->remove(event->getOptions());
                }
                Catch(Commons::Exception) {
                    LogError("Exception: "
                             << _rethrown_exception.GetMessage());
                }
                break;
            }
            default:
                ThrowMsg(Commons::PlatformException,
                         "Error on rename: " << DPL::GetErrnoString(error));
                break;
            }
        }

        event->setResult(Node::resolve(dest));
    } catch (const Commons::PlatformException& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    } catch (const Commons::SecurityException& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::SecurityException);
    }
    event->setCancelAllowed(false);
}

void Manager::OnRequestReceived(const EventRemovePtr& event)
{
    if (!event->checkCancelled()) {
        Try {
            INodePtr node = Node::resolve(event->getPath());
            node->remove(event->getOptions());
        }
        Catch(Commons::PlatformException) {
            LogError("Exception: " << _rethrown_exception.GetMessage());
            event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
        }
        Catch(Commons::SecurityException) {
            event->setExceptionCode(Commons::ExceptionCodes::SecurityException);
        }
        event->setCancelAllowed(false);
    } else {
        event->setCancelAllowed(true);
    }
}

void Manager::OnRequestReceived(const EventFindPtr& event)
{
    try {
        NodeList result;
        find(event->getPath(), event->getFilters(), result, event);
        event->setResult(result);
    } catch (const Commons::Exception& ex) {
        LogError("Exception: " << ex.GetMessage());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
    event->setCancelAllowed(true);
}

bool Manager::init()
{
    m_locations[LT_ROOT] = IPath::create(PATH_ROOT);
    m_locations[LT_SDCARD] = IPath::create(PATH_SDCARD);
    setupLocation(LT_DOWNLOADS, PATH_DOWNLOADS);
    setupLocation(LT_DOCUMENTS, PATH_DOCUMENTS);
    setupLocation(LT_SOUNDS, PATH_SOUNDS);
    setupLocation(LT_IMAGES, PATH_IMAGES);
    setupLocation(LT_VIDEOS, PATH_VIDEOS);

    return true;
}

void Manager::setupLocation(LocationType location,
                            const char* path)
{
    if (!nodeExists(path)) {
        try {
            makePath(path, 0755);
        } catch (const Commons::PlatformException& ex) {
            LogError("Exception: " << ex.DumpToString());
            return;
        }
    }
    m_locations[location] = IPath::create(path);
}
} // Filesystem
} // WrtDeviceApis
