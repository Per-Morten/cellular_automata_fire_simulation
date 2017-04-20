cmake_minimum_required (VERSION 2.6)

add_library(cp_common
    cp_log.c
    cp_clock.cpp
)

target_link_libraries(cp_common
    "C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v8.0\\lib\\Win32\\OpenCL.lib"
    # m # For math
)
