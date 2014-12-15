#include "OnUpPressed.hpp"

namespace event {
    OnUpPressed::OnUpPressed() {

    }

    EventType OnUpPressed::VGetEventType() const {
        return EventType::KEYBOARD_UP_PRESSED;
    }

    double OnUpPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnUpPressed::VGetName() const {
        return "OnUpPressed";
    }
}
