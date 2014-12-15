#pragma once

#include <boost/noncopyable.hpp>
#include "OpenGLSystem.hpp"

#ifdef _WIN32   // Windows version
#include <windows.h>
#endif
#include <CL/cl.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

namespace gamesystem
{

class OpenCLSystem : boost::noncopyable
{

private:
    OpenCLSystem();

private:
    cl_context _context;
    cl_device_id _deviceId;
    cl_command_queue _commandQueue;
    bool TryCreateContext();
    bool TryGetPlatforms(std::vector<cl_platform_id> &platforms);
public:
    static OpenCLSystem& getInstance() {
        static OpenCLSystem instance;
        return instance;
    }
    cl_context getContext() const {
        return _context;
    }
    cl_device_id getDeviceId() const {
        return _deviceId;
    }
    cl_command_queue getCommandQueue() const {
        return _commandQueue;
    }
    bool TryLoadKernel(std::string filePath, std::string kernelName, cl_kernel& kernel);

};

}