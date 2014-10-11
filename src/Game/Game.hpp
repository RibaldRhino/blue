#pragma once
#include <memory>
#include <boost/noncopyable.hpp>
#include "World/World.hpp"
#include "Window/Window.hpp"
#include "Process/ProcessManager.hpp"
#include "Event/EventManager.hpp"
#include "Timer.hpp"
namespace game {
    class Game : public boost::noncopyable {
    private:
        std::unique_ptr<Window> _windowUPtr;
        std::unique_ptr<process::ProcessManager> _processManagerUPtr;
        std::unique_ptr<World> _worldUPtr;
        std::unique_ptr<Timer> _timerUPtr;

    public:
        Game();
        void Run();

        World *getWorld() {
            return _worldUPtr.get();
        }

        Window *getWindow() {
            return _windowUPtr.get();
        }

        Timer *getTimer() {
            return _timerUPtr.get();
        }
    };
}