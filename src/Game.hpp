#pragma once

#include <memory>
#include "EventManager.hpp"
#include "ProcessManager.hpp"
#include "World.hpp"

class Window;

class Game
{
private:
    std::unique_ptr<Window> _windowUPtr;
    std::unique_ptr<EventManager> _eventManagerUPtr;
    std::unique_ptr<ProcessManager> _processManagerUPtr;
    std::unique_ptr<World> _worldUPtr;
public:
    Game();
    void Run();

    World* getWorld() { return _worldUPtr.get(); }
    EventManager* getEventManager() { return _eventManagerUPtr.get(); }
};