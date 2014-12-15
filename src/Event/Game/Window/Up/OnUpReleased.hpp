#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnUpReleased : public IEventData {
    public:
        OnUpReleased();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
