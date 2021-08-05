cmake_minimum_required(VERSION 3.19)

project(ACO)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}" ${PROJECT_SOURCE_DIR}/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY )

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/source)

file(GLOB_RECURSE SRC_LIST
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/source/*.cpp"
        )

add_executable(ACO ${SRC_LIST})