#include <Event/Game/Window/OnWindowResized.hpp>
#include <Event/EventManager.hpp>
#include <Event/Game/Window/OnCursorPositionChanged.hpp>
#include <Game/World/Actor/Components/TransformComponent.hpp>
#include "CameraLogicComponent.hpp"
#include "CameraModelComponent.hpp"
#include <glm/glm.hpp>
#include <log.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
game::CameraLogicComponent::CameraLogicComponent(game::ActorWPtr actorWPtr) {

    _cameraSpeed = 0.5;
    _moveForward = false;
    _moveBack = false;
    _moveRight = false;
    _moveLeft = false;
    _cameraLocked = true;
    _actorWPtr = actorWPtr;
    using std::placeholders::_1;
    event::EventManager& eventManager = event::EventManager::get();
    _connections[event::EventType::WINDOW_RESIZED] =
        eventManager.AddListener(event::EventType::WINDOW_RESIZED, std::bind(&game::CameraLogicComponent::OnWindowResized, this, _1));

    _connections[event::EventType::KEYBOARD_UP_PRESSED] =
        eventManager.AddListener(event::EventType::KEYBOARD_UP_PRESSED , [this](event::IEventDataSPtr& eventData) { _moveForward = true; });
    _connections[event::EventType::KEYBOARD_UP_RELEASED] =
            eventManager.AddListener(event::EventType::KEYBOARD_UP_RELEASED , [this](event::IEventDataSPtr& eventData) {_moveForward = false;});
    _connections[event::EventType::KEYBOARD_DOWN_PRESSED] =
            eventManager.AddListener(event::EventType::KEYBOARD_DOWN_PRESSED , [this](event::IEventDataSPtr& eventData) {_moveBack = true;});
    _connections[event::EventType::KEYBOARD_DOWN_RELEASED] =
            eventManager.AddListener(event::EventType::KEYBOARD_DOWN_RELEASED , [this](event::IEventDataSPtr& eventData) {_moveBack = false;});
    _connections[event::EventType::KEYBOARD_RIGHT_PRESSED] =
            eventManager.AddListener(event::EventType::KEYBOARD_RIGHT_PRESSED , [this](event::IEventDataSPtr& eventData) {_moveRight = true;});
    _connections[event::EventType::KEYBOARD_RIGHT_RELEASED] =
            eventManager.AddListener(event::EventType::KEYBOARD_RIGHT_RELEASED , [this](event::IEventDataSPtr& eventData) {_moveRight = false;});
    _connections[event::EventType::KEYBOARD_LEFT_PRESSED] =
            eventManager.AddListener(event::EventType::KEYBOARD_LEFT_PRESSED , [this](event::IEventDataSPtr& eventData) {_moveLeft = true;});
    _connections[event::EventType::KEYBOARD_LEFT_RELEASED] =
            eventManager.AddListener(event::EventType::KEYBOARD_LEFT_RELEASED , [this](event::IEventDataSPtr& eventData) {_moveLeft = false;});

    _connections[event::EventType::MOUSE_LMB_PRESSED] =
            eventManager.AddListener(event::EventType::MOUSE_LMB_PRESSED, [this](event::IEventDataSPtr& eventData) {_cameraLocked = false;});
    _connections[event::EventType::MOUSE_LMB_RELEASED] =
            eventManager.AddListener(event::EventType::MOUSE_LMB_RELEASED, [this](event::IEventDataSPtr& eventData) {_cameraLocked = true;});

    _connections[event::EventType::MOUSE_CURSOR_POS] =
            eventManager.AddListener(event::EventType::MOUSE_CURSOR_POS, std::bind(&game::CameraLogicComponent::OnCursorMoved, this, _1));

}

void game::CameraLogicComponent::OnWindowResized(event::IEventDataSPtr &eventData) {
    //TODO: change to event
    auto data = std::dynamic_pointer_cast<event::OnWindowResized>(eventData);
    float aspectRatio = (float)data->getWidth()/data->getHeight();
    auto actor = _actorWPtr.lock();
    auto cameraComponent = std::dynamic_pointer_cast<CameraModelComponent>(actor->getComponent(game::ComponentType::MODEL_COMPONENT));
    cameraComponent->UpdatePerspective(aspectRatio);
}



void game::CameraLogicComponent::OnCursorMoved(event::IEventDataSPtr &eventData) {
    if(!_cameraLocked) {
        //TODO: change to event
        auto actor = _actorWPtr.lock();
        auto data = std::dynamic_pointer_cast<event::OnCursorPositionChanged>(eventData);
        auto cameraComponent = std::dynamic_pointer_cast<CameraModelComponent>(actor->getComponent(game::ComponentType::MODEL_COMPONENT));
        auto transformComponent = std::dynamic_pointer_cast<TransformComponent>(actor->getComponent(game::ComponentType::TRANSFORM_COMPONENT));

        transformComponent->RotateBy(-data->getVec().x, glm::vec3(0, 1, 0));
        transformComponent->RotateBy(-data->getVec().y, transformComponent->getRight());
        cameraComponent->UpdateView();
    }
}

void game::CameraLogicComponent::Update(double deltaTime) {
    auto actor = _actorWPtr.lock();
    auto transformComponent = std::dynamic_pointer_cast<TransformComponent>(actor->getComponent(game::ComponentType::TRANSFORM_COMPONENT));
    auto cameraComponent = std::dynamic_pointer_cast<CameraModelComponent>(actor->getComponent(game::ComponentType::MODEL_COMPONENT));
    float factor = 2;
    if(_moveForward)
        transformComponent->MoveBy(factor*(float)deltaTime*transformComponent->getForward());
    if(_moveBack)
        transformComponent->MoveBy(factor*-(float)deltaTime*transformComponent->getForward());
    if(_moveRight)
        transformComponent->MoveBy(factor*-(float)deltaTime*transformComponent->getRight());
    if(_moveLeft)
        transformComponent->MoveBy(factor*(float)deltaTime*transformComponent->getRight());

    cameraComponent->UpdateView();

}
