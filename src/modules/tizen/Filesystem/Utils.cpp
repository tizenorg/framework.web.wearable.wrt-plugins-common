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
 * @file    Utils.cpp
 * @author  Zbigniew Kostrzewa (z.kostrzewa@samsung.com)
 * @version 1.0
 */

#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <Commons/Exception.h>
#include "Utils.h"

namespace {
int try_mkdir(const char* path,
              mode_t mode)
{
    struct stat st;
    int err = 0;

    if (::stat(path, &st) != 0) {
        if (::mkdir(path, mode) != 0) {
            err = -1;
        }
    } else if (!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;
        err = -1;
    }

    return err;
}

int mkpath(const char* path,
           mode_t mode)
{
    char* copy = ::strdup(path);
    if (NULL == copy) {
        return -1;
    }

    int err = 0;
    char* ptr = copy;
    char* slash = NULL;

    while ((0 == err) && (NULL != (slash = ::strchr(ptr, '/')))) {
        if (slash != ptr) {
            *slash = '\0';
            err = try_mkdir(copy, mode);
            *slash = '/';
        }
        ptr = slash + 1;
    }

    if (0 == err) {
        err = try_mkdir(path, mode);
    }

    ::free(copy);
    return err;
}
}

namespace WrtDeviceApis {
namespace Filesystem {
bool nodeExists(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) == 0) {
        if (S_ISDIR(info.st_mode)) {
            return true;
        }
    }
    return false;
}

void makePath(const std::string& path,
              mode_t mode)
{
    if (mkpath(path.c_str(), mode) == -1) {
        ThrowMsg(Commons::PlatformException, "Couldn't create path: " << path);
    }
}
}
}
