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

#ifndef _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_DETAIL_TRAITS_H_
#define _WRT_PLUGINS_COMMON_PLUGIN_API_SUPPORT_DETAIL_TRAITS_H_

namespace WrtPluginsApi {
namespace Traits {
namespace Detail {
template<size_t, typename RequiredType, typename ... TupleArgTypes>
struct index_of_;

/*
 * CurrentArgType is not equal to RequiredType, check next tuple's argument
 */
template<size_t n,
         typename RequiredType,
         typename CurrentArgType,
         typename ... TupleArgTypes>
struct index_of_<n, RequiredType, CurrentArgType, TupleArgTypes ...>
{
    static const size_t value = index_of_<n + 1,
                                          RequiredType,
                                          TupleArgTypes ...>::value;
};

/*
 * RequiredType found on tuple's args list
 * return position on tuple's list
 */
template<size_t n, typename RequiredType, typename ... TupleArgTypes>
struct index_of_<n, RequiredType, RequiredType, TupleArgTypes ...>
{
    static const size_t value = n;
};

/*
 * RequiredType found on last position of tuple's args list
 * return position on tuple's list
 */
template<size_t n, typename RequiredType>
struct index_of_<n, RequiredType, RequiredType>
{
    static const size_t value = n;
};

/*
 * RequiredType was not found on tuple args list
 */
template<size_t n, typename RequiredType, typename LastArgType>
struct index_of_<n, RequiredType, LastArgType>
{
    static const size_t value = -1;
};
}
}
}

#endif
