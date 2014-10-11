#include "OnWindowClose.hpp"

namespace event {
    OnWindowClose::OnWindowClose() {

    }

    const EventType OnWindowClose::VGetEventType() const {
        return EventType::WINDOW_CLOSE;
    }

    const double OnWindowClose::VGetTimeStamp() const {
        return 0;
    }

    const char *OnWindowClose::VGetName() const {
        return "OnWindowClose";
    }
}