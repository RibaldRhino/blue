#include "Game.hpp"

Game::Game()
{
    _windowUPtr = std::unique_ptr<Window>(new Window(800, 600, "SPH", false));
    _eventManagerUPtr = std::unique_ptr<EventManager>(new EventManager());
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());
}

void Game::Run()
{

}
