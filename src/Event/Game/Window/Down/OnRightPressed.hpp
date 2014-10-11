#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnRightPressed : public IEventData {
    public:
        OnRightPressed();

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}