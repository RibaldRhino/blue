#include "OnUpPressed.hpp"

namespace event {
    OnUpPressed::OnUpPressed() {

    }

    const EventType OnUpPressed::VGetEventType() const {
        return EventType::KEYBOARD_UP_PRESSED;
    }

    const double OnUpPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnUpPressed::VGetName() const {
        return "OnUpPressed";
    }
}