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
/*
 * @author      Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version     0.1
 * @brief
 */

#ifndef WRTDEVICEAPIS_COMMONSJAVASCRIPT_SUPPORT_PENDING_OPERATION_H_
#define WRTDEVICEAPIS_COMMONSJAVASCRIPT_SUPPORT_PENDING_OPERATION_H_

#include <boost/optional.hpp>

namespace WrtDeviceApis {
namespace CommonsJavaScript {
template<typename T>
class SupportPendingOperation
{
  public:
    virtual T getPendingOperation() const
    {
        return *m_operation;
    }

    virtual void setPendingOperation(const T& operation)
    {
        m_operation = operation;
    }

    bool checkPendingOperation() const
    {
        return !m_operation.IsNull();
    }

    virtual ~SupportPendingOperation(){}

  protected:
    boost::optional<T> m_operation;
};
}
}

#endif
