cmake_minimum_required (VERSION 2.6)
project (gpu_programming)

set(CP_STANDARD_INCLUDES "")
set(CP_STANDARD_LINKS "")

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CP_STANDARD_INCLUDES "${CP_STANDARD_INCLUDES} /opt/intel/opencl-1.2-sdk-6.3.0.1904/include/")
    set(CP_STANDARD_LINKS ${CP_STANDARD_LINKS} SDL2)
    set(CP_STANDARD_LINKS ${CP_STANDARD_LINKS} OpenCL)
    set(CP_STANDARD_LINKS ${CP_STANDARD_LINKS} m)
else()
    set(CP_STANDARD_INCLUDES ${CP_STANDARD_INCLUDES} C:\\Program\ Files\\NVIDIA\ GPU\ Computing\ Toolkit\\CUDA\\v8.0\\include)
    set(CP_STANDARD_INCLUDES ${CP_STANDARD_INCLUDES} $ENV{SDL_HOME}\\include)
    set(CP_STANDARD_LINKS ${CP_STANDARD_LINKS} C:\\Program\ Files\\NVIDIA\ GPU\ Computing\ Toolkit\\CUDA\\v8.0\\lib\\Win32\\OpenCL.lib)
    set(CP_STANDARD_LINKS ${CP_STANDARD_LINKS} $ENV{SDL_HOME}\\lib\\SDL2.lib)
endif()

include_directories("${PROJECT_BINARY_DIR}"
                    "${CMAKE_SOURCE_DIR}/common"
                    "${CMAKE_SOURCE_DIR}/cpu_implementation"
                    "${CMAKE_SOURCE_DIR}/opencl_implementation"
                    "${CMAKE_SOURCE_DIR}/sdl"
                    "${CP_STANDARD_INCLUDES}")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(C_FLAGS "-DCP_DEBUG_ON -Wall -Wextra -W -std=c11")
    set(CXX_FLAG "-DCP_DEBUG_ON -Wall -Wextra -std=c++14")
    set(DEBUG_FLAGS "-O0 -g3")
else()
    # TODO: Find out what options to do here
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

