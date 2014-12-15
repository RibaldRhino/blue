#include "OnLeftPressed.hpp"

namespace event {
    OnLeftPressed::OnLeftPressed() {

    }

    EventType OnLeftPressed::VGetEventType() const {
        return EventType::KEYBOARD_LEFT_PRESSED;
    }

    double OnLeftPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnLeftPressed::VGetName() const {
        return "OnLeftPressed";
    }
}
