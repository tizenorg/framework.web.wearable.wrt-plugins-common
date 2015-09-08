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
 * @author      Lukasz Marek (l.marek@samsung.com)
 * @version     0.1
 * @brief
 */

#ifndef WRTDEVICEAPIS_COMMONS_WIDGET_INTERFACE_H_
#define WRTDEVICEAPIS_COMMONS_WIDGET_INTERFACE_H_

#include <string>
#include <dpl/optional_typedefs.h>
#include <wrt-commons/widget-interface-dao/widget_interface_dao.h>
#include <LocalStorage/ILocalStorage.h>

namespace WrtDeviceApis {
class WidgetInterfaceObject :
    public LocalStorage::Api::ILocalStorage
{
  public:
    WidgetInterfaceObject(int widgetHandle);
    virtual ~WidgetInterfaceObject();

    void setItem(const std::string& key,
                 const std::string& value,
                 bool readOnly);
    void removeItem(const std::string& key);
    DPL::OptionalStdString getValue(const std::string& key) const;
    void clear(bool removeReadOnly);
    size_t getStorageSize() const;
    std::string getKeyByIndex(size_t index) const;

  private:
    WidgetInterfaceDB::WidgetInterfaceDAOPtr m_dao;
};
}

#endif //WRTDEVICEAPIS_COMMONS_WIDGET_INTERFACE_H_
