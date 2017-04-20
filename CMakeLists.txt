cmake_minimum_required (VERSION 2.6)
project (gpu_programming)

include_directories("${PROJECT_BINARY_DIR}"
                    "/opt/intel/opencl-1.2-sdk-6.3.0.1904/include/"
                    "${CMAKE_SOURCE_DIR}/common"
                    "${CMAKE_SOURCE_DIR}/cpu_implementation"
                    "${CMAKE_SOURCE_DIR}/opencl_implementation"
                    "${CMAKE_SOURCE_DIR}/sdl")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(GCC_C_FLAGS "-DCP_DEBUG_ON -Wall -Wextra -std=c11")
set(GCC_CXX_FLAGS "-DCP_DEBUG_ON -Wall -Wextra -std=c++14")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${GCC_C_FLAGS} -O0 -g3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS} -O3")

subdirs(
    common
    sdl
    cpu_implementation
    opencl_implementation
)

