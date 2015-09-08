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
#ifndef WRTDEVICEAPIS_COMMONS_EXCEPTION_H_
#define WRTDEVICEAPIS_COMMONS_EXCEPTION_H_

#include <dpl/exception.h>

namespace WrtDeviceApis {
namespace Commons {
#define PLUGINS_DECLARE_EXCEPTION_TYPE(BaseClass, Class) \
    class Class :       \
        public BaseClass \
    { \
        ExceptionCodes::Enumeration m_code; \
      public: \
        virtual ExceptionCodes::Enumeration getCode() const { return m_code; } \
        Class(const char *path, \
              const char *function, \
              int line, \
              const std::string & message = std::string()) :                                                                                               \
            BaseClass(path, function, line, message) \
        { \
            BaseClass::m_className = #Class; \
            m_code = ExceptionCodes::Class; \
        } \
 \
        Class(const char *path, \
              const char *function, \
              int line, \
              const DPL::Exception & reason, \
              const std::string & message = std::string()) :                                                                                                                             \
            BaseClass(path, function, line, reason, message) \
        { \
            BaseClass::m_className = #Class; \
            m_code = ExceptionCodes::Class; \
        } \
    };

class ExceptionCodes
{
  public:
    enum Enumeration
    {
        None,
        Exception,
        InvalidArgumentException,
        ConversionException,
        NullPointerException,
        UnknownException,
        PlatformException,
        OutOfRangeException,
        EventCancelledException,
        EventWrongStateException,
        SecurityException,
        UnsupportedException,
        PlatformWrongStateException,
        PendingOperationException,
        AlreadyInUseException,
        CameraCaptureException,
        CameraLiveVideoException,
        LocalStorageValueNoModifableException,
        NotFoundException
    };
};

/**
 * General exception.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(DPL::Exception, Exception)

/**
 * Thrown when passed argument is not of expected type.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, InvalidArgumentException)

/**
 * Thrown when type conversion is not possible.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, ConversionException)

/**
 * Thrown when trying to operate on an object which is set to NULL.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, NullPointerException)

/**
 * Thrown when unknown error occured.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, UnknownException)

/**
 * Thrown when platform function returned error code or threw an exception.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, PlatformException)

/**
 * Thrown when trying to access out of range element from array
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, OutOfRangeException)

/**
 * Thrown when trying to operate on cancelled event
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, EventCancelledException)

/**
 * Thrown when trying to operate on event in wrong state
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, EventWrongStateException)

/**
 * Thrown when trying to perform some action w/o proper permissions.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, SecurityException)

/**
 * Thrown when trying to perform action that is not supported by current
 * platform.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, UnsupportedException)

/**
 * Thrown when trying to perform action on platform in wrong state.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, PlatformWrongStateException)

/**
 * Thrown when trying to perform asynchronous action on JS object
 * that already executes other asynchronous operation.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, PendingOperationException)

/**
 * Thrown when used camera is already in use.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, AlreadyInUseException)

/**
 * Thrown when unpredicted error occurs while a picture or video is being
 * captured
 * or if endRecording is called while no video is currently captured.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, CameraCaptureException)

/**
 * camera live video cannot be provided.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, CameraLiveVideoException)

/**
 * Error trying to modify read only value
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, LocalStorageValueNoModifableException)

/**
 * Thrown when object is not found.
 */
PLUGINS_DECLARE_EXCEPTION_TYPE(Exception, NotFoundException)
}
} // WrtDeviceApisCommon

#endif /* WRTDEVICEAPIS_COMMONS_EXCEPTION_H_ */
