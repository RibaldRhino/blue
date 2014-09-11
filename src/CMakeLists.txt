include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)

link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(blue main.cpp window.cpp)
target_link_libraries(blue glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})