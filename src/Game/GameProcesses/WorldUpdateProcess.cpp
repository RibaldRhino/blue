#include <iostream>

#include "WorldUpdateProcess.hpp"



WorldUpdateProcess::WorldUpdateProcess(Game* game) : AbstractProcess()
{
    _game = game;
}

void WorldUpdateProcess::VUpdate(double deltaTime)
{
    std::cout<<"Updating world"<<std::endl;
    _game->getWorld()->Update(deltaTime);
}

void WorldUpdateProcess::VInit() {
    std::cout<<"Initializing world update process"<<std::endl;
    _state = ProcessState::RUNNING;
}

void WorldUpdateProcess::VEnd() {
    std::cout<<"Ending world update process"<<std::endl;
}
