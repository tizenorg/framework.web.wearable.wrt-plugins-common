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
 * @author      Andrzej Surdej (a.surdej@samsung.com)
 * @version     0.1
 * @brief       Local storage interface file
 */

#ifndef WRTDEVICEAPIS_LOCALSTORAGE_LOCAL_STORAGE_INTERFACE_H_
#define WRTDEVICEAPIS_LOCALSTORAGE_LOCAL_STORAGE_INTERFACE_H_

#include <string>
#include <dpl/shared_ptr.h>
#include <dpl/optional_typedefs.h>

namespace WrtDeviceApis {
namespace LocalStorage {
namespace Api {
class ILocalStorage
{
  public:

    /**
     * Save pair key/value in local storage
     * @param key
     * @param value
     * @param readOnly - if true, the value will be read only
     *                  otherwise the value will be modificable
     */
    virtual void setItem(const std::string& key,
                         const std::string& value,
                         bool readOnly) = 0;

    /**
     * Remove pair key/value from local storage
     */
    virtual void removeItem(const std::string& key) = 0;

    /**
     * Get value for key
     *
     */
    virtual DPL::OptionalStdString getValue(
        const std::string& key) const = 0;

    /**
     * Clears the local storage
     * @param removeReadOnly - true -remove all elements
     *                          false - ommit items which have read only flag
     * set
     */
    virtual void clear(bool removeReadOnly) = 0;

    /**
     * Get number of elements in local storage
     * @param removeReadOnly - true -remove all elements
     *                          false - ommit items which have read only flag
     * set
     */
    virtual size_t getStorageSize() const = 0;

    /**
     * Get key name for index
     */
    virtual std::string getKeyByIndex(size_t index) const = 0;

    /**
     * virtual destructor
     */
    virtual ~ILocalStorage()
    {}
};

typedef DPL::SharedPtr<ILocalStorage> ILocalStoragePtr;
} // Api
} // LocalStorage
} // WrtDeviceApis

#endif // WRTDEVICEAPIS_LOCALSTORAGE_LOACA_STORAGE_INTERFACE_H_
