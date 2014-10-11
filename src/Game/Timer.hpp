#pragma once

namespace game {
    class Timer {
    private:
        double _previousTime;
        bool _running;

    public:
        bool Running();

        void Start();

        double Delta();

        void Stop();


    };
}