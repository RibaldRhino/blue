include_directories(${OPENCL_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm)
include_directories(${PROJECT_SOURCE_DIR}/lib/clpp)

link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES main.cpp System/Window/Window.cpp System/Window/Window_InputManager.cpp Event/EventManager.cpp Game/Game.cpp Game/Process/AbstractProcess.cpp
Game/Process/ProcessManager.cpp Game/World/World.cpp Game/Process/Game/OutputProcess.cpp Game/Process/Game/EventDispatchProcess.cpp Event/Game/Window/OnWindowClose.cpp
Game/Process/Game/WorldUpdateProcess.cpp Game/Process/Game/InputProcess.cpp Event/Game/Window/OnWindowResized.cpp Event/Game/Window/OnCursorPositionChanged.cpp
Game/Timer.cpp Event/Game/Window/Down/OnDownPressed.cpp Event/Game/Window/Down/OnRightPressed.cpp Event/Game/Window/Down/OnUpPressed.cpp Event/Game/Window/Down/OnLeftPressed.cpp
Event/Game/Window/Down/OnLMBPressed.cpp Event/Game/Window/Down/OnRMBPressed.cpp Game/World/Actor/Actor.cpp

System/OpenCLSystem.cpp System/OpenGLSystem.cpp

Event/Game/Window/Up/OnUpReleased.cpp Event/Game/Window/Up/OnRightReleased.cpp Event/Game/Window/Up/OnDownReleased.cpp Event/Game/Window/Up/OnLeftReleased.cpp
Event/Game/Window/Up/OnLMBReleased.cpp Event/Game/Window/Up/OnRMBReleased.cpp

Game/World/Actor/Components/Basic/BasicRendererComponent.cpp Game/World/Actor/Components/TransformComponentt.cpp
Game/World/Actor/Components/Basic/BasicModelComponent.cpp Game/World/Actor/Components/Camera/CameraModelComponent.cpp Game/World/Actor/Components/IComponent.cpp
Game/World/Actor/Components/Camera/CameraLogicComponent.cpp Game/World/Actor/Components/TransformComponent.hpp Game/World/Actor/Components/Background/BackgroundRendererComponent.cpp
Game/World/Actor/Components/Water/WaterLogicComponent.cpp Game/World/Actor/Components/Water/WaterRendererComponent.cpp Game/World/Actor/Components/Water/WaterModelComponent.cpp)

add_executable(blue ${SOURCE_FILES})
target_link_libraries(blue libglew_shared glfw clpp ${GLFW_LIBRARIES} ${OPENCL_LIBRARIES} ${BOOST_LIBRARIES} ${ASSIMP_LIBRARY_RELEASE})

add_custom_target(copy_kernels)
set(KERNEL_FILES test.cl)
foreach(KernelFile ${KERNEL_FILES})
  add_custom_command(TARGET copy_kernels PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Kernel/${KernelFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Kernel/${KernelFile})
endforeach()
add_dependencies(blue copy_kernels)

add_custom_target(copy_clpp_kernels)
set(CLPP_KERNEL_FILES clppScan_Default.cl clppSort_RadixSort.cl clppSort_RadixSortGPU.cl clppScan_GPU.cl clppCount.cl clppSort_BitonicSort.cl clppSort_BitonicSortGPU.cl)
foreach(KernelFile ${CLPP_KERNEL_FILES})
  add_custom_command(TARGET copy_clpp_kernels PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/lib/clpp/${KernelFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Kernel/${KernelFile})
endforeach()
add_dependencies(blue copy_clpp_kernels)

add_custom_target(copy_shaders)
set(SHADER_FILES shader_frag.glsl shader_vert.glsl watershader_frag.glsl watershader_vert.glsl watershader_geom.glsl)
foreach(ShaderFile ${SHADER_FILES})
  add_custom_command(TARGET copy_shaders PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Shader/${ShaderFile} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shader/${ShaderFile})
endforeach()
add_dependencies(blue copy_shaders)

add_custom_target(copy_assets)
set(ASSETS box.obj floor.obj)
foreach(Asset ${ASSETS})
  add_custom_command(TARGET copy_assets PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/assets/${Asset} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/${Asset})
endforeach()
add_dependencies(blue copy_assets)

if(WIN32)
add_custom_target(copy_assimp_dll)
add_custom_command(TARGET copy_assimp_dll PRE_BUILD
                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/assets/libassimp.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libassimp.dll)
add_dependencies(blue copy_assimp_dll)

add_custom_target(copy_clpp_dll)
add_custom_command(TARGET copy_clpp_dll PRE_BUILD
                 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/lib/clpp/libclpp.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libclpp.dll)
add_dependencies(blue copy_clpp_dll)
endif()


