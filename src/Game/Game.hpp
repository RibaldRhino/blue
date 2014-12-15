#pragma once
#include <memory>
#include <boost/noncopyable.hpp>
#include "World/World.hpp"
#include "System/Window/Window.hpp"
#include "Process/ProcessManager.hpp"
#include "Event/EventManager.hpp"
#include "Timer.hpp"
namespace game {
    class Game : public boost::noncopyable {
    private:
        gamesystem::Window* _windowPtr;
        std::unique_ptr<process::ProcessManager> _processManagerUPtr;
        std::unique_ptr<World> _worldUPtr;
        std::unique_ptr<Timer> _timerUPtr;

    public:
        Game();
        void Run();

        World *getWorld() const {
            return _worldUPtr.get();
        }

        gamesystem::Window *getWindow() const {
            return _windowPtr;
        }

        Timer *getTimer() const {
            return _timerUPtr.get();
        }
    };
}