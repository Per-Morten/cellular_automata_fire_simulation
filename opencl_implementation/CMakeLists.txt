cmake_minimum_required (VERSION 2.6)
project (opencl_implementation)

set (opencl_implementation_VERSION_MAJOR 1)
set (opencl_implementation_VERSION_MINOR 0)

include_directories(opencl_implementation
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(opencl_implementation
    main.c
    opencl_utility.c
    opencl_utility.h
    print_info.c
)

target_link_libraries(opencl_implementation
    ${CP_STANDARD_LINKS}
    cp_common
    cp_sdl
)
