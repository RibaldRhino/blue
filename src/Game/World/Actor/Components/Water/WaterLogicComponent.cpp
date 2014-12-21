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
    
    auto context = openCLSystem.getContext();
    cl_int errNum = 0;

    _particle_count = model->particle_count();


    cl_float4 lbf{0,0,0,0};
    cl_float4 rtb{2,1,1,0};
    cl_float h{0.27};

    unsigned int voxelsX = (unsigned int) (fabs((float)((rtb.s[0] - lbf.s[0])/(2*h))) + 0.5);
    unsigned int voxelsY = (unsigned int) (fabs((float)((rtb.s[1] - lbf.s[1])/(2*h))) + 0.5);
    unsigned int voxelsZ = (unsigned int) (fabs((float)((rtb.s[2] - lbf.s[2])/(2*h))) + 0.5);


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
    _grid_voxel_indices.resize(voxelsX*voxelsY*voxelsZ + 1);
    _grid_voxel_indices.back() = _particle_count;
    _grid_voxel_index_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * _grid_voxel_indices.size(), &_grid_voxel_indices[0], &errNum);
    _neighbour_map.resize(neighbour_count*_particle_count);
    _neighbour_map_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * _neighbour_map.size(), &_neighbour_map[0], &errNum);
    _voxel_positions.resize(_particle_count);
    _voxel_positions_cl = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_int2) * _voxel_positions.size(), &_voxel_positions[0], &errNum);

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

    errNum = clSetKernelArg(_neighbour_map_kernel, 0, sizeof(cl_mem), &_grid_voxel_index_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 1, sizeof(cl_mem), &_voxel_positions_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 2, sizeof(cl_mem), &_sorted_position_cl);
    errNum = clSetKernelArg(_neighbour_map_kernel, 3, sizeof(cl_mem), &_neighbour_map);
    errNum = clSetKernelArg(_neighbour_map_kernel, 4, sizeof(cl_int), &neighbour_count);
    errNum = clSetKernelArg(_neighbour_map_kernel, 5, sizeof(cl_float4), &lbf);
    errNum = clSetKernelArg(_neighbour_map_kernel, 6, sizeof(cl_float4), &rtb);
    errNum = clSetKernelArg(_neighbour_map_kernel, 7, sizeof(cl_float), &h);
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
    size_t neighbourMapSize = _neighbour_map.size();
    size_t neighboursToFind = neighbour_count;
    errNum = clEnqueueNDRangeKernel(commandQueue, _hash_particles_kernel, 1, NULL, &particlesWorkSize, NULL, 0,0,0 );
    clFinish(commandQueue);
    sort->sort();
    sort->waitCompletion();
    errNum = clEnqueueNDRangeKernel(commandQueue, _sort_post_pass_kernel, 1, NULL, &particlesWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    errNum = clEnqueueReadBuffer(commandQueue, _sorted_position_cl, CL_TRUE, 0, _sorted_positions.size() * sizeof(cl_float4), &_sorted_positions[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _sorted_velocity_cl, CL_TRUE, 0, _sorted_velocities.size() * sizeof(cl_float4), &_sorted_velocities[0], 0, 0, NULL);
    for(int i=0;i<_sorted_positions.size();i++)
    {
        cout<<i<<" "<<_sorted_positions[i].s[0]<<" "<<_sorted_positions[i].s[1]<<" "<<_sorted_positions[i].s[2]<<" "<<_sorted_positions[i].s[3]<<std::endl;
    }
    cout<<"====================="<<endl;
    errNum = clEnqueueNDRangeKernel(commandQueue, _index_kernel, 1, NULL, &voxelWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    errNum = clEnqueueReadBuffer(commandQueue, _grid_voxel_index_cl, CL_TRUE, 0, _grid_voxel_indices.size() * sizeof(cl_int), &_grid_voxel_indices[0], 0, 0, NULL);
    for(int i=0;i<_grid_voxel_indices.size();i++)
    {
        cout<<i<<" "<<_grid_voxel_indices[i]<<std::endl;
    }
    cout<<"====================="<<endl;
    errNum = clEnqueueNDRangeKernel(commandQueue, _index_post_pass_kernel, 1, NULL, &voxelWorkSize, NULL, 0, 0, 0);
    clFinish(commandQueue);
    errNum = clEnqueueReadBuffer(commandQueue, _grid_voxel_index_cl, CL_TRUE, 0, _grid_voxel_indices.size() * sizeof(cl_int), &_grid_voxel_indices[0], 0, 0, NULL);
    for(int i=0;i<_grid_voxel_indices.size();i++)
    {
        cout<<i<<" "<<_grid_voxel_indices[i]<<std::endl;
    }
    cout<<"====================="<<endl;
    errNum = clEnqueueNDRangeKernel(commandQueue, _neighbour_map_kernel, 1, NULL, &neighbourMapSize, &neighboursToFind, 0, 0, 0);
    clFinish(commandQueue);



    errNum = clEnqueueReleaseGLObjects(commandQueue, 1, &_position_cl, 0,0,0);
    clFinish(commandQueue);
};

void game::WaterLogicComponent::LogAsynch() {
    cl_int errNum = 0;
    auto& clSystem = gamesystem::OpenCLSystem::getInstance();
    cl_command_queue commandQueue = clSystem.getCommandQueue();
    auto& config = gamesystem::ConfigSystem::getInstance();

    if(config["LOG_POSITIONS"] && config["LOG_POSITIONS"].get() == "T") {
        errNum = clEnqueueReadBuffer(commandQueue, _position_cl, CL_TRUE, 0, _positions.size() * sizeof(cl_float4), &_positions[0], 0, 0, NULL);
    }
    errNum = clEnqueueReadBuffer(commandQueue, _velocity_cl, CL_TRUE, 0, _velocities.size() * sizeof(cl_float4), &_velocities[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _sorted_position_cl, CL_TRUE, 0, _sorted_positions.size() * sizeof(cl_float4), &_sorted_positions[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _sorted_velocity_cl, CL_TRUE, 0, _sorted_velocities.size() * sizeof(cl_float4), &_sorted_velocities[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _acceleration_cl, CL_TRUE, 0, _accelerations.size() * sizeof(cl_float4), &_accelerations[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _grid_voxel_index_cl, CL_TRUE, 0, _grid_voxel_indices.size() * sizeof(cl_uint), &_grid_voxel_indices[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _neighbour_map_cl, CL_TRUE, 0, _grid_voxel_indices.size() * sizeof(cl_uint), &_neighbour_map[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _voxel_positions_cl, CL_TRUE, 0, _voxel_positions.size() * sizeof(cl_float2), &_voxel_positions[0], 0, 0, NULL);
}
