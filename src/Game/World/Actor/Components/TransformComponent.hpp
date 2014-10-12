#pragma once

#include <glm/glm.hpp>
#include "IComponent.hpp"
#include <Game/World/Actor/Actor.hpp>
#include <glm/gtc/quaternion.hpp>

namespace game {

    class TransformComponent : public IComponent {
    private:
        ActorWPtr _actorWPtr;
        glm::vec3 _translation;
        glm::quat _rotation;

    public:

        TransformComponent(ActorWPtr actorWPtr);
        const glm::mat4 getTransform() const;
        const glm::vec3 getPosition() const;
        const glm::vec3 getUp() const;
        const glm::vec3 getRight() const;
        const glm::vec3 getForward() const;
        const glm::quat getRotation() const {return _rotation;}

        void SetRotation(glm::quat rotation) {_rotation = rotation;}
        void MoveBy(glm::vec3 vector);
        void RotateBy(float angle, glm::vec3 axis);

        virtual ComponentType VGetComponentType() { return ComponentType::TRANSFORM_COMPONENT; }

    };

    typedef std::shared_ptr<TransformComponent> TransformSPtr;
}