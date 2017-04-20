cmake_minimum_required (VERSION 2.6)
project (opencl_implementation)

set (opencl_implementation_VERSION_MAJOR 1)
set (opencl_implementation_VERSION_MINOR 0)

add_executable(opencl_implementation
    main.c
)

target_link_libraries(opencl_implementation
       	"C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v8.0\\lib\\win32\\OpenCL.lib"
        cp_common
        cp_sdl
)
