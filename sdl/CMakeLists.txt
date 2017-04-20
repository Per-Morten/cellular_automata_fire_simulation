cmake_minimum_required (VERSION 2.6)

add_library(cp_sdl
    cp_sdl.c
)

target_link_libraries(cp_sdl
    $ENV{SDL_HOME}\\lib\\SDL2.lib
    "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v8.0\\lib\\Win32\\OpenCL.lib"
    cp_common
    # m # For math
)

# For testing
add_executable(cp_sdl_test
    main.c
)

target_link_libraries(cp_sdl_test
    $ENV{SDL_HOME}\\lib\\SDL2.lib
    "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v8.0\\lib\\Win32\\OpenCL.lib"
    cp_common
    cp_sdl
    # m # For math
)
