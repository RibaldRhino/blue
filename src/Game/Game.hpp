#pragma once

#include <memory>
#include <boost/noncopyable.hpp>
#include "../Event/EventManager.hpp"
#include "../Process/ProcessManager.hpp"
#include "../World/World.hpp"
#include "../Window/Window.hpp"

class Game : public boost::noncopyable
{
private:
    std::unique_ptr<Window> _windowUPtr;
    std::unique_ptr<ProcessManager> _processManagerUPtr;
    std::unique_ptr<EventManager> _eventManagerUPtr;
    std::unique_ptr<World> _worldUPtr;

private:
    Game();

public:
    static Game& getInstance() {
        static Game instance;
        return instance;
    }
    void Run();
    World* getWorld() { return _worldUPtr.get(); }
    EventManager* getEventManager() { return _eventManagerUPtr.get(); }
};