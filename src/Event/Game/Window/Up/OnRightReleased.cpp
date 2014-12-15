#include "OnRightReleased.hpp"

namespace event {
    OnRightReleased::OnRightReleased() {

    }

    EventType OnRightReleased::VGetEventType() const {
        return EventType::KEYBOARD_RIGHT_RELEASED;
    }

    double OnRightReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnRightReleased::VGetName() const {
        return "OnRightReleased";
    }
}
