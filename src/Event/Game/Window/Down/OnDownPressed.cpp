#include "OnDownPressed.hpp"

namespace event {
    OnDownPressed::OnDownPressed() {

    }

    const EventType OnDownPressed::VGetEventType() const {
        return EventType::KEYBOARD_DOWN_PRESSED;
    }

    const double OnDownPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnDownPressed::VGetName() const {
        return "OnDownPressed";
    }
}