#include "OnLeftReleased.hpp"

namespace event {
    OnLeftReleased::OnLeftReleased() {

    }

    EventType OnLeftReleased::VGetEventType() const {
        return EventType::KEYBOARD_LEFT_RELEASED;
    }

    double OnLeftReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnLeftReleased::VGetName() const {
        return "OnLeftReleased";
    }
}
