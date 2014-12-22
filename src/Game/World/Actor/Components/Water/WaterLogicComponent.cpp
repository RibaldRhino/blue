#include <Game/World/Actor/Actor.hpp>
#include "WaterLogicComponent.hpp"
#include "WaterModelComponent.hpp"
#include <log.hpp>
#include <stdio.h>
#include <Event/EventManager.hpp>
#include <Event/Game/Window/OnWindowClose.hpp>

#ifdef _WIN32   // Windows version
#include <windows.h>
#endif

#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <System/OpenCLSystem.hpp>
#include <System/ConfigSystem.hpp>
#include <CL/cl_platform.h>
#include "clpp.h"

game::WaterLogicComponent::WaterLogicComponent(game::ActorWPtr actorWPtr) {
    auto &&model = std::dynamic_pointer_cast<WaterModelComponent>(actorWPtr.lock()->getComponent(ComponentType::MODEL_COMPONENT));
    auto &openCLSystem = gamesystem::OpenCLSystem::getInstance();
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "hash_particles", _hash_particles_kernel)) {
        LOG(ERROR) << "Failed to load hash_particles kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "sort_post_pass", _sort_post_pass_kernel)) {
        LOG(ERROR) << "Failed to load sort_post_pass kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "indexx", _index_kernel)) {
        LOG(ERROR) << "Failed to load indexx kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "index_post_pass", _index_post_pass_kernel)) {
        LOG(ERROR) << "Failed to load index_post_pass kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "neighbour_map", _neighbour_map_kernel)) {
        LOG(ERROR) << "Failed to load neighbour_map kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "find_voxel_neighbours", _find_voxel_neighbours_kernel)) {
        LOG(ERROR) << "Failed to load find_voxel_neighbours kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }

    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "compute_density_pressure", _compute_density_pressure_kernel)) {
        LOG(ERROR) << "Failed to load compute_density_pressure kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "compute_acceleration", _compute_acceleration_kernel)) {
        LOG(ERROR) << "Failed to load compute_acceleration kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "integrate", _integrate_kernel)) {
        LOG(ERROR) << "Failed to load integrate kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }

    auto context = openCLSystem.getContext();
    cl_int errNum = 0;

    _particle_count = model->particle_count();


    cl_float4 lbf{0,0,0,0};
    cl_float4 rtb{2,1,1,0};

    cl_float h{0.25};

    cl_float4 g{0,-9.81,0,0};

    cl_float m = model->particle_mass();
    cl_float mi{0.894};
    cl_float k{1};
    cl_float ro0 = 1000.0;

    unsigned int voxelsX = (unsigned int) (fabs((float)((rtb.s[0] - lbf.s[0])/(2*h))) + 0.5);
    unsigned int voxelsY = (unsigned int) (fabs((float)((rtb.s[1] - lbf.s[1])/(2*h))) + 0.5);
    unsigned int voxelsZ = (unsigned int) (fabs((float)((rtb.s[2] - lbf.s[2])/(2*h))) + 0.5);
    unsigned voxelCount = voxelsX * voxelsY * voxelsZ;

    clppProgram::setBasePath("Kernel");
    cont.clContext = openCLSystem.getContext();
    cont.clDevice = openCLSystem.getDevice();
    cont.clPlatform = openCLSystem.getPlatform();
    cont.clQueue = openCLSystem.getCommandQueue();

    _positions.resize(_particle_count);
    _position_cl = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, model->position_vbo, &errNum);
    _velocities.resize(_particle_count);
    _velocity_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * _velocities.size(), &_velocities[0], &errNum);
    _accelerations.resize(_particle_count);
    _acceleration_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * _accelerations.size(), &_accelerations[0], &errNum);
    _sorted_positions.resize(_particle_count);
    _sorted_position_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * _sorted_positions.size(), &_sorted_positions[0], &errNum);
    _sorted_velocities.resize(_particle_count);
    _sorted_velocity_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float4) * _sorted_velocities.size(), &_sorted_velocities[0], &errNum);
    _grid_voxel_indices.resize(voxelCount + 1);
    _grid_voxel_indices.back() = _particle_count;
    _grid_voxel_index_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * _grid_voxel_indices.size(), &_grid_voxel_indices[0], &errNum);
    _neighbour_map.resize(neighbour_count*_particle_count);
    _neighbour_map_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * _neighbour_map.size(), &_neighbour_map[0], &errNum);
    _voxel_positions.resize(_particle_count);
    _voxel_positions_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int2) * _voxel_positions.size(), &_voxel_positions[0], &errNum);
    _voxel_neighbours.resize(64*voxelCount);
    _voxel_neighbours_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * _voxel_neighbours.size(), &_voxel_neighbours[0], &errNum);
    _density_pressure.resize(_particle_count);
    _density_pressure_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float2) * _density_pressure.size(), &_density_pressure[0], &errNum);

    sort = clpp::createBestSortKV(&cont, _voxel_positions.size(), false);
    sort->pushCLDatas(_voxel_positions_cl, _voxel_positions.size());

    errNum = clSetKernelArg(_hash_particles_kernel, 0, sizeof(cl_mem), &_position_cl);
    errNum = clSetKernelArg(_hash_particles_kernel, 1, sizeof(cl_mem), &_voxel_positions_cl);
    errNum = clSetKernelArg(_hash_particles_kernel, 2, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(_hash_particles_kernel, 3, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(_hash_particles_kernel, 4, sizeof(cl_float), &h);

    errNum = clSetKernelArg(_sort_post_pass_kernel, 0, sizeof(cl_mem), &_position_cl);
    errNum = clSetKernelArg(_sort_post_pass_kernel, 1, sizeof(cl_mem), &_velocity_cl);
    errNum = clSetKernelArg(_sort_post_pass_kernel, 2, sizeof(cl_mem), &_sorted_position_cl);
    errNum = clSetKernelArg(_sort_post_pass_kernel, 3, sizeof(cl_mem), &_sorted_velocity_cl);
    errNum = clSetKernelArg(_sort_post_pass_kernel, 4, sizeof(cl_mem), &_voxel_positions_cl);

    errNum = clSetKernelArg(_index_kernel, 0, sizeof(cl_mem), &_grid_voxel_index_cl);
    errNum = clSetKernelArg(_index_kernel, 1, sizeof(cl_mem), &_voxel_positions_cl);
    errNum = clSetKernelArg(_index_kernel, 2, sizeof(cl_uint), &_particle_count);

    errNum = clSetKernelArg(_index_post_pass_kernel, 0, sizeof(cl_mem), &_grid_voxel_index_cl);

    /*
     __global int* neighbourMap,
    __global int2* voxelParticle,
    __global int* gridVoxelIndex,
    __global float4* sortedPositions,
    __global int* randomInts, //should be particle_count elements
    __global int* voxelNeighbourMap,
    int neighboursToFind,
    float4 lbf,
    float4 rtb,
    float h
     */
    errNum = clSetKernelArg(_neighbour_map_kernel, 0, sizeof(cl_mem), &_neighbour_map_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 1, sizeof(cl_mem), &_voxel_positions_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 2, sizeof(cl_mem), &_grid_voxel_index_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 3, sizeof(cl_mem), &_sorted_position_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 4, sizeof(cl_mem), &_voxel_neighbours_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 5, sizeof(cl_int), &neighbour_count);
    errNum = clSetKernelArg(_neighbour_map_kernel, 6, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(_neighbour_map_kernel, 7, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(_neighbour_map_kernel, 8, sizeof(cl_float), &h);

    errNum = clSetKernelArg(_find_voxel_neighbours_kernel, 0, sizeof(cl_mem), &_voxel_neighbours_cl);
    errNum = clSetKernelArg(_find_voxel_neighbours_kernel, 1, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(_find_voxel_neighbours_kernel, 2, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(_find_voxel_neighbours_kernel, 3, sizeof(cl_float), &h);

    auto commandQueue = openCLSystem.getCommandQueue();
    size_t voxelWorkSize = _grid_voxel_indices.size() - 1;
    errNum = clEnqueueNDRangeKernel(commandQueue, _find_voxel_neighbours_kernel, 1, NULL, &voxelWorkSize, NULL, 0,0,0 );
    clFinish(commandQueue);
/*    errNum = clEnqueueReadBuffer(commandQueue, _voxel_neighbours_cl, CL_TRUE, 0, _voxel_neighbours.size() * sizeof(cl_int), &_voxel_neighbours[0], 0, 0, NULL);
    for(int i=0;i<_voxel_neighbours.size()/64;i++) {
        cout<<i<<endl;
        for (int j = 0; j < 8; j++) {
            cout<<_voxel_neighbours[i*64+j*8] << " " << _voxel_neighbours[i*64+j*8+1] << " " << _voxel_neighbours[i*64+j*8+2] << " " << _voxel_neighbours[i*64+j*8+3] << " "
                    <<_voxel_neighbours[i*64+j*8+4] << " " << _voxel_neighbours[i*64+j*8+5] << " " << _voxel_neighbours[i*64+j*8+6] << " " << _voxel_neighbours[i*64+j*8+7] << std::endl;
        }
    }*/

    errNum = clSetKernelArg(_compute_density_pressure_kernel, 0, sizeof(cl_mem), &_sorted_position_cl);
    errNum = clSetKernelArg(_compute_density_pressure_kernel, 1, sizeof(cl_mem), &_neighbour_map_cl);
    errNum = clSetKernelArg(_compute_density_pressure_kernel, 2, sizeof(cl_mem), &_density_pressure_cl);
    errNum = clSetKernelArg(_compute_density_pressure_kernel, 3, sizeof(cl_int), &neighbour_count);
    errNum = clSetKernelArg(_compute_density_pressure_kernel, 4, sizeof(cl_float), &m);
    errNum = clSetKernelArg(_compute_density_pressure_kernel, 5, sizeof(cl_float), &h);
    errNum = clSetKernelArg(_compute_density_pressure_kernel, 6, sizeof(cl_float), &k);
    errNum = clSetKernelArg(_compute_density_pressure_kernel, 7, sizeof(cl_float), &ro0);

    errNum = clSetKernelArg(_compute_acceleration_kernel, 0, sizeof(cl_mem), &_sorted_position_cl);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 1, sizeof(cl_mem), &_sorted_velocity_cl);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 2, sizeof(cl_mem), &_acceleration_cl);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 3, sizeof(cl_mem), &_density_pressure_cl);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 4, sizeof(cl_mem), &_neighbour_map_cl);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 5, sizeof(cl_float4), &g);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 6, sizeof(cl_float), &m);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 7, sizeof(cl_float), &h);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 8, sizeof(cl_float), &mi);
    errNum = clSetKernelArg(_compute_acceleration_kernel, 9, sizeof(cl_int), &neighbour_count);

    errNum = clSetKernelArg(_integrate_kernel, 0, sizeof(cl_mem), &_position_cl);
    errNum = clSetKernelArg(_integrate_kernel, 1, sizeof(cl_mem), &_velocity_cl);
    errNum = clSetKernelArg(_integrate_kernel, 2, sizeof(cl_mem), &_acceleration_cl);
    errNum = clSetKernelArg(_integrate_kernel, 3, sizeof(cl_mem), &_voxel_positions_cl);
    errNum = clSetKernelArg(_integrate_kernel, 4, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(_integrate_kernel, 5, sizeof(cl_float4), &rtb);
}

void game::WaterLogicComponent::Update(double deltaTime)
{
    glFinish();

    cl_int errNum = CL_SUCCESS;
    auto& clSystem = gamesystem::OpenCLSystem::getInstance();
    cl_command_queue commandQueue = clSystem.getCommandQueue();
    errNum = clEnqueueAcquireGLObjects(commandQueue, 1, &_position_cl, 0,0,0);

    //hash particles
    size_t particlesWorkSize = _particle_count;
    size_t voxelWorkSize = _grid_voxel_indices.size() - 1;



    errNum = clEnqueueNDRangeKernel(commandQueue, _hash_particles_kernel, 1, NULL, &particlesWorkSize, NULL, 0,0,0 );
    clFinish(commandQueue);
    sort->sort();
    sort->waitCompletion();
    errNum = clEnqueueNDRangeKernel(commandQueue, _sort_post_pass_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    //errNum = clEnqueueReadBuffer(commandQueue, _sorted_position_cl, CL_TRUE, 0, _sorted_positions.size() * sizeof(cl_float4), &_sorted_positions[0], 0, 0, NULL);
    //errNum = clEnqueueReadBuffer(commandQueue, _sorted_velocity_cl, CL_TRUE, 0, _sorted_velocities.size() * sizeof(cl_float4), &_sorted_velocities[0], 0, 0, NULL);
    //for(int i=0;i<_sorted_positions.size();i++)
    //{
    //    cout<<i<<" "<<_sorted_positions[i].s[0]<<" "<<_sorted_positions[i].s[1]<<" "<<_sorted_positions[i].s[2]<<" "<<_sorted_positions[i].s[3]<<std::endl;
    //}
    //cout<<"====================="<<endl;
    errNum = clEnqueueNDRangeKernel(commandQueue, _index_kernel, 1, NULL, &voxelWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    //errNum = clEnqueueReadBuffer(commandQueue, _grid_voxel_index_cl, CL_TRUE, 0, _grid_voxel_indices.size() * sizeof(cl_int), &_grid_voxel_indices[0], 0, 0, NULL);
    //for(int i=0;i<_grid_voxel_indices.size();i++)
    //{
    //    cout<<i<<" "<<_grid_voxel_indices[i]<<std::endl;
    //}
    //cout<<"====================="<<endl;
    errNum = clEnqueueNDRangeKernel(commandQueue, _index_post_pass_kernel, 1, NULL, &voxelWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    //errNum = clEnqueueReadBuffer(commandQueue, _grid_voxel_index_cl, CL_TRUE, 0, _grid_voxel_indices.size() * sizeof(cl_int), &_grid_voxel_indices[0], 0, 0, NULL);
    //for(int i=0;i<_grid_voxel_indices.size();i++)
    //{
    //    cout<<i<<" "<<_grid_voxel_indices[i]<<std::endl;
    //}
    //cout<<"====================="<<endl;
    errNum = clEnqueueNDRangeKernel(commandQueue, _neighbour_map_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

    errNum = clEnqueueReadBuffer(commandQueue, _neighbour_map_cl, CL_TRUE, 0, _neighbour_map.size() * sizeof(cl_int), &_neighbour_map[0], 0, 0, NULL);
/*    for(int i=0;i<_neighbour_map.size()/neighbour_count;i++) {
        cout<<i<<endl;
        for(int j=0;j<neighbour_count;j++) {
            cout<<_neighbour_map[i*32+j]<<" ";
        }
        cout<<endl;
    }
    cout<<"====================="<<endl;*/

    errNum = clEnqueueNDRangeKernel(commandQueue, _compute_density_pressure_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    /*errNum = clEnqueueReadBuffer(commandQueue, _density_pressure_cl, CL_TRUE, 0, _density_pressure.size() * sizeof(cl_float2), &_density_pressure[0], 0, 0, NULL);

    for(int i=0;i<_density_pressure.size();i++)
    {
        cout<<i<<" "<<_density_pressure[i].s[0]<<" "<<_density_pressure[i].s[1]<<endl;
    }*/


    errNum = clEnqueueNDRangeKernel(commandQueue, _compute_acceleration_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

/*    errNum = clEnqueueReadBuffer(commandQueue, _acceleration_cl, CL_TRUE, 0, _accelerations.size() * sizeof(cl_float4), &_accelerations[0], 0, 0, NULL);

    for(int i=0;i<_accelerations.size();i++)
    {
    cout<<i<<" "<<_accelerations[i].s[0]*deltaTime<<" "<<_accelerations[i].s[1]*deltaTime<<" "<<_accelerations[i].s[2]*deltaTime<<endl;
    }*/

    float castedDelta = deltaTime;
    errNum = clSetKernelArg(_integrate_kernel, 6, sizeof(cl_float), &castedDelta);
    errNum = clEnqueueNDRangeKernel(commandQueue, _integrate_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);

    errNum = clEnqueueReadBuffer(commandQueue, _position_cl, CL_TRUE, 0, _positions.size() * sizeof(cl_float4), &_positions[0], 0, 0, NULL);

    for(int i=0;i<_positions.size();i++)
    {
    cout<<i<<" "<<_positions[i].s[0]<<" "<<_positions[i].s[1]<<" "<<_positions[i].s[2]<<endl;
    }

    errNum = clEnqueueReleaseGLObjects(commandQueue, 1, &_position_cl, 0,0,0);
    clFinish(commandQueue);
};
