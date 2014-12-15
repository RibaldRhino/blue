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
        cl_kernel _calculate_density_kernel;

        cl_mem _position_buffer_cl;
        cl_mem _pressure_buffer_cl;
        std::vector<float> _pressure_buffer;
        cl_mem _density_buffer_cl;
        std::vector<float> _density_buffer;
        cl_mem _velocity_buffer_cl;
        std::vector<float> _velocity_buffer;
        cl_mem _acceleration_buffer_cl;
        std::vector<float> _acceleration_buffer;

        float offset;

        public:
        WaterLogicComponent(game::ActorWPtr actorWPtr);

        virtual ComponentType VGetComponentType() { return ComponentType::LOGIC_COMPONENT; }
        void Update(double deltaTime);

    };
}