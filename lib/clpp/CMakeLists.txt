include_directories(${OPENCL_INCLUDE_DIRS})

SET(LIBCLPP_SRCS
clpp.cpp clppContext.cpp clppScan_Default.cpp clppScan_GPU.cpp clppProgram.cpp clppSort_BitonicSort.cpp
clppSort_BitonicSortGPU.cpp
clppSort_CPU.cpp clppSort_RadixSort.cpp clppSort_RadixSortGPU.cpp clppSort.cpp StopWatch.cpp)

ADD_LIBRARY(clpp SHARED ${LIBCLPP_SRCS})

TARGET_LINK_LIBRARIES(clpp ${OPENCL_LIBRARIES})