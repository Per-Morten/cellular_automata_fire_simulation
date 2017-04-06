cmake_minimum_required (VERSION 2.6)

add_library(cp_sdl
    cp_sdl.c
)

target_link_libraries(cp_sdl
    SDL2
    cp_common
    m # For math
)

# For testing
add_executable(cp_sdl_test
    main.c
)

target_link_libraries(cp_sdl_test
    SDL2
    cp_common
    cp_sdl
    m # For math
)
