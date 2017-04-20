cmake_minimum_required (VERSION 2.6)
project (cpu_implementation)

set (cpu_implementation_VERSION_MAJOR 1)
set (cpu_implementation_VERSION_MINOR 0)

add_executable(cpu_implementation
    main.c
)

target_link_libraries(cpu_implementation
       	$ENV{CUDA_PATH}\\lib\\Win32\\OpenCL.lib
        cp_common
        cp_sdl
)
