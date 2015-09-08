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
 * @file    plugin_property_support.cpp
 * @author  Yunchan Cho (yunchan.cho@samsung.com)
 * @version 1.0
 * @brief
 */

#include "plugin_property_support.h"
#include <dpl/log/secure_log.h>

using namespace PluginModule;

const char *NAVIGATOR_PROPERTY_NAME = "navigator";
const char *SCALE_PROPERTY_NAME = "scale";
const char *BUNDLE_PROPERTY_NAME = "__bundle";
const char *THEME_PROPERTY_NAME = "theme";

WindowPropertySupport::WindowPropertySupport(
    JSGlobalContextRef context) :
    m_scale(0),
    m_bundle(""),
    m_theme(""),
    m_context(context)
{}

WindowPropertySupport::~WindowPropertySupport()
{}

float WindowPropertySupport::getScale() const
{
    return m_scale;
}

std::string WindowPropertySupport::getBundle() const
{
    return m_bundle;
}

std::string WindowPropertySupport::getTheme() const
{
    return m_theme;
}

JSGlobalContextRef WindowPropertySupport::getJSContext() const
{
    return m_context;
}

void WindowPropertySupport::setScaleToNavigatorProperty(const double scale)
{
    _D("set window.navigator.scale: %ld", scale);
    m_scale = scale;
    setPropertyToNavigator(SCALE_PROPERTY_NAME,
                           JSValueMakeNumber(m_context, scale));
}

void WindowPropertySupport::setBundleToWindowProperty(const char* bundle)
{
    _D("set window.__bundle: %s", bundle);
    if (bundle) {
        m_bundle = bundle;
        JSStringRef bundleString = JSStringCreateWithUTF8CString(bundle);
        setPropertyToWindow(BUNDLE_PROPERTY_NAME,
                            JSValueMakeString(m_context, bundleString));
        JSStringRelease(bundleString);
    }
}

void WindowPropertySupport::setThemeToNavigatorProperty(const char* theme)
{
    _D("set window.navigator.__theme: %s", theme);
    if (theme) {
        m_theme = theme;
        JSStringRef themeString = JSStringCreateWithUTF8CString(theme);
        setPropertyToNavigator(THEME_PROPERTY_NAME,
                               JSValueMakeString(m_context, themeString));
        JSStringRelease(themeString);
    }
}

void WindowPropertySupport::setPropertyToWindow(const char* propertyName,
                                                JSValueRef jsValue)
{
    _D("et property to window : %s", propertyName);
    if (propertyName) {
        JSObjectRef globalObject = JSContextGetGlobalObject(m_context);
        JSStringRef propertyNameString =
            JSStringCreateWithUTF8CString(propertyName);
        JSObjectSetProperty(m_context,
                            globalObject,
                            propertyNameString,
                            jsValue,
                            kJSPropertyAttributeNone,
                            NULL);
        JSStringRelease(propertyNameString);
    }
}

void WindowPropertySupport::setPropertyToNavigator(const char* propertyName,
                                                   JSValueRef jsValue)
{
    _D("set property to navigator : %s", propertyName);
    if (propertyName) {
        JSObjectRef globalObject = JSContextGetGlobalObject(m_context);

        JSStringRef navigatorString =
            JSStringCreateWithUTF8CString(NAVIGATOR_PROPERTY_NAME);
        JSValueRef navigatorValue = JSObjectGetProperty(m_context,
                                                        globalObject,
                                                        navigatorString,
                                                        NULL);

        JSStringRef propertyNameString =
            JSStringCreateWithUTF8CString(propertyName);
        JSObjectSetProperty(m_context,
                            JSValueToObject(m_context, navigatorValue, NULL),
                            propertyNameString,
                            jsValue,
                            kJSPropertyAttributeNone,
                            NULL);

        JSStringRelease(propertyNameString);
        JSStringRelease(navigatorString);
    }
}

