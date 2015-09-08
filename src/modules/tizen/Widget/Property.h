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
 *
 *
 * @file       Property.h
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */

#ifndef WIDGET_PROPERTY_H_
#define WIDGET_PROPERTY_H_

#include <string>
#include <Commons/Exception.h>

namespace WrtDeviceApis {
namespace Widget {
template<typename T>
class Property
{
  public:
    explicit Property() :
        m_value(),
        m_valid(false)
    {}

    void setValue(const T& value)
    {
        m_value = value;
        m_valid = true;
    }

    T getValue() const
    {
        if (!m_valid) {
            Throw(Commons::PlatformException);
        }
        return m_value;
    }

    bool isValid() const
    {
        return m_valid;
    }

    virtual ~Property()
    {}

  private:
    T m_value;
    bool m_valid;
};

typedef Property<std::string> StringProperty;
typedef Property<unsigned int> UIntProperty;
}
}
#endif
