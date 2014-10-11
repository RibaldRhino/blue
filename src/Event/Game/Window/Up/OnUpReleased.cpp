#include "OnUpReleased.hpp"

namespace event {
    OnUpReleased::OnUpReleased() {

    }

    const EventType OnUpReleased::VGetEventType() const {
        return EventType::KEYBOARD_UP_RELEASED;
    }

    const double OnUpReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnUpReleased::VGetName() const {
        return "OnUpReleased";
    }
}