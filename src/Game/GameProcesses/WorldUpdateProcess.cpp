#include <iostream>

#include "WorldUpdateProcess.hpp"



WorldUpdateProcess::WorldUpdateProcess() : AbstractProcess() {}

void WorldUpdateProcess::VUpdate(double deltaTime)
{
    std::cout<<"Updating world"<<std::endl;
    Game::getInstance().getWorld()->Update(deltaTime);
}

void WorldUpdateProcess::VInit() {
    std::cout<<"Initializing world update process"<<std::endl;
    _state = ProcessState::RUNNING;
}

void WorldUpdateProcess::VEnd() {
    std::cout<<"Ending world update process"<<std::endl;
}
