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
/*
 * @file        PluginManager.cpp
 * @author      Lukasz Marek (l.marek@samsung.com)
 * @version     0.1
 * @brief
 */

#include "PluginManager.h"
#include <algorithm>
#include <dpl/wrt-dao-ro/widget_dao_read_only.h>
#include <dpl/wrt-dao-ro/plugin_dao_read_only.h>
#include <dpl/wrt-dao-ro/WrtDatabase.h>
#include <plugin_logic.h>
#include <js_types.h>
#include <dpl/wrt-dao-ro/common_dao_types.h>
#include <Commons/Exception.h>
#include <CommonsJavaScript/Converter.h>
#include <dpl/exception.h>
#include <dpl/log/secure_log.h>
#include <dpl/foreach.h>

using namespace std;

namespace {
const string GLOBAL_OBJECT_NAME = "GLOBAL_OBJECT";
const char* SEPARATOR = ".";
}

namespace WrtDeviceApis {
namespace PluginManager {
PluginManager::PluginManager(int widgetHandle,
                             const string &objectUri,
                             JSObjectRef object,
                             JSContextRef context) :
    m_widgetHandle(widgetHandle),
    m_objectInstance(object),
    m_objectUri(GLOBAL_OBJECT_NAME),
    m_shortUri(objectUri),
    m_context(context)
{
    m_objectUri.append(SEPARATOR).append(objectUri);
    WrtDB::WrtDatabase::attachToThreadRO();
}

PluginManager::~PluginManager()
{
    ObjectList::iterator it = m_objectList.begin();
    for (; it != m_objectList.end(); ++it) {
        JSValueUnprotect(m_context, it->second);
    }
    WrtDB::WrtDatabase::detachFromThread();
}

bool PluginManager::hasChild(const string &name) const
{
    const PropertyList &prop = getProperties();
    return prop.end() != find(prop.begin(), prop.end(), name);
}

bool PluginManager::loadChild(const string &name) const
{
    _D("loading %s", name.c_str());
    string localUri = m_objectUri;
    localUri.append(SEPARATOR).append(name);

    WrtDB::DbPluginHandle handle =
        WrtDB::PluginDAOReadOnly::getPluginHandleForImplementedObject(localUri);
    if (handle == WrtDB::INVALID_PLUGIN_HANDLE) {
        _E("Plugin not found");
        return false;
    }

    WrtDB::DbWidgetFeatureSet features;
    WrtDB::WidgetType widgetType;
    Try
    {
        WrtDB::WidgetDAOReadOnly dao(m_widgetHandle);
        features = dao.getFeaturesList();
        widgetType = dao.getWidgetType();
    }
    Catch(WrtDB::WidgetDAOReadOnly::Exception::Base)
    {
        // Error while reading database - widget handle may
        // be invalid or some data may be missing in database
        _E("Cannot get feature list");
        return false;
    }

    if (widgetType == WrtDB::APP_TYPE_TIZENWEBAPP) {
        JavaScriptObject jsObject = { m_objectInstance,
                                      m_shortUri };
        return PluginLogicSingleton::Instance().loadPluginOnDemand(
                   handle, jsObject, const_cast<JSGlobalContextRef>(m_context));
    }

    //check does plugin with feature was requested
    FOREACH(it, features)
    {
        if (it->pluginId == handle) {
            if (it->rejected) {
                _E("Feature rejected by ACE");
                continue;
            }

            JavaScriptObject jsObject = { m_objectInstance,
                                          m_shortUri };

            return PluginLogicSingleton::Instance().loadPluginOnDemand(
                       handle, jsObject,
                       const_cast<JSGlobalContextRef>(m_context));
        }
    }
    _E("Plugin not loaded");
    return false;
}

JSValueRef PluginManager::getProperty(const string &name) const
{
    _D("getProperty %s", name.c_str());
    ObjectList::const_iterator it = m_objectList.find(name);
    if (it != m_objectList.end()) {
        //return already set value
        return it->second;
    }

    if (!loadChild(name)) {
        ThrowMsg(Commons::PlatformException, "Cannot load plugin");
    }

    it = m_objectList.find(name);
    if (it != m_objectList.end()) {
        //return set value
        return it->second;
    }

    ThrowMsg(Commons::PlatformException, "Cannot obtain property");
}

bool PluginManager::setProperty(const string &name,
                                JSValueRef value)
{
    _D("setProperty %s", name.c_str());
    if (m_objectList.count(name) > 0) {
        JSValueUnprotect(m_context, m_objectList[name]);
    }
    JSValueProtect(m_context, value);
    m_objectList[name] = value;
    return true;
}

bool PluginManager::deleteProperty(const string &name)
{
    if (m_objectList.count(name) > 0) {
        _D("deleteProperty %s", name.c_str());
        JSValueUnprotect(m_context, m_objectList[name]);
        m_objectList.erase(name);
        return true;
    }
    return false;
}

Api::IPluginManager::PropertyList PluginManager::getProperties() const
{
    using namespace WrtDB;

    if (!!m_propertyCache) {
        return *m_propertyCache;
    }

    m_propertyCache = PropertyList();

    WidgetDAOReadOnly dao(m_widgetHandle);
    string localUri = m_objectUri + SEPARATOR;

    if (dao.getWidgetType() == APP_TYPE_TIZENWEBAPP)
    {
        static ImplementedObjectsList implObjectList =
            PluginDAOReadOnly::getImplementedObjects();

        FOREACH(it_implObject, implObjectList)
        {
            //check if implemented object stats with localUri
            if (it_implObject->find(localUri) == 0) {
                string property = *it_implObject;
                //remove local uri that predicts property name.
                property.erase(0, localUri.size());
                //check if property has its own properties.
                size_t pos = property.find(SEPARATOR);
                if (pos != string::npos) {
                    //if so then remove them.
                    property.erase(pos);
                }

                m_propertyCache->push_back(property);
            }
        }
    }
    else
    {
        DbWidgetFeatureSet features;

        Try
        {
            features = dao.getFeaturesList();
        }
        Catch(WidgetDAOReadOnly::Exception::Base)
        {
            _E("Cannot get feature list");
            ReThrow(Commons::PlatformException);
        }

        DbWidgetFeatureSet::const_iterator feature = features.begin();
        for (; feature != features.end(); ++feature) {
            ImplementedObjectsList implObjs =
                PluginDAOReadOnly::getImplementedObjectsForPluginHandle(
                    feature->pluginId);
            FOREACH(it, implObjs) {
                //check if implemented object stats with localUri
                if (it->find(localUri) == 0) {
                    string property = *it;
                    //remove local uri that predicts property name.
                    property.erase(0, localUri.size());
                    //check if property has its own properties.
                    size_t pos = property.find(SEPARATOR);
                    if (pos != string::npos) {
                        //if so then remove them.
                        property.erase(pos);
                    }
                    m_propertyCache->push_back(property);
                }
            }
        }
    }

    return *m_propertyCache;
}

void PluginManager::addPropertiesToList(
    JSPropertyNameAccumulatorRef propertyNames) const
{
    PropertyList properties = getProperties();
    CommonsJavaScript::Converter converter(m_context);
    FOREACH(it, properties) {
        JSStringRef name = converter.toJSStringRef(*it);
        JSPropertyNameAccumulatorAddName(propertyNames, name);
        JSStringRelease(name);
    }
}
}
}
