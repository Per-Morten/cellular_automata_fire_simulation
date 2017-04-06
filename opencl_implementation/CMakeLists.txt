cmake_minimum_required (VERSION 2.6)
project (opencl_implementation)

set (opencl_implementation_VERSION_MAJOR 1)
set (opencl_implementation_VERSION_MINOR 0)

add_executable(opencl_implementation
    main.c
)

target_link_libraries(opencl_implementation
       	OpenCL
        cp_common
        SDL2
)
