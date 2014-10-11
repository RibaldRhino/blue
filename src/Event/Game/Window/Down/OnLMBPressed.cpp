#include "OnLMBPressed.hpp"

namespace event {
    OnLMBPressed::OnLMBPressed() {

    }

    const EventType OnLMBPressed::VGetEventType() const {
        return EventType::MOUSE_LMB_PRESSED;
    }

    const double OnLMBPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnLMBPressed::VGetName() const {
        return "OnLMBPressed";
    }
}