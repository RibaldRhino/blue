#include "MouseEvent.hpp"
#include "../EventType.hpp"

MouseEvent::MouseEvent(double xPos, double yPos, int button, int action, int mods) :
        xPos(xPos), yPos(yPos), button(button), action(action), mods(mods)
{

}

const EventType MouseEvent::VGetEventType() const
{
    return EventType::MOUSE_EVENT;
}

MouseEvent::~MouseEvent() {

}

const double MouseEvent::VGetTimeStamp() const {
    return 0;
}

const char *MouseEvent::VGetName() const {
    return nullptr;
}
