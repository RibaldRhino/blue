#pragma once

#include "IEventData.hpp"

enum class EventType;

class OnWindowClosedEvent: public IEventData
{
public:
    OnWindowClosedEvent();
    ~OnWindowClosedEvent();
    virtual const EventType VGetEventType() const;
    virtual const double VGetTimeStamp() const;
    virtual const char* VGetName() const;
};