#include "CarProcess.hpp"
#include "../../../Game/Game.hpp"
#include "../../../Game/InputEvents/KeyboardEvent.hpp"
#include <memory>

CarProcess::CarProcess(Actor* actor) : AbstractProcess(), _accelerationOn(false), _actor(actor) {}

void CarProcess::VInit()
{
    auto eventManager = Game::getInstance().getEventManager();
    eventManager->AddListener(EventType::KEYBOARD_EVENT ,
            [&](IEventDataSPtr eventData)
            {
                std::shared_ptr<KeyboardEvent> keyboardEventDataSPtr = std::dynamic_pointer_cast<KeyboardEvent, IEventData>(eventData);
                if(keyboardEventDataSPtr->key == GLFW_KEY_W && keyboardEventDataSPtr->action == GLFW_KEY_DOWN)
                    _accelerationOn = true;
                else if(keyboardEventDataSPtr->key == GLFW_KEY_W && keyboardEventDataSPtr->action == GLFW_KEY_DOWN)
                    _accelerationOn = false;

            });
}

void CarProcess::VUpdate(double deltaTime)
{


}

void CarProcess::VEnd() {

}
