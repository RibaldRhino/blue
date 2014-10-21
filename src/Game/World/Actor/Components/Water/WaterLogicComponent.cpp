#include <Game/World/Actor/Actor.hpp>
#include "WaterLogicComponent.hpp"
#include <log.hpp>
#include <stdio.h>

#ifdef _WIN32   // Windows version
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <CL/cl_gl.h>
#include <Event/EventManager.hpp>
#include <Event/Game/Window/OnWindowClose.hpp>

#if defined (__APPLE__) || defined(MACOSX)
#define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#else
#define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

// Error and Exit Handling Macros...
// *********************************************************************
// Full error handling macro with Cleanup() callback (if supplied)...
// (Companion Inline Function lower on page)
#define oclCheckErrorEX(a, b, c) __oclCheckErrorEX(a, b, c, __FILE__ , __LINE__)

// Short version without Cleanup() callback pointer
// Both Input (a) and Reference (b) are specified as args
#define oclCheckError(a, b) oclCheckErrorEX(a, b, 0)

const char* oclErrorString(cl_int error)
{
    static const char* errorString[] = {
            "CL_SUCCESS",
            "CL_DEVICE_NOT_FOUND",
            "CL_DEVICE_NOT_AVAILABLE",
            "CL_COMPILER_NOT_AVAILABLE",
            "CL_MEM_OBJECT_ALLOCATION_FAILURE",
            "CL_OUT_OF_RESOURCES",
            "CL_OUT_OF_HOST_MEMORY",
            "CL_PROFILING_INFO_NOT_AVAILABLE",
            "CL_MEM_COPY_OVERLAP",
            "CL_IMAGE_FORMAT_MISMATCH",
            "CL_IMAGE_FORMAT_NOT_SUPPORTED",
            "CL_BUILD_PROGRAM_FAILURE",
            "CL_MAP_FAILURE",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "CL_INVALID_VALUE",
            "CL_INVALID_DEVICE_TYPE",
            "CL_INVALID_PLATFORM",
            "CL_INVALID_DEVICE",
            "CL_INVALID_CONTEXT",
            "CL_INVALID_QUEUE_PROPERTIES",
            "CL_INVALID_COMMAND_QUEUE",
            "CL_INVALID_HOST_PTR",
            "CL_INVALID_MEM_OBJECT",
            "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
            "CL_INVALID_IMAGE_SIZE",
            "CL_INVALID_SAMPLER",
            "CL_INVALID_BINARY",
            "CL_INVALID_BUILD_OPTIONS",
            "CL_INVALID_PROGRAM",
            "CL_INVALID_PROGRAM_EXECUTABLE",
            "CL_INVALID_KERNEL_NAME",
            "CL_INVALID_KERNEL_DEFINITION",
            "CL_INVALID_KERNEL",
            "CL_INVALID_ARG_INDEX",
            "CL_INVALID_ARG_VALUE",
            "CL_INVALID_ARG_SIZE",
            "CL_INVALID_KERNEL_ARGS",
            "CL_INVALID_WORK_DIMENSION",
            "CL_INVALID_WORK_GROUP_SIZE",
            "CL_INVALID_WORK_ITEM_SIZE",
            "CL_INVALID_GLOBAL_OFFSET",
            "CL_INVALID_EVENT_WAIT_LIST",
            "CL_INVALID_EVENT",
            "CL_INVALID_OPERATION",
            "CL_INVALID_GL_OBJECT",
            "CL_INVALID_BUFFER_SIZE",
            "CL_INVALID_MIP_LEVEL",
            "CL_INVALID_GLOBAL_WORK_SIZE",
    };

    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "Unspecified Error";
}

inline void __oclCheckErrorEX(cl_int iSample, cl_int iReference, void (*pCleanup)(int), const char* cFile, const int iLine)
{
    // An error condition is defined by the sample/test value not equal to the reference
    if (iReference != iSample)
    {
        // If the sample/test value isn't equal to the ref, it's an error by defnition, so override 0 sample/test value
        iSample = (iSample == 0) ? -9999 : iSample;

        // Log the error info
        LOG(ERROR) << "Error # " << iSample << "(" << oclErrorString(iSample) << ")" << " at line " << iLine <<
                ", in file " << cFile;

        // Cleanup and exit, or just exit if no cleanup function pointer provided.  Use iSample (error code in this case) as process exit code.
        if (pCleanup != NULL)
        {
            pCleanup(iSample);
        }
        else
        {
            LOG(INFO) << "Exiting...";
            exit(iSample);
        }
    }
}

cl_int oclGetPlatformID(cl_platform_id* clSelectedPlatformID)
{
    char chBuffer[1024];
    cl_uint num_platforms;
    cl_platform_id* clPlatformIDs;
    cl_int ciErrNum;
    *clSelectedPlatformID = NULL;

    // Get OpenCL platform count
    ciErrNum = clGetPlatformIDs (0, NULL, &num_platforms);
    if (ciErrNum != CL_SUCCESS)
    {
        LOG(ERROR) << "Error " << ciErrNum << " in clGetPlatformIDs Call";
        return -1000;
    }
    else
    {
        if(num_platforms == 0)
        {
            LOG(ERROR) << "No OpenCL platform found";
            return -2000;
        }
        else
        {
            // if there's a platform or more, make space for ID's
            if ((clPlatformIDs = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id))) == NULL)
            {
                LOG(ERROR) << "Failed to allocate memory for cl_platform ID's";
                return -3000;
            }

            // get platform info for each platform and trap the NVIDIA platform if found
            ciErrNum = clGetPlatformIDs (num_platforms, clPlatformIDs, NULL);
            for(cl_uint i = 0; i < num_platforms; ++i)
            {
                ciErrNum = clGetPlatformInfo (clPlatformIDs[i], CL_PLATFORM_NAME, 1024, &chBuffer, NULL);
                if(ciErrNum == CL_SUCCESS)
                {
                    if(strstr(chBuffer, "NVIDIA") != NULL)
                    {
                        *clSelectedPlatformID = clPlatformIDs[i];
                        break;
                    }
                }
            }

            // default to zeroeth platform if NVIDIA not found
            if(*clSelectedPlatformID == NULL)
            {
                LOG(WARNING) << "WARNING: NVIDIA OpenCL platform not found - defaulting to first platform";
                *clSelectedPlatformID = clPlatformIDs[0];
            }

            free(clPlatformIDs);
        }
    }

    return CL_SUCCESS;
}

void oclPrintDevName(cl_device_id device)
{
    char device_string[1024];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_string), &device_string, NULL);
    LOG(INFO) << device_string;
}

cl_device_id oclGetFirstDev(cl_context cxGPUContext)
{
    size_t szParmDataBytes;
    cl_device_id* cdDevices;

    // get the list of GPU devices associated with context
    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, 0, NULL, &szParmDataBytes);
    cdDevices = (cl_device_id*) malloc(szParmDataBytes);

    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, szParmDataBytes, cdDevices, NULL);

    cl_device_id first = cdDevices[0];
    free(cdDevices);

    return first;
}

cl_device_id oclGetMaxFlopsDev(cl_context cxGPUContext)
{
    size_t szParmDataBytes;
    cl_device_id* cdDevices;

    // get the list of GPU devices associated with context
    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, 0, NULL, &szParmDataBytes);
    cdDevices = (cl_device_id*) malloc(szParmDataBytes);
    size_t device_count = szParmDataBytes / sizeof(cl_device_id);

    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, szParmDataBytes, cdDevices, NULL);

    cl_device_id max_flops_device = cdDevices[0];
    int max_flops = 0;

    size_t current_device = 0;

    // CL_DEVICE_MAX_COMPUTE_UNITS
    cl_uint compute_units;
    clGetDeviceInfo(cdDevices[current_device], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, NULL);

    // CL_DEVICE_MAX_CLOCK_FREQUENCY
    cl_uint clock_frequency;
    clGetDeviceInfo(cdDevices[current_device], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clock_frequency), &clock_frequency, NULL);

    max_flops = compute_units * clock_frequency;
    ++current_device;

    while( current_device < device_count )
    {
        // CL_DEVICE_MAX_COMPUTE_UNITS
        cl_uint compute_units;
        clGetDeviceInfo(cdDevices[current_device], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, NULL);

        // CL_DEVICE_MAX_CLOCK_FREQUENCY
        cl_uint clock_frequency;
        clGetDeviceInfo(cdDevices[current_device], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clock_frequency), &clock_frequency, NULL);

        int flops = compute_units * clock_frequency;
        if( flops > max_flops )
        {
            max_flops        = flops;
            max_flops_device = cdDevices[current_device];
        }
        ++current_device;
    }

    free(cdDevices);

    return max_flops_device;
}

char* oclLoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength)
{
    // locals
    FILE* pFileStream = NULL;
    size_t szSourceLength;

    // open the OpenCL source code file
    pFileStream = fopen(cFilename, "rb");
    if(pFileStream == 0)
    {
        return NULL;
    }

    size_t szPreambleLength = strlen(cPreamble);

    // get the length of the source code
    fseek(pFileStream, 0, SEEK_END);
    szSourceLength = ftell(pFileStream);
    fseek(pFileStream, 0, SEEK_SET);

    // allocate a buffer for the source code string and read it in
    char* cSourceString = (char *)malloc(szSourceLength + szPreambleLength + 1);
    memcpy(cSourceString, cPreamble, szPreambleLength);
    if (fread((cSourceString) + szPreambleLength, szSourceLength, 1, pFileStream) != 1)
    {
        fclose(pFileStream);
        free(cSourceString);
        return 0;
    }

    // close the file and return the total length of the combined (preamble + source) string
    fclose(pFileStream);
    if(szFinalLength != 0)
    {
        *szFinalLength = szSourceLength + szPreambleLength;
    }
    cSourceString[szSourceLength + szPreambleLength] = '\0';

    return cSourceString;
}

cl_device_id oclGetDev(cl_context cxGPUContext, unsigned int nr)
{
    size_t szParmDataBytes;
    cl_device_id* cdDevices;

    // get the list of GPU devices associated with context
    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, 0, NULL, &szParmDataBytes);

    if( szParmDataBytes / sizeof(cl_device_id) <= nr ) {
        return (cl_device_id)-1;
    }

    cdDevices = (cl_device_id*) malloc(szParmDataBytes);

    clGetContextInfo(cxGPUContext, CL_CONTEXT_DEVICES, szParmDataBytes, cdDevices, NULL);

    cl_device_id device = cdDevices[nr];
    free(cdDevices);

    return device;
}

void oclGetProgBinary( cl_program cpProgram, cl_device_id cdDevice, char** binary, size_t* length)
{
    // Grab the number of devices associated witht the program
    cl_uint num_devices;
    clGetProgramInfo(cpProgram, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint), &num_devices, NULL);

    // Grab the device ids
    cl_device_id* devices = (cl_device_id*) malloc(num_devices * sizeof(cl_device_id));
    clGetProgramInfo(cpProgram, CL_PROGRAM_DEVICES, num_devices * sizeof(cl_device_id), devices, 0);

    // Grab the sizes of the binaries
    size_t* binary_sizes = (size_t*)malloc(num_devices * sizeof(size_t));
    clGetProgramInfo(cpProgram, CL_PROGRAM_BINARY_SIZES, num_devices * sizeof(size_t), binary_sizes, NULL);

    // Now get the binaries
    char** ptx_code = (char**) malloc(num_devices * sizeof(char*));
    for( unsigned int i=0; i<num_devices; ++i) {
        ptx_code[i]= (char*)malloc(binary_sizes[i]);
    }
    clGetProgramInfo(cpProgram, CL_PROGRAM_BINARIES, 0, ptx_code, NULL);

    // Find the index of the device of interest
    unsigned int idx = 0;
    while( idx<num_devices && devices[idx] != cdDevice ) ++idx;

    // If it is associated prepare the result
    if( idx < num_devices )
    {
        *binary = ptx_code[idx];
        *length = binary_sizes[idx];
    }

    // Cleanup
    free( devices );
    free( binary_sizes );
    for( unsigned int i=0; i<num_devices; ++i) {
        if( i != idx ) free(ptx_code[i]);
    }
    free( ptx_code );
}

void oclLogBuildInfo(cl_program cpProgram, cl_device_id cdDevice)
{
    // write out the build log and ptx, then exit
    char cBuildLog[10240];
    clGetProgramBuildInfo(cpProgram, cdDevice, CL_PROGRAM_BUILD_LOG,
            sizeof(cBuildLog), cBuildLog, NULL );
    LOG(INFO) << "Build Log:";
    LOG(INFO) << cBuildLog;
}

void Cleanup(int iExitCode)
{
    LOG(ERROR) << "Cleanup func, exitcode = " << iExitCode;
    event::EventManager::get().TriggerEvent(std::make_shared<event::OnWindowClose>());
}

void (*pCleanup)(int) = &Cleanup;

game::WaterLogicComponent::WaterLogicComponent(game::ActorWPtr actorWPtr, GLuint vb)
{
    this->vbo = vb;
    // OpenCL vars
    cl_platform_id cpPlatform;
    cl_context cxGPUContext;
    cl_device_id* cdDevices;
    cl_uint uiDevCount;
    cl_program cpProgram;
    cl_int ciErrNum;
    char* cPathAndName = NULL;          // var for full paths to data, src, etc.
    char* cSourceCL = NULL;             // Buffer to hold source for compilation
    const char* cExecutableName = NULL;

    //Get the NVIDIA platform
    ciErrNum = oclGetPlatformID(&cpPlatform);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // Get the number of GPU devices available to the platform
    ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &uiDevCount);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // Create the device list
    cdDevices = new cl_device_id [uiDevCount];
    ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, uiDevCount, cdDevices, NULL);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    unsigned int uiDeviceUsed = 0;
    unsigned int uiEndDev = uiDevCount - 1;

    bool bSharingSupported = false;
    for(unsigned int i = uiDeviceUsed; (!bSharingSupported && (i <= uiEndDev)); ++i)
    {
        size_t extensionSize;
        ciErrNum = clGetDeviceInfo(cdDevices[i], CL_DEVICE_EXTENSIONS, 0, NULL, &extensionSize );
        oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
        if(extensionSize > 0)
        {
            char* extensions = (char*)malloc(extensionSize);
            ciErrNum = clGetDeviceInfo(cdDevices[i], CL_DEVICE_EXTENSIONS, extensionSize, extensions, &extensionSize);
            oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
            std::string stdDevString(extensions);
            free(extensions);

            size_t szOldPos = 0;
            size_t szSpacePos = stdDevString.find(' ', szOldPos); // extensions string is space delimited
            while (szSpacePos != stdDevString.npos)
            {
                if( strcmp(GL_SHARING_EXTENSION, stdDevString.substr(szOldPos, szSpacePos - szOldPos).c_str()) == 0 )
                {
                    // Device supports context sharing with OpenGL
                    uiDeviceUsed = i;
                    bSharingSupported = true;
                    break;
                }
                do
                {
                    szOldPos = szSpacePos + 1;
                    szSpacePos = stdDevString.find(' ', szOldPos);
                }
                while (szSpacePos == szOldPos);
            }
        }
    }
    if(bSharingSupported)
        LOG(INFO) << "Using CL-GL Interop";
    else
        LOG(INFO) << "No device found that supports CL/GL context sharing";
    oclCheckErrorEX(bSharingSupported, true, pCleanup);

    // Define OS-specific context properties and create the OpenCL context
#if defined (__APPLE__)
    CGLContextObj kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
    cl_context_properties props[] =
    {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
        0
    };
    cxGPUContext = clCreateContext(props, 0,0, NULL, NULL, &ciErrNum);
#else
    #ifdef UNIX
        cl_context_properties props[] =
        {
            CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM, (cl_context_properties)cpPlatform,
            0
        };
        cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &ciErrNum);
    #else // Win32
        cl_context_properties props[] =
            {
                CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                CL_CONTEXT_PLATFORM, (cl_context_properties)cpPlatform,
                0
            };
        cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &ciErrNum);
    #endif
#endif
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // Log device used (reconciled for requested requested and/or CL-GL interop capable devices, as applies)
    LOG(INFO) << "Device # " << uiDeviceUsed;
    oclPrintDevName(cdDevices[uiDeviceUsed]);

    // create a command-queue
    cqCommandQueue = clCreateCommandQueue(cxGPUContext, cdDevices[uiDeviceUsed], 0, &ciErrNum);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // Program Setup
    size_t program_length;
    cPathAndName = "Kernel/test.cl";
    cSourceCL = oclLoadProgSource(cPathAndName, "", &program_length);
    LOG(INFO) << "Program length: " << program_length;

    // create the program
    cpProgram = clCreateProgramWithSource(cxGPUContext, 1,
            (const char **) &cSourceCL, &program_length, &ciErrNum);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // build the program
    ciErrNum = clBuildProgram(cpProgram, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
    if (ciErrNum != CL_SUCCESS)
    {
        LOG(ERROR) << "build error";
        oclLogBuildInfo(cpProgram, oclGetFirstDev(cxGPUContext));
    }

    // create the kernel
    ckKernel = clCreateKernel(cpProgram, "sine_wave", &ciErrNum);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    vbo_cl = clCreateFromGLBuffer(cxGPUContext, CL_MEM_WRITE_ONLY, vbo, &ciErrNum);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // set the args values
    ciErrNum  = clSetKernelArg(ckKernel, 0, sizeof(cl_mem), (void *) &vbo_cl);
    ciErrNum |= clSetKernelArg(ckKernel, 1, sizeof(unsigned int), &mesh_width);
    ciErrNum |= clSetKernelArg(ckKernel, 2, sizeof(unsigned int), &mesh_height);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    offset = 0.0f;

    szGlobalWorkSize[0] = 10;
    szGlobalWorkSize[1] = 10;
}

void game::WaterLogicComponent::Update(double deltaTime)
{
    cl_int ciErrNum = CL_SUCCESS;

    glFinish();
    ciErrNum = clEnqueueAcquireGLObjects(cqCommandQueue, 1, &vbo_cl, 0,0,0);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // Set arg 3 and execute the kernel
    ciErrNum = clSetKernelArg(ckKernel, 3, sizeof(float), &offset);
    ciErrNum |= clEnqueueNDRangeKernel(cqCommandQueue, ckKernel, 2, NULL, szGlobalWorkSize, NULL, 0,0,0 );
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);

    // unmap buffer object
    ciErrNum = clEnqueueReleaseGLObjects(cqCommandQueue, 1, &vbo_cl, 0,0,0);
    oclCheckErrorEX(ciErrNum, CL_SUCCESS, pCleanup);
    clFinish(cqCommandQueue);

    offset += deltaTime;
}
