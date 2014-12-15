#include "OnUpReleased.hpp"

namespace event {
    OnUpReleased::OnUpReleased() {

    }

    EventType OnUpReleased::VGetEventType() const {
        return EventType::KEYBOARD_UP_RELEASED;
    }

    double OnUpReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnUpReleased::VGetName() const {
        return "OnUpReleased";
    }
}
