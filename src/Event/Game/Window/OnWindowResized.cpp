#include "OnWindowResized.hpp"

event::OnWindowResized::OnWindowResized(const int width, const int height) : _width(width), _height(height){}

event::EventType event::OnWindowResized::VGetEventType() const {
    return event::EventType::WINDOW_RESIZED ;
}

double event::OnWindowResized::VGetTimeStamp() const {
    return 0;
}

const char *event::OnWindowResized::VGetName() const {
    return "OnWindowResized";
}
