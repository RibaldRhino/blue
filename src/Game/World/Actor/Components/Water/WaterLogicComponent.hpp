#pragma once

#include <memory>
#include <Game/World/Actor/Components/IComponent.hpp>
#include <Game/World/Actor/Actor.hpp>

#include <GL/glew.h>
#include "CL/cl.h"

const unsigned int mesh_width = 10;
const unsigned int mesh_height = 10;

namespace game {
    class WaterLogicComponent : public IComponent {
        private:
        ActorWPtr _actorWPtr;
        cl_kernel ckKernel;
        cl_command_queue cqCommandQueue;

        GLuint vbo;
        cl_mem vbo_cl;
        size_t szGlobalWorkSize[2];

        float offset;

        public:
        WaterLogicComponent(game::ActorWPtr actorWPtr, GLuint vb);

        virtual ComponentType VGetComponentType() { return ComponentType::LOGIC_COMPONENT; }
        void Update(double deltaTime);

    };
}