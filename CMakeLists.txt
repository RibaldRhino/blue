cmake_minimum_required(VERSION 2.8.4)
project(blue)

find_package(GLEW REQUIRED)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)