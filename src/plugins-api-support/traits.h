/*
 * Copyright (c) 2012 Samsung Electronics Co., Ltd All Rights Reserved
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
 * @file   traits.h
 * @author Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 */

#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_TRAITS_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_TRAITS_H_

#include "detail/traits.h"

namespace WrtPluginsApi {
namespace Traits {
/**
 * Gets index of specified type in the type list.
 */
template<typename RequiredType, typename ... TupleArgTypes>
struct index_of
{
    static const size_t value = Detail::index_of_<0,
                                                  RequiredType,
                                                  TupleArgTypes ...>::value;
};
}
}

#endif
