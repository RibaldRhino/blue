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


game::WaterLogicComponent::WaterLogicComponent(game::ActorWPtr actorWPtr) {
    auto &&model = std::dynamic_pointer_cast<WaterModelComponent>(actorWPtr.lock()->getComponent(ComponentType::MODEL_COMPONENT));
    auto &openCLSystem = gamesystem::OpenCLSystem::getInstance();
    if (!openCLSystem.TryLoadKernel("Kernel/test.cl", "calculate_density", _calculate_density_kernel)) {
        LOG(ERROR) << "Failed to load calculate_density kernel";
        event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
    }

    auto context = openCLSystem.getContext();
    cl_int errNum = 0;
    _position_buffer_cl = clCreateFromGLBuffer(context, CL_MEM_WRITE_ONLY, model->position_vbo, &errNum);
    if (errNum != CL_SUCCESS) {
        LOG(ERROR) << "clCreateFromGLBuffer error " << errNum;
    }
    _density_buffer.resize(model->resolution());
    _density_buffer_cl = clCreateBuffer(context, CL_MEM_READ_WRITE, _density_buffer.size() * sizeof(float), &_density_buffer[0], &errNum);
    _pressure_buffer.resize(model->resolution());
    _pressure_buffer_cl = clCreateBuffer(context, CL_MEM_READ_WRITE, _pressure_buffer.size() * sizeof(float), &_pressure_buffer[0], &errNum);
    _velocity_buffer.resize(model->resolution() * 3);
    _velocity_buffer_cl = clCreateBuffer(context, CL_MEM_READ_WRITE, _velocity_buffer.size() * sizeof(float), &_velocity_buffer[0], &errNum);
    _acceleration_buffer.resize(model->resolution() * 3);
    _acceleration_buffer_cl = clCreateBuffer(context, CL_MEM_READ_WRITE, _acceleration_buffer.size() * sizeof(float), &_acceleration_buffer[0], &errNum);

    int a = 10;
    int mass = model->particle_mass();
    errNum = clSetKernelArg(_calculate_density_kernel, 0, sizeof(cl_mem), &_position_buffer_cl);
    errNum = clSetKernelArg(_calculate_density_kernel, 1, sizeof(cl_mem), &_density_buffer_cl);
    errNum = clSetKernelArg(_calculate_density_kernel, 2, sizeof(cl_mem), &_pressure_buffer_cl);
    errNum = clSetKernelArg(_calculate_density_kernel, 3, sizeof(cl_mem), &_velocity_buffer_cl);
    errNum = clSetKernelArg(_calculate_density_kernel, 4, sizeof(cl_mem), &_acceleration_buffer_cl);
    errNum = clSetKernelArg(_calculate_density_kernel, 5, sizeof(float), &mass);
    errNum = clSetKernelArg(_calculate_density_kernel, 6, sizeof(unsigned int), &a);
    errNum = clSetKernelArg(_calculate_density_kernel, 7, sizeof(unsigned int), &a);

    offset = 0.0f;
}

void game::WaterLogicComponent::Update(double deltaTime)
{
    cl_int errNum = CL_SUCCESS;

    glFinish();
    cl_command_queue commandQueue = gamesystem::OpenCLSystem::getInstance().getCommandQueue();
    errNum = clEnqueueAcquireGLObjects(commandQueue, 1, &_position_buffer_cl, 0,0,0);

    // Set arg 3 and execute the kernel
    float f = deltaTime;
    errNum = clSetKernelArg(_calculate_density_kernel, 8, sizeof(float), &f);
    size_t globalWorkSize = 100;
    errNum |= clEnqueueNDRangeKernel(commandQueue, _calculate_density_kernel, 1, NULL, &globalWorkSize, NULL, 0,0,0 );

    std::vector<float> positions;
    positions.resize(400);
    errNum |= clEnqueueReadBuffer(commandQueue, _position_buffer_cl, CL_TRUE, 0, 400 * sizeof(float), &positions[0], 0, 0, NULL);
    errNum |= clEnqueueReadBuffer(commandQueue, _density_buffer_cl, CL_TRUE, 0, 100 * sizeof(float), &_density_buffer[0], 0, 0, NULL);

    for(int i =0;i<positions.size();i+=4)
        std::cout<<positions[i]<<" "<<positions[i+1]<<" "<<positions[i+2]<<" "<<positions[i+3]<<std::endl;
    for(int i=0;i<_density_buffer.size();i++)
        std::cout<<_density_buffer[i]<<" ";
    std::cout<<std::endl;

    errNum = clEnqueueReleaseGLObjects(commandQueue, 1, &_position_buffer_cl, 0,0,0);
    clFinish(commandQueue);
};
