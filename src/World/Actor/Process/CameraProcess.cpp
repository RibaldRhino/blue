#include "CameraProcess.hpp"
#include "../../../Game/Game.hpp"
#include "../../../Game/InputEvents/KeyboardEvent.hpp"
#include "../../../Game/InputEvents/MouseEvent.hpp"


void CameraProcess::VInit() {
    _state = ProcessState::RUNNING;
    _accelerationOn = false;
    _breakOn = false;
    _mouseMove = glm::vec2(0, 0);
    _prevPos = glm::vec2(0, 0);
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
    _connection = eventManager->AddListener(EventType::MOUSE_EVENT ,
            [&](IEventDataSPtr eventData)
            {
                std::shared_ptr<MouseEvent> mouseEventDataSPtr = std::dynamic_pointer_cast<MouseEvent, IEventData>(eventData);
                _mouseMove = glm::vec2(mouseEventDataSPtr->xPos - _prevPos.x, mouseEventDataSPtr->yPos - _prevPos.y);
                _prevPos = glm::vec2(mouseEventDataSPtr->xPos, mouseEventDataSPtr->yPos);
            });
}

void CameraProcess::VUpdate(double deltaTime) {
    if(_accelerationOn)
    {
        _camera->_transformUPtr->getMatrix() = glm::translate(_camera->_transformUPtr->getMatrix(), glm::vec3(0.0f, 0.f, 0.001f));
        //std::cout<<_camera->_transformUPtr->getMatrix()[3][0]<<" "<<_camera->_transformUPtr->getMatrix()[3][1]<<" "<<_camera->_transformUPtr->getMatrix()[3][2]<<" "<<_camera->_transformUPtr->getMatrix()[3][3]<<std::endl;
    }
    else if(_breakOn)
    {
        _camera->_transformUPtr->getMatrix() = glm::translate(_camera->_transformUPtr->getMatrix(), glm::vec3(0.0f, 0.f, -0.001f));
    }
    _camera->_transformUPtr->Rotate(_mouseMove.y/100, glm::vec3(0.f, 1.f, 0.f));
    _camera->_transformUPtr->Rotate(_mouseMove.x/100, glm::vec3(1.f, 0.f, 0.f));
    _mouseMove.y = 0;
    _mouseMove.x = 0;
}

void CameraProcess::VEnd() {
    _connection.disconnect();
}
