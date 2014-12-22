#pragma once
#include <memory>

#include <Game/World/Actor/Components/IComponent.hpp>
#include <Game/World/Actor/Components/ComponentType.hpp>
#include <map>

namespace game
{

    class Actor {
    private:
        std::map<ComponentType, std::shared_ptr<IComponent>> _components;
    public:
        std::shared_ptr<IComponent> getComponent(ComponentType type);
        bool hasComponent(ComponentType type);
        void AddComponent(std::shared_ptr<IComponent> component);
        void ClearComponent(ComponentType type);
    };
    typedef std::unique_ptr<Actor> ActorUPtr;
    typedef std::shared_ptr<Actor> ActorSPtr;
    typedef std::weak_ptr<Actor> ActorWPtr;
}