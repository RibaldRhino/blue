cmake_minimum_required(VERSION 2.8.4)
project(blue)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package( OpenCL REQUIRED )
find_package( Boost 1.54 REQUIRED )
find_package( Assimp REQUIRED )
FIND_PACKAGE( GLEW REQUIRED )
FIND_PACKAGE( OpenGL REQUIRED )
find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )


add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
