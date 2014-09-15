#include <iostream>
#include "OutputProcess.hpp"
#include "../Game.hpp"

OutputProcess::OutputProcess(Game* game) : AbstractProcess()
{
    _game = game;
}

void OutputProcess::VUpdate(double deltaTime)
{
    std::cout<<"Rendering world"<<std::endl;
    _game->getWorld()->Render();
}

void OutputProcess::VInit()
{
    std::cout<<"Initializing output process"<<std::endl;
    _state = ProcessState::RUNNING;
}

void OutputProcess::VEnd()
{
    std::cout<<"Ending output process"<<std::endl;
}
