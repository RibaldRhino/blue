#include "Game.hpp"
#include "GameProcesses/InitProcess.hpp"

Game::Game()
{
    _windowUPtr = std::unique_ptr<Window>(new Window(800, 600, "SPH", this, false));
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());
    _worldUPtr = std::unique_ptr<World>(new World());

    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new InitProcess()));
}

void Game::Run()
{
    double currentTime = glfwGetTime();
    double previousTime = glfwGetTime();
    while(!_processManagerUPtr->Ended())
    {
        _processManagerUPtr->Update(currentTime - previousTime);
        previousTime = currentTime;
        currentTime = glfwGetTime();
    }
}
