#include <GLFW/glfw3.h>
#include "Timer.hpp"


namespace game {

    void Timer::Start() {
        _previousTime = glfwGetTime();
        _running = true;
    }

    double Timer::Delta() {
        if (_running) {
            double currentTime = glfwGetTime();
            double delta = currentTime - _previousTime;
            _previousTime = currentTime;
            return delta;
        }
        return 0;
    }

    void Timer::Stop() {
        _running = false;
    }


    bool Timer::Running() {
        return _running;
    }
}