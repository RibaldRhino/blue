#include "CarProcess.hpp"
#include "../../../Game/Game.hpp"

CarProcess::CarProcess(Actor* actor) : AbstractProcess()
{
    auto eventManager = Game::getInstance().getEventManager();
    eventManager->AddListener(EventType::KEYBOARD_EVENT , [])
}

void CarProcess::VInit() {

}

void CarProcess::VUpdate(double deltaTime) {

}

void CarProcess::VEnd() {

}
