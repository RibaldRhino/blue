#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnLeftReleased : public IEventData {
    public:
        OnLeftReleased();

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}