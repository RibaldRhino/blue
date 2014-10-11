#include "OnDownReleased.hpp"

namespace event {
    OnDownReleased::OnDownReleased() {

    }

    const EventType OnDownReleased::VGetEventType() const {
        return EventType::KEYBOARD_DOWN_RELEASED;
    }

    const double OnDownReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnDownReleased::VGetName() const {
        return "OnDownReleased";
    }
}