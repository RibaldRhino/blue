#pragma once

#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <Event/IEventData.hpp>
#include <Game/World/Actor/Components/IComponent.hpp>
#include <Game/World/Actor/Actor.hpp>
#include <boost/signals2/connection.hpp>

namespace game {
    class CameraLogicComponent : public IComponent {
    private:
        ActorWPtr _actorWPtr;
        std::map<event::EventType, boost::signals2::connection> _connections;
        bool _moveForward;
        bool _moveBack;
        bool _moveRight;
        bool _moveLeft;
        bool _cameraLocked;
        float _cameraSpeed;
    public:
        CameraLogicComponent(ActorWPtr actorWPtr);
        void OnWindowResized(event::IEventDataSPtr &eventData);
        void OnCursorMoved(event::IEventDataSPtr &eventData);

        virtual ComponentType VGetComponentType() { return ComponentType::LOGIC_COMPONENT; }
        void Update(double deltaTime);

    };
}