#include "OnWindowClose.hpp"

namespace event {
    OnWindowClose::OnWindowClose() {

    }

    EventType OnWindowClose::VGetEventType() const {
        return EventType::WINDOW_CLOSE;
    }

    double OnWindowClose::VGetTimeStamp() const {
        return 0;
    }

    const char *OnWindowClose::VGetName() const {
        return "OnWindowClose";
    }
}
