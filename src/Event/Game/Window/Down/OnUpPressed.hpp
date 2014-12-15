#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnUpPressed : public IEventData {
    public:
        OnUpPressed();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
