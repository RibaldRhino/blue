#include <log.hpp>
#include "OpenCLSystem.hpp"
#include <algorithm>
#include <array>

namespace gamesystem {
    OpenCLSystem::OpenCLSystem() {
        TryCreateContext();
    }
    bool OpenCLSystem::TryGetPlatforms(std::vector<cl_platform_id> &platforms) {
        cl_uint numPlatforms;
        cl_int errNum;

        errNum = clGetPlatformIDs(0, NULL, &numPlatforms);
        if (errNum != CL_SUCCESS) {
            LOG(ERROR) << "Error " << errNum << " in clGetPlatformIDs Call";
            return false;
        }
        else {
            if (numPlatforms == 0) {
                LOG(ERROR) << "No OpenCL platform found";
                return false;
            }
            else {
                platforms.resize(numPlatforms);
                errNum = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
                if (errNum != CL_SUCCESS) {
                    LOG(ERROR) << "Error " << errNum << " in clGetPlatformIDs Call ";
                    return false;
                }
            }
        }
        return true;
    }

    bool OpenCLSystem::TryCreateContext() {
        std::vector<cl_platform_id> platforms;
        auto& glSystem = OpenGLSystem::getInstance();
        cl_int errNum;
        if(!TryGetPlatforms(platforms))
        {
            LOG(ERROR) << "Unable to get opencl platforms";
            return false;
        }
        else
        {
            for(auto& platform : platforms)
            {
                cl_context_properties props[] =
                {
                        CL_GL_CONTEXT_KHR,
                        (cl_context_properties) glSystem.getCurrentContext(),
                        #ifdef _WIN32   // Windows version
                        CL_WGL_HDC_KHR,
                        #else
                        CL_GLX_DISPLAY_KHR,
                        #endif
                        (cl_context_properties) glSystem.getCurrentDisplay(),
                        CL_CONTEXT_PLATFORM,
                        (cl_context_properties)platform, 0
                };

                size_t size;
                errNum = clGetGLContextInfoKHR(props, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id), &_deviceId, NULL);
                if (errNum != CL_SUCCESS) {
                    LOG(ERROR) << "Platform: "<<platform<<", Error:" << errNum << " in clGetGLContextInfoKHR Call";
                    continue;
                } else {
                    LOG(INFO) << "Platform: "<<platform<<", clGetGLContextInfoKHR succeeded";
                }
                _context = clCreateContext(props, 1, &_deviceId, NULL, NULL, &errNum);
                if (errNum != CL_SUCCESS) {
                    LOG(ERROR) << "Platform: "<<platform<<", Error:" << errNum << " clCreateContext Call";
                    continue;
                } else {
                    LOG(INFO) << "Platform: "<<platform<<", clCreateContext succeeded";
                    break;
                }
            }
        }
        if(errNum != CL_SUCCESS)
            return false;
        _commandQueue = clCreateCommandQueue(_context, _deviceId, 0, &errNum);
        if (errNum != CL_SUCCESS) {
            LOG(ERROR) << "Error:" << errNum << " clCreateCommandQueue Call";
            return false;
        } else {
            LOG(INFO) << "clCreateCommandQueue succeeded";
            return true;
        }
    }


    bool OpenCLSystem::TryLoadKernel(std::string filePath, std::string kernelName, cl_kernel& kernel)
    {
        cl_int errNum;

        size_t program_length;
        std::ifstream in(filePath, std::ios::in | std::ios::binary);
        std::ostringstream buffer;
        buffer << in.rdbuf();
        std::string kernelSource = buffer.str();
        LOG(INFO) << kernelSource;
        program_length = kernelSource.size();

        LOG(INFO) << "Program length: " << program_length;

        const char* kernelSourceC = &kernelSource[0];
        cl_program program = clCreateProgramWithSource(_context, 1, (const char **) &kernelSourceC, &program_length,
                &errNum);

        if (errNum != CL_SUCCESS)
        {
            LOG(ERROR) << "Kernel program load from source error " << errNum;
        }
        errNum = clBuildProgram(program, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
        if (errNum != CL_SUCCESS)
        {
            LOG(ERROR) << "Kernel program build error " << errNum;
        }

        // create the kernel
        kernel = clCreateKernel(program, kernelName.c_str(), &errNum);
        if(errNum != CL_SUCCESS)
        {
            LOG(ERROR) << "Failed to create kernel" << errNum;
            return false;
        }
        return true;
    }
}