#include "OnWindowClosedEvent.hpp"
#include "../../Event/EventType.hpp"

OnWindowClosedEvent::OnWindowClosedEvent()
{

}

const EventType OnWindowClosedEvent::VGetEventType() const
{
    return EventType::ON_WINDOW_CLOSED_EVENT;
}


OnWindowClosedEvent::~OnWindowClosedEvent() {

}

const double OnWindowClosedEvent::VGetTimeStamp() const {
    return 0;
}

const char *OnWindowClosedEvent::VGetName() const {
    return nullptr;
}
