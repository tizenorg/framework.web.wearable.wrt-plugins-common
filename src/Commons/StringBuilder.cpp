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
/**
 * @author          Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */
#include <sstream>
#include "Exception.h"
#include "StringBuilder.h"

namespace WrtDeviceApis {
namespace Commons {
class StringBuilderImpl
{
  public:
    StringBuilderImpl() : m_counter(1)
    {}

    void append(const std::string& str)
    {
        if (!(m_ss << str)) {
            m_ss.clear();
            ThrowMsg(PlatformException, "Couldn't append string.");
        }
    }

    void append(const StringBuilderImpl* impl)
    {
        if (!(m_ss << impl->m_ss)) {
            m_ss.clear();
            ThrowMsg(PlatformException,
                     "Couldn't append string builder.");
        }
    }

    void addRef()
    {
        ++m_counter;
    }

    void deref()
    {
        if (0 == --m_counter) {
            delete this;
        }
    }

    StringBuilderImpl* clone()
    {
        if (1 == m_counter) {
            return this;
        }

        --m_counter;

        StringBuilderImpl* result = new StringBuilderImpl();
        result->m_ss << m_ss;
        return result;
    }

    std::string toString() const
    {
        return m_ss.str();
    }

  private:
    std::stringstream m_ss;
    unsigned int m_counter;
};

StringBuilder::StringBuilder() : m_impl(new StringBuilderImpl())
{}

StringBuilder::~StringBuilder()
{
    m_impl->deref();
}

StringBuilder::StringBuilder(const StringBuilder& other)
{
    other.m_impl->addRef();
    m_impl = other.m_impl;
}

StringBuilder & StringBuilder::operator=(const StringBuilder& other)
{
    if (this == &other) {
        return *this;
    }

    other.m_impl->addRef();

    m_impl->deref();
    m_impl = other.m_impl;

    return *this;
}

StringBuilder& StringBuilder::append(const std::string& str)
{
    m_impl = m_impl->clone();
    m_impl->append(str);
    return *this;
}

StringBuilder& StringBuilder::append(const StringBuilder& builder)
{
    m_impl = m_impl->clone();
    m_impl->append(builder.m_impl);
    return *this;
}

std::string StringBuilder::toString() const
{
    return m_impl->toString();
}
}
} // WrtDeviceApisCommon
