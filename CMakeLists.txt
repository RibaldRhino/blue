cmake_minimum_required(VERSION 2.8.4)
project(blue)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package( OpenCL REQUIRED )
find_package( Boost 1.54 REQUIRED )
find_package( Assimp REQUIRED )
find_package( OpenGL REQUIRED )

add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
