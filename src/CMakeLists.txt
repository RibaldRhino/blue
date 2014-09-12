include_directories(${PROJECT_SOURCE_DIR}/lib/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)
include_directories(${OPENCL_INCLUDE_DIRS})

link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
configure_file(HelloWorld.cl ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/HelloWorld.cl COPYONLY)


add_executable(blue main.cpp window.cpp)
target_link_libraries(blue glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENCL_LIBRARIES})

