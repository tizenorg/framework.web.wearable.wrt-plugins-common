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
#ifndef WRTDEVICEAPIS_COMMONS_EVENT_LISTENER_H_
#define WRTDEVICEAPIS_COMMONS_EVENT_LISTENER_H_

#include <memory>

#include <dpl/shared_ptr.h> //TODO: temporary code: DPL::SharedPtr -> std::shared_ptr
#include <Commons/ThreadPool.h>
#include <Commons/EventReceiver.h>

namespace WrtDeviceApis {
namespace Commons {
/**
 * Base class for listeners.
 * Object that is to act as listener should dervie from this class and implement
 * OnAnswerReceived() function.
 */
template<class TemplateEvent>
class EventListener : private EventReceiver<TemplateEvent>
{
  public:
    EventListener(ThreadEnum::Enumeration threadType) :
        EventReceiver<TemplateEvent>(threadType)
    {}

    virtual void onAnswerReceived(const DPL::SharedPtr<TemplateEvent>& event)
    { LogError("Pure virtual function call"); } //TODO: temporary code: DPL::SharedPtr -> std::shared_ptr

    virtual void onAnswerReceived(const std::shared_ptr<TemplateEvent>& event)
    { LogError("Pure virtual function call"); } //TODO: temporary code: DPL::SharedPtr -> std::shared_ptr

    void postAnswer(const DPL::SharedPtr<TemplateEvent>& event)
    {
        DPL::Event::ControllerEventHandler<DPL::SharedPtr<TemplateEvent> >::
            PostEvent(
            event);
    }

  protected:
    void OnEventReceived(const DPL::SharedPtr<TemplateEvent> &event)
    {
        onAnswerReceived(event);
    }
};

}
} // WrtDeviceApisCommon

#endif /* WRTDEVICEAPIS_COMMONS_EVENT_LISTENER_H_ */
