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
#ifndef WRTDEVICEAPIS_FILESYSTEM_ISTREAM_H_
#define WRTDEVICEAPIS_FILESYSTEM_ISTREAM_H_

#include <cstddef>
#include <string>
#include <dpl/noncopyable.h>
#include <dpl/shared_ptr.h>

namespace WrtDeviceApis {
namespace Filesystem {
namespace Api {
class IStream;
typedef DPL::SharedPtr<IStream> IStreamPtr;

class IStream : private DPL::Noncopyable
{
  public:
    virtual ~IStream() = 0;

    virtual IStreamPtr write(bool arg) = 0;
    virtual IStreamPtr write(unsigned char arg) = 0;
    virtual IStreamPtr write(char arg) = 0;
    virtual IStreamPtr write(int arg) = 0;
    virtual IStreamPtr write(double arg) = 0;
    virtual IStreamPtr write(const std::string& arg) = 0;

    virtual IStreamPtr read(bool& arg) = 0;
    virtual IStreamPtr read(unsigned char& arg) = 0;
    virtual IStreamPtr read(char& arg) = 0;
    virtual IStreamPtr read(int& arg) = 0;
    virtual IStreamPtr read(double& arg) = 0;
    virtual IStreamPtr read(std::string& arg) = 0;

    /**
     * Gets characters from stream.
     * @param num Number of characters to read.
     * @return Pointer to read buffer.
     * @throw PlatformException if stream is closed, EOF is set or write-only.
     * @remarks Passes ownership to the caller.
     */
    virtual char* getChars(std::size_t num) = 0;

    /**
     * Gets bytes from stream.
     * @param num Number of bytes to read.
     * @return Pointer to read buffer.
     * @remarks Passes ownership to the caller.
     * @throw PlatformException if stream is closed, EOF is set or write-only.
     */
    virtual unsigned char* getBytes(std::size_t num) = 0;

    /**
     * Gets number of bytes read by last getBytes() or getChars() operation.
     * @return Number of read bytes.
     * @throw PlatformException if stream is closed or write-only.
     */
    virtual std::size_t getCount() const = 0;

    /**
     * Reads line of characters from stream (till '\n' character).
     * @return Read line.
     */
    virtual std::string getLine() = 0;

    /**
     * Checks whether stream is open.
     * @return True if stream is open, false otherwsie.
     */
    virtual bool isOpen() const = 0;

    /**
     * Checks whether End-Of-Line character occured.
     * @return True if EOF flag was set, false otherwise.
     */
    virtual bool isEof() const = 0;

    /**
     * Closes stream.
     */
    virtual void close() = 0;

    /**
     * Gets current position in stream.
     * @return Position or -1 if fails.
     */
    virtual long getPosition() const = 0;

    /**
     * Sets current position in stream.
     * @param position Position to set.
     */
    virtual void setPosition(long position) = 0;

    /**
     * Gets mode stream was opened in.
     * @return Mode @see Api::Filesystem::AccessMode.
     */
    virtual int getMode() const = 0;

    /**
     * Gets stream's size.
     * @return Size or -1 if unavailable (e.g. stream is write-only).
     */
    virtual long getSize() const = 0;
};
} // API
} // Filesystem
} // WrtDeviceApis

#endif /* WRTDEVICEAPIS_FILESYSTEM_ISTREAM_H_ */
