#pragma once

#include <memory>
#include <Game/World/Actor/Components/IComponent.hpp>
#include <Game/World/Actor/Actor.hpp>

#include <GL/glew.h>
#include <bits/stl_vector.h>
#include "CL/cl.h"


namespace game {
    class WaterLogicComponent : public IComponent {
        private:
        ActorWPtr _actorWPtr;
        cl_kernel _hash_particles_kernel;
        cl_kernel _histogram_kernel;

        std::vector<cl_float4> _positions;
        cl_mem _position_cl;
        std::vector<cl_int2> _voxel_positions;
        cl_mem _voxel_positions_cl;
        std::vector<cl_int> _histogram;
        cl_mem _histogram_cl;

        GLuint _particle_count;

        const GLuint _radix_bits = 8;

        public:
        WaterLogicComponent(game::ActorWPtr actorWPtr);

        virtual ComponentType VGetComponentType() { return ComponentType::LOGIC_COMPONENT; }
        void Update(double deltaTime);

    };
}