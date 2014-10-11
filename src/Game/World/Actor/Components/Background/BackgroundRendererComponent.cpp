#include <Game/World/Actor/Actor.hpp>
#include "BackgroundRendererComponent.hpp"

game::BackgroundRendererComponent::BackgroundRendererComponent(ActorWPtr actorWPtr, GLuint program) {

}

void game::BackgroundRendererComponent::Render() {
    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
}
