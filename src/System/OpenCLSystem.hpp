#pragma once

#include <boost/noncopyable.hpp>
#include "OpenGLSystem.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif
#include <CL/cl.h>
#include <CL/cl_gl.h>

namespace gamesystem
{

class OpenCLSystem : boost::noncopyable
{

private:
    OpenCLSystem();

private:
    cl_context _context;
    cl_device_id _deviceId;
    cl_platform_id _platformId;
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

    bool TryGetDevices(std::vector < cl_device_id > &devices);

    bool TryGetDevices(cl_platform_id platform, std::vector <cl_device_id> &devices);
};

}