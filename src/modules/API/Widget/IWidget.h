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
 *
 *
 * @file       IWidget.h
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */
#ifndef INTERFACE_WIDGET_CONFIG_INFO_H
#define INTERFACE_WIDGET_CONFIG_INFO_H

#include <string>

#include <dpl/shared_ptr.h>

namespace WrtDeviceApis {
namespace Widget {
namespace Api {
class IWidget;
typedef DPL::SharedPtr<IWidget> IWidgetPtr;

class IWidget
{
  public: // methods

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getAuthor() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getAuthorEmail() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getAuthorHref() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getDescription() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getId() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getName() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getShortName() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual std::string getVersion() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual unsigned int getHeight() const = 0;

    /**
     * getter for property from widget config xml
     * @throw Commons::PlatformException if problem with platform occured
     * @throw Commons::NullPointerException if problem with platform occured
     * @throw Commons::SecurityException if wrt forbiden using this function
     * */
    virtual unsigned int getWidth() const = 0;

    virtual ~IWidget()
    {}
};
}
}
}

#endif
