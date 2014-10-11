#pragma once

#include <memory>
#include <GL/glew.h>
#include <Game/World/Actor/Actor.hpp>
#include <Game/World/Actor/Components/IComponent.hpp>


namespace game {
    class BasicRendererComponent : public IComponent {
    private:
        ActorWPtr _actorWPtr;
        GLuint _program;
    public:
        BasicRendererComponent(ActorWPtr actorWPtr, GLuint program);
        void Render();

        virtual ComponentType VGetComponentType() { return ComponentType::RENDER_COMPONENT; }
    };
}