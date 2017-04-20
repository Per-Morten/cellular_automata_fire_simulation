cmake_minimum_required (VERSION 2.6)
project (gpu_programming)

include_directories("${PROJECT_BINARY_DIR}"
                    "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v8.0\\include"
                    "${CMAKE_SOURCE_DIR}/common"
                    "${CMAKE_SOURCE_DIR}/cpu_implementation"
                    "${CMAKE_SOURCE_DIR}/opencl_implementation"
                    "${CMAKE_SOURCE_DIR}/sdl"
                    "$ENV{SDL_HOME}\\include")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_C_COMPILER_ID EQUAL "GNU")
	set(C_FLAGS "-DCP_DEBUG_ON -Wall -Wextra -std=c11")
	set(CXX_FLAG "-DCP_DEBUG_ON -Wall -Wextra -std=c++14")
	set(DEBUG_FLAGS "-O0 -g3")
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAG}")


subdirs(
    common
    sdl
    cpu_implementation
    opencl_implementation
)

