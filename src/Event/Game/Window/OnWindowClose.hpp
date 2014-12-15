#pragma once

#include <Event/IEventData.hpp>

namespace event {

    enum class EventType;

    class OnWindowClose : public IEventData {
    public:
        OnWindowClose();

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;
    };
}
