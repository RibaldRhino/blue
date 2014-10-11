#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnLeftPressed : public IEventData {
    public:
        OnLeftPressed();

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}