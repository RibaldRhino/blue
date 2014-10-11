#include "OnRMBPressed.hpp"

namespace event {
    OnRMBPressed::OnRMBPressed() {

    }

    const EventType OnRMBPressed::VGetEventType() const {
        return EventType::MOUSE_RMB_PRESSED;
    }

    const double OnRMBPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnRMBPressed::VGetName() const {
        return "OnRMBPressed";
    }
}