#include <Event/IEventData.hpp>

#pragma once

namespace event {
    class OnWindowResized : public IEventData {
    private:
        const int _width;
        const int _height;

    public:
        OnWindowResized(int width, int height);

        virtual const EventType VGetEventType() const;

        virtual const double VGetTimeStamp() const;

        virtual const char *VGetName() const;

        const int getWidth() const { return _width; }
        const int getHeight() const { return _height; }

    };
}