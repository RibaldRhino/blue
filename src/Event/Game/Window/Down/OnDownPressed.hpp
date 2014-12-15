#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnDownPressed : public IEventData {
    public:
        OnDownPressed();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
