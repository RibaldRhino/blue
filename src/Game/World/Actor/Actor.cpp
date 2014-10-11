#include "Actor.hpp"

std::shared_ptr<game::IComponent> game::Actor::getComponent(game::ComponentType type) {
    return _components[type];
}

bool game::Actor::hasComponent(game::ComponentType type) {
    return _components.find(type) != _components.end();
}

void game::Actor::AddComponent(std::shared_ptr<game::IComponent> component) {
    _components[component->VGetComponentType()] = component;
}
