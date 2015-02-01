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
 * @author       Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef WRTDEVICEAPIS_COMMONS_DIMENSION_H_
#define WRTDEVICEAPIS_COMMONS_DIMENSION_H_

namespace WrtDeviceApis {
namespace Commons {
/**
 * Encapsulates width and height of a component.
 */
class Dimension
{
  public:
    /**
     * Type of dimension's values.
     */
    typedef unsigned int ValueType;

  public:
    /**
     * Creates an instance of Dimension with specified width and height.
     * @param width Specified width.
     * @param height Specified height.
     */
    Dimension(ValueType width,
              ValueType height) :
        m_width(width),
        m_height(height)
    {}

    ValueType getWidth() const
    {
        return m_width;
    }

    ValueType getHeight() const
    {
        return m_height;
    }

    void setSize(ValueType width,
                 ValueType height)
    {
        m_width = width;
        m_height = height;
    }

  private:
    ValueType m_width;
    ValueType m_height;
};
}
} // WrtDeviceApisCommon

#endif // WRTDEVICEAPIS_COMMONS_DIMENSION_H_
