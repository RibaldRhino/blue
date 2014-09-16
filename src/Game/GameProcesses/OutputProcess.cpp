#include <iostream>
#include "OutputProcess.hpp"
#include "../Game.hpp"

OutputProcess::OutputProcess() : AbstractProcess() {}

void OutputProcess::VUpdate(double deltaTime)
{
    std::cout<<"Rendering world"<<std::endl;
    Game::getInstance().getWorld()->Render();
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
