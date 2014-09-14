include_directories(${PROJECT_SOURCE_DIR}/lib/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)
include_directories(${OPENCL_INCLUDE_DIRS})
include_directories(${BOOST_INCLUDE_DIRS})

link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES main.cpp Window.cpp Window_InputManager.cpp EventManager.cpp IEventData.cpp Game.cpp AbstractProcess.cpp
ProcessManager.cpp)

add_executable(blue ${SOURCE_FILES})
target_link_libraries(blue glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENCL_LIBRARIES} ${BOOST_LIBRARIES})

