#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnRMBReleased : public IEventData {
    public:
        OnRMBReleased();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
