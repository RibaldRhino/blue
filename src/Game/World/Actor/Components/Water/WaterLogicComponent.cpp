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
#include <CL/cl_platform.h>


game::WaterLogicComponent::WaterLogicComponent(game::ActorWPtr actorWPtr) {
    auto &&model = std::dynamic_pointer_cast<WaterModelComponent>(actorWPtr.lock()->getComponent(ComponentType::MODEL_COMPONENT));
    auto &openCLSystem = gamesystem::OpenCLSystem::getInstance();
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "hash_particles", _hash_particles_kernel)) {
        LOG(ERROR) << "Failed to load hash_particles kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "histogram", _histogram_kernel)) {
        LOG(ERROR) << "Failed to load histogram kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }
    
    auto context = openCLSystem.getContext();
    cl_int errNum = 0;

    _particle_count = model->particle_count();


    _positions.resize(_particle_count);
    _position_cl = clCreateFromGLBuffer(context, CL_MEM_READ_WRITE, model->position_vbo, &errNum);
    _voxel_positions.resize(_particle_count);
    _voxel_positions_cl = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int2)*_voxel_positions.size(), NULL, &errNum);
    _histogram.resize(1<< _radix_bits);
    _histogram_cl = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int)*_histogram.size(), NULL, &errNum);

    cl_float4 blf{0,0,0,0};
    cl_float4 trb{1,2,1,0};
    cl_float h = 0.037;

    errNum = clSetKernelArg(_hash_particles_kernel, 0, sizeof(cl_mem), &_position_cl);
    errNum = clSetKernelArg(_hash_particles_kernel, 1, sizeof(cl_mem), &_voxel_positions_cl);
    errNum = clSetKernelArg(_hash_particles_kernel, 2, sizeof(cl_float4), &blf);
    errNum = clSetKernelArg(_hash_particles_kernel, 3, sizeof(cl_float4), &trb);
    errNum = clSetKernelArg(_hash_particles_kernel, 4, sizeof(cl_float), &h);

    errNum = clSetKernelArg(_histogram_kernel, 0, sizeof(cl_mem), &_voxel_positions_cl);
    errNum = clSetKernelArg(_histogram_kernel, 1, sizeof(cl_mem), &_histogram_cl);
    errNum = clSetKernelArg(_histogram_kernel, 2, sizeof(cl_int), &_radix_bits);
}

void game::WaterLogicComponent::Update(double deltaTime)
{
    cl_int errNum = CL_SUCCESS;
    cl_command_queue commandQueue = gamesystem::OpenCLSystem::getInstance().getCommandQueue();
    glFinish();
    errNum = clEnqueueAcquireGLObjects(commandQueue, 1, &_position_cl, 0,0,0);

    //hash particles
    size_t hashParticlesWorkSize = _particle_count;
    errNum = clEnqueueNDRangeKernel(commandQueue, _hash_particles_kernel, 1, NULL, &hashParticlesWorkSize, NULL, 0,0,0 );
    errNum = clEnqueueReadBuffer(commandQueue, _position_cl, CL_TRUE, 0, _positions.size() * sizeof(cl_float4),
            &_positions[0], 0, 0, NULL);
    errNum = clEnqueueReadBuffer(commandQueue, _voxel_positions_cl, CL_TRUE, 0, _voxel_positions.size() * sizeof(cl_float2),
            &_voxel_positions[0], 0, 0, NULL);
    for(int i =0;i< _positions.size();i++)
        std::cout<< _positions[i].s[0]<<" "<< _positions[i].s[1]<<" "<< _positions[i].s[2]<<" "<< _positions[i].s[3]<<std::endl;
    for(int i=0;i<_voxel_positions.size();i++)
        std::cout<<_voxel_positions[i].s[0]<<" "<<_voxel_positions[i].s[1]<<std::endl;

    //histogram
    size_t histogramWorkSize = _particle_count;
    cl_int it = 0;
    errNum = clSetKernelArg(_histogram_kernel, 3, sizeof(cl_int), &it);
    errNum = clEnqueueNDRangeKernel(commandQueue, _histogram_kernel, 1, NULL, &histogramWorkSize, NULL, 0,0,0 );
    errNum = clEnqueueReadBuffer(commandQueue, _histogram_cl, CL_TRUE, 0, _histogram.size()*sizeof(cl_int),
            &_histogram[0], 0, 0, NULL);
    for(auto& hist : _histogram)
        std::cout<<hist<<std::endl;

    errNum = clEnqueueReleaseGLObjects(commandQueue, 1, &_position_cl, 0,0,0);
    clFinish(commandQueue);
};
