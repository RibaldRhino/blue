#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnDownReleased : public IEventData {
    public:
        OnDownReleased();

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}