#include "InitProcess.hpp"
#include "../Game.hpp"
#include "OutputProcess.hpp"
#include "EventDispatchProcess.hpp"
#include "WorldUpdateProcess.hpp"

InitProcess::InitProcess(Game* game)
{
    AddSuccessor(std::unique_ptr<AbstractProcess>(new WorldUpdateProcess(game)));
    AddSuccessor(std::unique_ptr<AbstractProcess>(new OutputProcess(game)));
    AddSuccessor(std::unique_ptr<AbstractProcess>(new EventDispatchProcess(game)));
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
