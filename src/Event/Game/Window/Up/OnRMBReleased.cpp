#include "OnRMBReleased.hpp"

namespace event {
    OnRMBReleased::OnRMBReleased() {

    }

    const EventType OnRMBReleased::VGetEventType() const {
        return EventType::MOUSE_RMB_RELEASED;
    }

    const double OnRMBReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnRMBReleased::VGetName() const {
        return "OnRMBReleased";
    }
}