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
#ifndef WRTDEVICEAPIS_FILESYSTEM_STREAM_H_
#define WRTDEVICEAPIS_FILESYSTEM_STREAM_H_

#include <cstddef>
#include <fstream>
#include <dpl/shared_ptr.h>
#include <dpl/enable_shared_from_this.h>
#include <Filesystem/IStream.h>
#include <Filesystem/Enums.h>
#include <Filesystem/INode.h>

namespace WrtDeviceApis {
namespace Filesystem {
class Node;
typedef DPL::SharedPtr<Node> NodePtr;

class Stream :
    public Api::IStream,
    public DPL::EnableSharedFromThis<Stream>
{
    friend class Node;

  public:
    ~Stream();

    Api::IStreamPtr write(bool arg);
    Api::IStreamPtr write(unsigned char arg);
    Api::IStreamPtr write(char arg);
    Api::IStreamPtr write(int arg);
    Api::IStreamPtr write(double arg);
    Api::IStreamPtr write(const std::string& arg);

    Api::IStreamPtr read(bool& arg);
    Api::IStreamPtr read(unsigned char& arg);
    Api::IStreamPtr read(char& arg);
    Api::IStreamPtr read(int& arg);
    Api::IStreamPtr read(double& arg);
    Api::IStreamPtr read(std::string& arg);

    /**
     * @throw PlatformException If unable to read from the stream.
     */
    unsigned char* getBytes(std::size_t num);

    std::size_t getCount() const;

    /**
     * @throw PlatformException If unable to read from the stream.
     */
    char* getChars(std::size_t num);

    std::string getLine();

    bool isOpen() const;
    bool isEof() const;

    void close();

    long getPosition() const;
    void setPosition(long position);

    int getMode() const;

    long getSize() const;

  private:
    template<typename T>
    Api::IStreamPtr read_(T& arg);
    template<typename T>
    Api::IStreamPtr write_(T arg);

    inline bool isReadable() const;
    inline bool isWriteable() const;
    void checkForReading() const;
    void checkForWriting() const;

  private:
    Stream(const NodePtr& parent,
           int mode);

  private:
    NodePtr m_parent;
    int m_mode;
    mutable std::fstream m_stream;
};

typedef DPL::SharedPtr<Stream> StreamPtr;
} // Filesystem
} // WrtDeviceApis

#endif /* WRTDEVICEAPIS_FILESYSTEM_STREAM_H_ */
