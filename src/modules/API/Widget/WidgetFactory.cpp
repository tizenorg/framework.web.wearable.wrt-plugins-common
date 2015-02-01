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
 * @file       WidgetFactory.cpp
 * @author     Grzegorz Krawczyk (g.krawczyk@samsung.com)
 * @version    0.1
 * @brief
 */
#include "WidgetFactory.h"

#include <Widget/Widget.h>

namespace WrtDeviceApis {
namespace Widget {
namespace Api {
IWidgetPtr WidgetFactory::createWidget()
{
    return IWidgetPtr(new WrtDeviceApis::Widget::Widget());
}
}
}
}
