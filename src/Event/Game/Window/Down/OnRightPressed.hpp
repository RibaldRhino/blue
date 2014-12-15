#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnRightPressed : public IEventData {
    public:
        OnRightPressed();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
