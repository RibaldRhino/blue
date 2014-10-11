#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnRightReleased : public IEventData {
    public:
        OnRightReleased();

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}