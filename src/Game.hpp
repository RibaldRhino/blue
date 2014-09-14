#pragma once

#include <memory>
#include "Window.hpp"
#include "EventManager.hpp"
#include "ProcessManager.hpp"

class Game
{
private:
    std::unique_ptr<Window> _windowUPtr;
    std::shared_ptr<EventManager> _eventManagerSPtr;
    std::unique_ptr<ProcessManager> _processManagerUPtr;
public:
    Game();
    void Run();
};