#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnUpPressed : public IEventData {
    public:
        OnUpPressed();

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}