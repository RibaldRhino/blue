#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnLMBPressed : public IEventData {
    public:
        OnLMBPressed();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
