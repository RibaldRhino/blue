#include "Game.hpp"

Game::Game()
{
    _windowUPtr = std::unique_ptr<Window>(new Window(800, 600, "SPH", false));
    _eventManagerSPtr = std::make_shared<EventManager>();
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());
}

void Game::Run()
{

}
