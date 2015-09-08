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

#include "TimeUtils.h"
#include <unicode/ucal.h>

namespace WrtDeviceApis {
namespace Commons {
namespace Time {
struct tm* localtime_r(long long int* timep, struct tm* result)
{
    UErrorCode status = U_ZERO_ERROR;
    UCalendar *cal;

    if (!result || !timep) {
        return NULL;
    }

    cal = ucal_open(NULL, 0, NULL, UCAL_DEFAULT, &status);

    if (U_FAILURE(status)) {
        return NULL;
    }

    ucal_setMillis(cal, (*timep * (double)1000.0), &status);

    result->tm_sec = ucal_get(cal, UCAL_SECOND, &status);
    result->tm_min = ucal_get(cal, UCAL_MINUTE, &status);
    result->tm_hour = ucal_get(cal, UCAL_HOUR, &status);
    result->tm_mday = ucal_get(cal, UCAL_DAY_OF_MONTH, &status);
    result->tm_mon = ucal_get(cal, UCAL_MONTH, &status);
    result->tm_year = ucal_get(cal, UCAL_YEAR, &status);
    result->tm_wday = ucal_get(cal, UCAL_DAY_OF_WEEK, &status);
    result->tm_yday = ucal_get(cal, UCAL_DAY_OF_YEAR, &status);
    result->tm_isdst = ucal_get(cal, UCAL_DST_OFFSET, &status);

    ucal_close(cal);

    if (U_FAILURE(status)) {
        return NULL;
    } else {
        return result;
    }
}
}
}
}
