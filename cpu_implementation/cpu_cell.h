#pragma once
#include <stdint.h>
#include <cp_sdl.h>

typedef struct
{
    cp_sdl_cell graphics;
    int32_t fuel;
    float temperature;
} cpu_cell;
