#include <GL/glew.h>
#include <log.hpp>
#include "BasicModelComponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <Game/World/Actor/Components/TransformComponent.hpp>
#include "BasicRendererComponent.hpp"

void game::BasicRendererComponent::Render()
{
    std::shared_ptr<Actor> actor = _actorWPtr.lock();
    auto transformComponent = std::dynamic_pointer_cast<TransformComponent>(actor->getComponent(game::ComponentType::TRANSFORM_COMPONENT));
    glUseProgram (_program);
    int modelToWorldLocation = glGetUniformLocation (_program, "modelToWorld");
    glUniformMatrix4fv (modelToWorldLocation, 1, GL_FALSE, glm::value_ptr(transformComponent->getTransform()));
    auto model = std::dynamic_pointer_cast<BasicModelComponent>(actor->getComponent(ComponentType::MODEL_COMPONENT));
    glBindVertexArray(model->vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, model->vertices.size()/3);

}

game::BasicRendererComponent::BasicRendererComponent(game::ActorWPtr actorWPtr, GLuint program) {
    _actorWPtr = actorWPtr;
    _program = program;
}
