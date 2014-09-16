#include <iostream>
#include "InputProcess.hpp"
#include "../Game.hpp"

InputProcess::InputProcess() {

}

void InputProcess::VUpdate(double deltaTime) {
    //std::cout<<"Polling"<<std::endl;
    Game::getInstance().getWindow()->PollEvents();
}

void InputProcess::VInit() {
    _state = ProcessState::RUNNING;
}

void InputProcess::VEnd() {

}
