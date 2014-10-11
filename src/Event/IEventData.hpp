#pragma once

#include "EventType.hpp"
#include <memory>


namespace event {
    class IEventData {
    public:
        virtual const EventType VGetEventType() const = 0;

        virtual const double VGetTimeStamp() const = 0;

        virtual const char *VGetName() const = 0;
    };

    typedef std::shared_ptr<IEventData> IEventDataSPtr;
}