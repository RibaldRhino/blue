#include "CarProcess.hpp"
#include "../../../Game/Game.hpp"
#include "../../../Game/InputEvents/KeyboardEvent.hpp"
#include "../../../Game/InputEvents/MouseEvent.hpp"
#include <memory>
#include <iostream>
#include <glm/gtx/transform.hpp>

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
        _actor->_transformUPtr->Translate(glm::vec3(0.0f, 0.0f, 4.f*deltaTime));
    }
    else if(_breakOn)
    {
        _actor->_transformUPtr->Translate(glm::vec3(0.0f, 0.0f, -4.f*deltaTime));
    }

}

void CarProcess::VEnd()
{
    _connection.disconnect();
}
