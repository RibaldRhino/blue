#include <iostream>

#include "EventDispatchProcess.hpp"
#include "../Game.hpp"

EventDispatchProcess::EventDispatchProcess() : AbstractProcess() {}

void EventDispatchProcess::VUpdate(double deltaTime)
{
    //std::cout<<"Updating game event manager"<<std::endl;
    Game::getInstance().getEventManager()->Update(deltaTime);
}

void EventDispatchProcess::VInit()
{
    //std::cout<<"Initializing event dispatcher process"<<std::endl;
    _state = ProcessState::RUNNING;
}

void EventDispatchProcess::VEnd()
{
    //std::cout<<"Ending event dispatcher process"<<std::endl;
}
