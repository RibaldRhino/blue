#include "OnLMBReleased.hpp"

namespace event {
    OnLMBReleased::OnLMBReleased() {

    }

    EventType OnLMBReleased::VGetEventType() const {
        return EventType::MOUSE_LMB_RELEASED;
    }

    double OnLMBReleased::VGetTimeStamp() const {
        return 0;
    }

    const char *OnLMBReleased::VGetName() const {
        return "OnLMBReleased";
    }
}
