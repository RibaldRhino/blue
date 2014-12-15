#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnLeftReleased : public IEventData {
    public:
        OnLeftReleased();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
