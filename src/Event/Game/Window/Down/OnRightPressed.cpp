#include "OnRightPressed.hpp"

namespace event {
    OnRightPressed::OnRightPressed() {

    }

    const EventType OnRightPressed::VGetEventType() const {
        return EventType::KEYBOARD_RIGHT_PRESSED;
    }

    const double OnRightPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnRightPressed::VGetName() const {
        return "OnRightPressed";
    }
}