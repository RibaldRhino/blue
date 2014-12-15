#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnRightReleased : public IEventData {
    public:
        OnRightReleased();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
