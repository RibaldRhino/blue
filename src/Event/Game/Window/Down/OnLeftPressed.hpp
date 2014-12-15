#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnLeftPressed : public IEventData {
    public:
        OnLeftPressed();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
