#include "Game.hpp"
#include "GameProcesses/InitProcess.hpp"
#include <cstdlib>

Game::Game()
{
    _windowUPtr = std::unique_ptr<Window>(new Window(800, 600, "SPH", false));
    _eventManagerUPtr = std::unique_ptr<EventManager>(new EventManager());
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());
    _worldUPtr = std::unique_ptr<World>(new World());

    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new InitProcess(this)));
}

void Game::Run()
{
    while(!_processManagerUPtr->Ended())
    {
        _processManagerUPtr->Update(0);
    }
}
