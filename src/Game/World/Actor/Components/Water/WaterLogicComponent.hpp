#pragma once

#include <memory>
#include <Game/World/Actor/Components/IComponent.hpp>
#include <Game/World/Actor/Actor.hpp>

#include <GL/glew.h>
#include <bits/stl_vector.h>
#include <clppSort.h>
#include "CL/cl.h"


namespace game {
    class WaterLogicComponent : public IComponent {
        private:
        ActorWPtr _actorWPtr;
        cl_kernel _hash_particles_kernel;
        clppContext cont;
        clppSort* sort;
        cl_kernel _sort_post_pass_kernel;
        cl_kernel _index_kernel;
        cl_kernel _index_post_pass_kernel;
        cl_kernel _find_voxel_neighbours_kernel;
        cl_kernel _neighbour_map_kernel;
        cl_kernel _compute_density_pressure_kernel;
        cl_kernel _compute_acceleration_kernel;
        cl_kernel _integrate_kernel;

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
        std::vector<cl_int> _grid_voxel_indices;
        cl_mem _grid_voxel_index_cl;
        std::vector<cl_int> _neighbour_map;
        cl_mem _neighbour_map_cl;
        std::vector<cl_int2> _voxel_positions;
        cl_mem _voxel_positions_cl;
        std::vector<cl_int> _voxel_neighbours;
        cl_mem _voxel_neighbours_cl;
        std::vector<cl_float2> _density_pressure;
        cl_mem _density_pressure_cl;

        int neighbour_count{55};

        unsigned int _particle_count;

    public:
        WaterLogicComponent(game::ActorWPtr actorWPtr);
        ~WaterLogicComponent();
        virtual ComponentType VGetComponentType() { return ComponentType::LOGIC_COMPONENT; }
        void Update(double deltaTime);
    };
}