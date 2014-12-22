#include <GL/glew.h>
#include <log.hpp>
#include "WaterModelComponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <Game/World/Actor/Components/TransformComponent.hpp>
#include "WaterRendererComponent.hpp"

void game::WaterRendererComponent::Render()
{
    std::shared_ptr<Actor> actor = _actorWPtr.lock();
    auto transformComponent = std::dynamic_pointer_cast<TransformComponent>(actor->getComponent(game::ComponentType::TRANSFORM_COMPONENT));
    glUseProgram (_program);
    int modelToWorldLocation = glGetUniformLocation (_program, "modelToWorld");
    glUniformMatrix4fv (modelToWorldLocation, 1, GL_FALSE, glm::value_ptr(transformComponent->getTransform()));
    auto model = std::dynamic_pointer_cast<WaterModelComponent>(actor->getComponent(ComponentType::MODEL_COMPONENT));
    glBindVertexArray(model->vao);
    glDrawArrays(GL_POINTS, 0, model->points.size()/3);
}

game::WaterRendererComponent::WaterRendererComponent(game::ActorWPtr actorWPtr, GLuint program) {
    _actorWPtr = actorWPtr;
    _program = program;
}