#include <iostream>

#include "InitProcess.hpp"
#include "../Game.hpp"
#include "OutputProcess.hpp"
#include "EventDispatchProcess.hpp"
#include "WorldUpdateProcess.hpp"

InitProcess::InitProcess()
{
    AddSuccessor(std::unique_ptr<AbstractProcess>(new WorldUpdateProcess()));
    AddSuccessor(std::unique_ptr<AbstractProcess>(new OutputProcess()));
    AddSuccessor(std::unique_ptr<AbstractProcess>(new EventDispatchProcess()));
}

void InitProcess::VUpdate(double deltaTime)
{
    std::cout<<"Updating init process"<<std::endl;
    _state = ProcessState::SUCCEEDED;
}

void InitProcess::VInit()
{
    std::cout<<"Initializing init process"<<std::endl;
    _state = ProcessState::RUNNING;
}

void InitProcess::VEnd()
{
    std::cout<<"Ending init process"<<std::endl;
}
