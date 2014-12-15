#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnDownReleased : public IEventData {
    public:
        OnDownReleased();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
