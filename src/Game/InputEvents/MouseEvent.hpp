#pragma once

#include "../../Event/IEventData.hpp"

enum class EventType;

class MouseEvent: public IEventData
{
public:
    double xPos, yPos;
    int button, action, mods;
public:
    MouseEvent(double xPos, double yPos, int button, int action, int mods);
    ~MouseEvent();
    virtual const EventType VGetEventType() const;
    virtual const double VGetTimeStamp() const;
    virtual const char* VGetName() const;
};