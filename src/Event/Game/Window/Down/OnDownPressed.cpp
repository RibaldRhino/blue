#include "OnDownPressed.hpp"

namespace event {
    OnDownPressed::OnDownPressed() {

    }

    EventType OnDownPressed::VGetEventType() const {
        return EventType::KEYBOARD_DOWN_PRESSED;
    }

    double OnDownPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnDownPressed::VGetName() const {
        return "OnDownPressed";
    }
}
