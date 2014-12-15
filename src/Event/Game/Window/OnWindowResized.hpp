#include <Event/IEventData.hpp>

#pragma once

namespace event {
    class OnWindowResized : public IEventData {
    private:
        const int _width;
        const int _height;

    public:
        OnWindowResized(int width, int height);

        virtual EventType VGetEventType() const;

        virtual double VGetTimeStamp() const;

        virtual const char *VGetName() const;

        int getWidth() const { return _width; }
        int getHeight() const { return _height; }

    };
}
