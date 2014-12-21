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
        cl_kernel _sort_post_pass_kernel;
        cl_kernel _index_kernel;
        cl_kernel _index_post_pass_kernel;

        std::vector<cl_float4> _positions;
        cl_mem _position_cl;
        std::vector<cl_float4> _velocities;
        cl_mem _velocity_cl;
        std::vector<cl_float4> _accelerations;
        cl_mem _acceleration_cl;
        std::vector<cl_float4> _sorted_positions;
        cl_mem _sorted_position_cl;
        std::vector<cl_float4> _sorted_velocities;
        cl_mem _sorted_velocity_cl;
        std::vector<cl_float4> _grid_voxel_indexes;
        cl_mem _grid_voxel_index_cl;
        std::vector<cl_float4> _neighbour_maps;
        cl_mem _neighbour_map_cl;
        std::vector<cl_int2> _voxel_positions;
        cl_mem _voxel_positions_cl;

        unsigned int _particle_count;

        public:
        WaterLogicComponent(game::ActorWPtr actorWPtr);

        virtual ComponentType VGetComponentType() { return ComponentType::LOGIC_COMPONENT; }
        void Update(double deltaTime);

        void LogAsynch();
    };
}