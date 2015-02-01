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
 * @file   TimeUtils.h
 * @author Tae-Jeong Lee (taejeong.lee@samsung.com)
 */

#ifndef WRTDEVICEAPIS_COMMONS_TIMEUTILS_H_
#define WRTDEVICEAPIS_COMMONS_TIMEUTILS_H_

#include <time.h>

namespace WrtDeviceApis {
namespace Commons {
namespace Time {
struct tm* localtime_r(long long int* timep, struct tm* result);
}
}
}
#endif