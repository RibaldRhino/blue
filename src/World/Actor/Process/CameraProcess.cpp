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
    _pitch_up = false;
    _pitch_down - false;
    _steerLeft = _steerRight = false;
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
                if(keyboardEventDataSPtr->key == GLFW_KEY_UP && keyboardEventDataSPtr->action == GLFW_PRESS)
                { _pitch_up = true; _pitch_down = false; }
                else if(keyboardEventDataSPtr->key == GLFW_KEY_UP && keyboardEventDataSPtr->action == GLFW_RELEASE)
                    _pitch_up = false;
                else if(keyboardEventDataSPtr->key == GLFW_KEY_DOWN && keyboardEventDataSPtr->action == GLFW_PRESS)
                { _pitch_down = true; _pitch_up = false; }
                else if(keyboardEventDataSPtr->key == GLFW_KEY_DOWN && keyboardEventDataSPtr->action == GLFW_RELEASE)
                    _pitch_down = false;
                if(keyboardEventDataSPtr->key == GLFW_KEY_RIGHT && keyboardEventDataSPtr->action == GLFW_PRESS)
                { _steerRight = true; _steerLeft= false; }
                else if(keyboardEventDataSPtr->key == GLFW_KEY_RIGHT && keyboardEventDataSPtr->action == GLFW_RELEASE)
                    _steerRight = false;
                else if(keyboardEventDataSPtr->key == GLFW_KEY_LEFT && keyboardEventDataSPtr->action == GLFW_PRESS)
                { _steerLeft = true; _steerRight = false; }
                else if(keyboardEventDataSPtr->key == GLFW_KEY_LEFT && keyboardEventDataSPtr->action == GLFW_RELEASE)
                    _steerLeft= false;

            });
}

void CameraProcess::VUpdate(double deltaTime) {
    if(_accelerationOn)
    {
        _camera->_cam_pos[2] -= _camera->_cam_speed * deltaTime;
    }
    else if(_breakOn)
    {
        _camera->_cam_pos[2] += _camera->_cam_speed * deltaTime;
    }
    if(_pitch_up)
    {
        _camera->_cam_pitch += _camera->_cam_yaw_pitch_speed * deltaTime;
    }
    else if(_pitch_down)
    {
        _camera->_cam_pitch -= _camera->_cam_yaw_pitch_speed * deltaTime;
    }
    if(_steerRight)
    {
        _camera->_cam_yaw -=  _camera->_cam_yaw_pitch_speed * deltaTime;
    }
    else if(_steerLeft)
    {
        _camera->_cam_yaw +=  _camera->_cam_yaw_pitch_speed * deltaTime;
    }
}

void CameraProcess::VEnd() {
    _connection.disconnect();
}
