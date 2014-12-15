#include "OnLMBPressed.hpp"

namespace event {
    OnLMBPressed::OnLMBPressed() {

    }

    EventType OnLMBPressed::VGetEventType() const {
        return EventType::MOUSE_LMB_PRESSED;
    }

    double OnLMBPressed::VGetTimeStamp() const {
        return 0;
    }

    const char *OnLMBPressed::VGetName() const {
        return "OnLMBPressed";
    }
}
