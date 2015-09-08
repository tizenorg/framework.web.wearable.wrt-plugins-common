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
#include "Stream.h"
#include <stdio.h>
#include <new>
#include <dpl/assert.h>
#include <dpl/scoped_array.h>
#include <Commons/Exception.h>
#include "Manager.h"
#include "Node.h"

namespace WrtDeviceApis {
namespace Filesystem {
using namespace Api;

Stream::Stream(const NodePtr& parent,
               int mode) :
    m_parent(parent),
    m_mode(mode)
{
    AssertMsg(m_parent, "Stream needs to have parent.");

    std::ios_base::openmode mode_ = std::ios_base::binary;
    if (mode & AM_READ) {
        mode_ |= std::ios_base::in;
    }
    if (mode & AM_WRITE) {
        mode_ |= std::ios_base::out;
    } else if (mode &
               AM_APPEND)
    {
        mode_ |= (std::ios_base::app | std::ios_base::out);
    }

    m_stream.open(m_parent->getPath()->getFullPath().c_str(), mode_);
    if (!m_stream) {
        ThrowMsg(Commons::PlatformException, "Could not open stream.");
    }
}

Stream::~Stream()
{
    close();
}

IStreamPtr Stream::write(bool arg)
{
    return write_(arg);
}

IStreamPtr Stream::write(unsigned char arg)
{
    return write_(arg);
}

IStreamPtr Stream::write(char arg)
{
    return write_(arg);
}

IStreamPtr Stream::write(int arg)
{
    return write_(arg);
}

IStreamPtr Stream::write(double arg)
{
    return write_(arg);
}

IStreamPtr Stream::write(const std::string& arg)
{
    return write_(arg);
}

IStreamPtr Stream::read(bool& arg)
{
    return read_(arg);
}

IStreamPtr Stream::read(unsigned char& arg)
{
    return read_(arg);
}

IStreamPtr Stream::read(char& arg)
{
    return read_(arg);
}

IStreamPtr Stream::read(int& arg)
{
    return read_(arg);
}

IStreamPtr Stream::read(double& arg)
{
    return read_(arg);
}

IStreamPtr Stream::read(std::string& arg)
{
    return read_(arg);
}

char* Stream::getChars(std::size_t num)
{
    checkForReading();

    std::size_t maxChars = num + 1;
    DPL::ScopedArray<char> result;
    Try {
        result.Reset(new char[maxChars]);
    }
    Catch(std::bad_alloc) {
        ThrowMsg(Commons::PlatformException, "Couldn't allocate output buffer.");
    }
    if (m_stream.get(result.Get(), maxChars).bad()) {
        ThrowMsg(Commons::PlatformException,
                 "Error while reading from the stream.");
    }

    return result.Release();
}

unsigned char* Stream::getBytes(std::size_t num)
{
    checkForReading();

    DPL::ScopedArray<char> buffer;
    Try {
        buffer.Reset(new char[num]);
    }
    Catch(std::bad_alloc) {
        ThrowMsg(Commons::PlatformException, "Couldn't allocate output buffer.");
    }
    if (m_stream.read(buffer.Get(), num).bad()) {
        ThrowMsg(Commons::PlatformException,
                 "Error while reading from the stream.");
    }

    return static_cast<unsigned char*>(static_cast<void*>(buffer.Release()));
}

std::size_t Stream::getCount() const
{
    if (!isOpen()) {
        ThrowMsg(Commons::PlatformException, "Stream is closed.");
    }

    if (!isReadable()) {
        ThrowMsg(Commons::PlatformException, "Stream is not readable.");
    }

    return m_stream.gcount();
}

std::string Stream::getLine()
{
    checkForReading();

    std::string result;
    std::getline(m_stream, result);

    return result;
}

bool Stream::isOpen() const
{
    return m_stream.is_open();
}

bool Stream::isEof() const
{
    return m_stream.eof();
}

void Stream::close()
{
    if (isOpen()) {
        m_stream.close();
        m_parent->onStreamClose(SharedFromThis());
    }
}

int Stream::getMode() const
{
    return m_mode;
}

long Stream::getPosition() const
{
    return static_cast<long>(m_stream.tellg());
}

void Stream::setPosition(long position)
{
    if (m_stream.rdstate() & std::ios_base::eofbit) {
        m_stream.clear();
    }
    if (!(m_stream.seekg(position)) || !(m_stream.seekp(position))) {
        ThrowMsg(Commons::PlatformException, "Could not set position.");
    }
}

long Stream::getSize() const
{
    std::fstream::streampos pos = m_stream.tellg();
    if (pos == -1) {
        return -1;
    }

    if (!m_stream.seekg(0, std::_S_end)) {
        return -1;
    }

    long result = m_stream.tellg();
    m_stream.seekg(pos, std::_S_beg);

    return (result == -1 ? result : result + 1);
}

template<typename T>
IStreamPtr Stream::write_(T arg)
{
    checkForWriting();

    if (!(m_stream << arg)) {
        LogError("Error while writing to the stream.");
        ThrowMsg(Commons::PlatformException,
                 "Error while writing to the stream.");
    }
    m_stream.flush();

    return DPL::StaticPointerCast<IStream>(SharedFromThis());
}

template<typename T>
IStreamPtr Stream::read_(T& arg)
{
    checkForReading();

    if (!(m_stream >> arg)) {
        ThrowMsg(Commons::PlatformException,
                 "Error while reading from the stream.");
    }

    return DPL::StaticPointerCast<IStream>(SharedFromThis());
}

bool Stream::isReadable() const
{
    return (m_mode & AM_READ);
}

bool Stream::isWriteable() const
{
    return ((m_mode & AM_WRITE) || (m_mode & AM_APPEND));
}

void Stream::checkForReading() const
{
    if (!isOpen()) {
        ThrowMsg(Commons::PlatformException, "Stream is closed.");
    }

    if (isEof()) {
        ThrowMsg(Commons::PlatformException, "Stream is marked as EOF.");
    }

    if (!isReadable()) {
        ThrowMsg(Commons::PlatformException, "Stream is not readable.");
    }
}

void Stream::checkForWriting() const
{
    if (!isOpen()) {
        LogError("Stream is closed.");
        ThrowMsg(Commons::PlatformException, "Stream is closed.");
    }

    if (!isWriteable()) {
        LogError("Stream is not writeable.");
        ThrowMsg(Commons::PlatformException, "Stream is not writeable.");
    }
}
} // Filesystem
} // WrtDeviceApis
