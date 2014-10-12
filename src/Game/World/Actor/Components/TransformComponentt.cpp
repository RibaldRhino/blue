#include <Game/World/Actor/Actor.hpp>
#include "TransformComponent.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "log.hpp"
game::TransformComponent::TransformComponent(ActorWPtr actorWPtr) {
    _actorWPtr = actorWPtr;
    _translation = glm::vec3(0);
    _rotation = glm::quat();
    _rotation = glm::normalize(_rotation);
}

const glm::mat4 game::TransformComponent::getTransform() const {
    return glm::translate(glm::mat4(1), _translation) * glm::toMat4(_rotation);
}

const glm::vec3 game::TransformComponent::getPosition() const {
    return _translation;
}

const glm::vec3 game::TransformComponent::getRight() const {
    return glm::normalize(glm::vec3(_rotation * glm::vec4(-1, 0, 0, 0)));
}

const glm::vec3 game::TransformComponent::getUp() const {
    return glm::normalize(glm::vec3(_rotation * glm::vec4(0, 1, 0, 0)));
}
const glm::vec3 game::TransformComponent::getForward() const {
    return glm::normalize(glm::vec3(_rotation * glm::vec4(0, 0, 1, 0)));
}

void game::TransformComponent::MoveBy(glm::vec3 vector) {
    _translation+= vector;
}

void game::TransformComponent::RotateBy(float angle, glm::vec3 axis) {
        _rotation = glm::normalize(glm::angleAxis(angle, axis)) * _rotation;
}

