#include "OnRightReleased.hpp"

namespace event {
    OnRightReleased::OnRightReleased() {

    }

    const EventType OnRightReleased::VGetEventType() const {
        return EventType::KEYBOARD_RIGHT_RELEASED;
    }

    const double OnRightReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnRightReleased::VGetName() const {
        return "OnRightReleased";
    }
}