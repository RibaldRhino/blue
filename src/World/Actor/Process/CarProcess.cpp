#include "CarProcess.hpp"
#include "../../../Game/Game.hpp"
#include "../../../Game/InputEvents/KeyboardEvent.hpp"
#include <memory>
#include <iostream>

CarProcess::CarProcess(Actor* actor) : AbstractProcess(), _actor(actor) {}

void CarProcess::VInit()
{
    _state = ProcessState::RUNNING;
    _accelerationOn = false;
    _breakOn = false;
    auto eventManager = Game::getInstance().getEventManager();
    _connection = eventManager->AddListener(EventType::KEYBOARD_EVENT ,
            [&](IEventDataSPtr eventData)
            {
                std::shared_ptr<KeyboardEvent> keyboardEventDataSPtr = std::dynamic_pointer_cast<KeyboardEvent, IEventData>(eventData);
                if(keyboardEventDataSPtr->key == GLFW_KEY_W && keyboardEventDataSPtr->action == GLFW_PRESS)
                { _accelerationOn = true; _breakOn = false; }
                else if(keyboardEventDataSPtr->key == GLFW_KEY_W && keyboardEventDataSPtr->action == GLFW_RELEASE)
                    _accelerationOn = false;
                else if(keyboardEventDataSPtr->key == GLFW_KEY_S && keyboardEventDataSPtr->action == GLFW_PRESS)
                { _breakOn = true; _accelerationOn = false; }
                else if(keyboardEventDataSPtr->key == GLFW_KEY_S && keyboardEventDataSPtr->action == GLFW_RELEASE)
                    _breakOn = false;

            });
}

void CarProcess::VUpdate(double deltaTime)
{
    if(_accelerationOn)
    {
        std::cout<<"accel"<<std::endl;//_actor->getPhysics()->Accelerate(deltaTime);
    }
    else if(_breakOn)
    {
        std::cout<<"decel"<<std::endl;//_actor->getPhysics()->Decelerate(deltaTime);
    }
}

void CarProcess::VEnd()
{
    _connection.disconnect();
}
