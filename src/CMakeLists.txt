include_directories(${PROJECT_SOURCE_DIR}/lib/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)
include_directories(${OPENCL_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/lib/glm/glm)

link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES main.cpp Window/Window.cpp Window/Window_InputManager.cpp Event/EventManager.cpp Event/IEventData.cpp Game/Game.cpp Process/AbstractProcess.cpp
Process/ProcessManager.cpp World/World.cpp Game/GameProcesses/OutputProcess.cpp Game/GameProcesses/EventDispatchProcess.cpp Game/InputEvents/MouseEvent.cpp
Game/InputEvents/KeyboardEvent.cpp Game/InputEvents/OnWindowClosedEvent.cpp Game/GameProcesses/WorldUpdateProcess.cpp Game/GameProcesses/InitProcess.cpp
World/Actor/Actor.cpp World/Scene/Scene.cpp)

add_executable(blue ${SOURCE_FILES})
target_link_libraries(blue libglew_shared glfw ${GLFW_LIBRARIES} ${OPENCL_LIBRARIES} ${BOOST_LIBRARIES})

