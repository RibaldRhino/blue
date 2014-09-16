#pragma once

#include "../../Event/IEventData.hpp"

enum class EventType;

class KeyboardEvent: public IEventData
{
public:
    int key, code, action, mods;
public:
    KeyboardEvent(int key, int code, int action, int mods);
    ~KeyboardEvent();
    virtual const EventType VGetEventType() const;
    virtual const double VGetTimeStamp() const;
    virtual const char* VGetName() const;
};