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
 * @file        popup-runner.h
 * @author      Janusz Kozerski (j.kozerski@samsung.com)
 * @version     1.0
 */

#include <ace_api_client.h>
#include <vector>

#include <dpl/serialization.h>

namespace Wrt {
namespace Popup {
class BinaryStream : public DPL::IStream
{
  public:
    void Read (size_t num, void * bytes);
    void Write(size_t num, const void * bytes);

    BinaryStream();
    ~BinaryStream();

    const unsigned char* char_pointer() const;
    size_t size() const;

  private:
    std::vector<unsigned char> m_data;
    size_t m_readPosition;
};

ace_return_t run_popup(
    ace_popup_t popup_type,
    const ace_resource_t resource_name,
    const ace_session_id_t session_id,
    const ace_param_list_t* ace_param_list,
    ace_widget_handle_t handle,
    ace_bool_t* validation_result
    );
} // Popup
} // Wrt
