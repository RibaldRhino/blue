#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnLMBReleased : public IEventData {
    public:
        OnLMBReleased();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
