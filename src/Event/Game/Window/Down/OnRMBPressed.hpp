#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnRMBPressed : public IEventData {
    public:
        OnRMBPressed();

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}