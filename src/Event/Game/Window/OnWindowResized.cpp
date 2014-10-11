#include "OnWindowResized.hpp"

event::OnWindowResized::OnWindowResized(const int width, const int height) : _width(width), _height(height){}

event::EventType const event::OnWindowResized::VGetEventType() const {
    return event::EventType::WINDOW_RESIZED ;
}

const double event::OnWindowResized::VGetTimeStamp() const {
    return 0;
}

const char *event::OnWindowResized::VGetName() const {
    return "OnWindowResized";
}
