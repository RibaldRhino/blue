
#include "CameraComponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Game/World/Actor/Components/TransformComponent.hpp>
#include <log.hpp>

game::CameraComponent::CameraComponent(ActorWPtr actorWPtr) {
    _actorWPtr = actorWPtr;
    _index = 0;
}

void game::CameraComponent::Load(GLuint program) {
    auto uniformBlockIndex = glGetUniformBlockIndex(program, "Camera");
    glGenBuffers(1, &_cameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, _cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    UpdateView();
    UpdatePerspective(4.0f / 3.0f);

    glUniformBlockBinding(program, uniformBlockIndex, _index);
    glBindBufferRange(GL_UNIFORM_BUFFER, _index, _cameraUBO, 0, sizeof(glm::mat4) * 2);

}


void game::CameraComponent::UpdatePerspective(float aspectRatio) {
    cameraToPerspective = (glm::mat4)glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, _cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cameraToPerspective));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void game::CameraComponent::UpdateView() {
    auto actor = _actorWPtr.lock();
    auto transformComponent = std::dynamic_pointer_cast<TransformComponent>(actor->getComponent(game::ComponentType::TRANSFORM_COMPONENT));
    auto pos = transformComponent->getPosition();
    auto forward = transformComponent->getForward();
    auto up = transformComponent->getUp();
    worldToCamera = glm::lookAt(pos, pos + forward, up);
    glBindBuffer(GL_UNIFORM_BUFFER, _cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(worldToCamera));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
