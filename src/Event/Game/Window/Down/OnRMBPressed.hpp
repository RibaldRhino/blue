#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnRMBPressed : public IEventData {
    public:
        OnRMBPressed();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
