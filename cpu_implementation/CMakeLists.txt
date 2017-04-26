cmake_minimum_required (VERSION 2.6)
project (cpu_implementation)

set (cpu_implementation_VERSION_MAJOR 1)
set (cpu_implementation_VERSION_MINOR 0)

add_executable(cpu_implementation
    constants.h
    cpu_grid.c
    cpu_grid.h
    cpu_simulation.c
    cpu_simulation.h
    main.c
)

target_link_libraries(cpu_implementation
    ${CP_STANDARD_LINKS}
    cp_common
    cp_sdl
)
