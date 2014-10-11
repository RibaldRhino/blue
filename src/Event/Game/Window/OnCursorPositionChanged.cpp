#include "OnCursorPositionChanged.hpp"

event::EventType const event::OnCursorPositionChanged::VGetEventType() const {
    return event::EventType::MOUSE_CURSOR_POS;
}

const double event::OnCursorPositionChanged::VGetTimeStamp() const {
    return 0;
}

const char *event::OnCursorPositionChanged::VGetName() const {
    return "OnCursorPositionChanged";
}

event::OnCursorPositionChanged::OnCursorPositionChanged(glm::vec2 vec) {
    moveVec = vec;
}
