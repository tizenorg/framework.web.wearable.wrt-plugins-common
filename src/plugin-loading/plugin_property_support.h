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
 * @file    plugin_property_support.h
 * @author  Yunchan Cho (yunchan.cho@samsung.com)
 * @version 1.0
 * @brief
 */

#ifndef _PLUGIN_LOGIC_PROPERTY_SUPPORT_H_
#define _PLUGIN_LOGIC_PROPERTY_SUPPORT_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>

namespace PluginModule {
class WindowPropertySupport
{
  public:
    float getScale() const;
    std::string getBundle() const;
    std::string getTheme() const;
    JSGlobalContextRef getJSContext() const;

    void setScaleToNavigatorProperty(const double scale);
    void setBundleToWindowProperty(const char* bundle);
    void setThemeToNavigatorProperty(const char* theme);

    explicit WindowPropertySupport(JSGlobalContextRef m_context);
    ~WindowPropertySupport();

  private:
    float m_scale;
    std::string m_bundle;   // string of base64 encoded bundle
    std::string m_theme;
    JSGlobalContextRef m_context;

    void setPropertyToWindow(const char* propertyName, JSValueRef jsValue);
    void setPropertyToNavigator(const char* propertyName, JSValueRef jsValue);
};
} // namespace PluginModule

#endif // _PLUGIN_LOGIC_PROPERTY_SUPPORT_H_
