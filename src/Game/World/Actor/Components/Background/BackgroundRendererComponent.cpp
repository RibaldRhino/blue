#include <Game/World/Actor/Actor.hpp>
#include "BackgroundRendererComponent.hpp"

game::BackgroundRendererComponent::BackgroundRendererComponent(ActorWPtr actorWPtr, GLuint program) {

}

void game::BackgroundRendererComponent::Render() {
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
}
