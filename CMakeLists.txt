cmake_minimum_required(VERSION 3.15)
project(hjson LANGUAGES CXX)

enable_testing()

# 构建选项
option(CMAKE_BUILD_NO_EXAMPLES "Build examples" OFF)
option(CMAKE_BUILD_TESTS "Build tests" ON)
option(CMAKE_BUILD_BENCHMARK "Build benchmarks" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# 编译器标志
set(CXX_FLAGS
        -fno-omit-frame-pointer # linux perf
        -Wall
        -Wextra
        -Werror
        -Wconversion
        -Wno-unused-parameter
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpointer-arith
        -Wshadow
        -Wwrite-strings
        -std=c++17
        -march=native
        -rdynamic)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# 包含目录
include_directories(${PROJECT_SOURCE_DIR})

# 打印构建信息
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build examples: ${CMAKE_BUILD_NO_EXAMPLES}")
message(STATUS "Build tests: ${CMAKE_BUILD_TESTS}")
message(STATUS "Build benchmarks: ${CMAKE_BUILD_BENCHMARK}")

add_subdirectory(hjson)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
    add_subdirectory(example)
endif()
if (CMAKE_BUILD_TESTS)
    add_subdirectory(hjson/test)
endif()
if (CMAKE_BUILD_BENCHMARK)
    add_subdirectory(hjson/bench)
endif()

