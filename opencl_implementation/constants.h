#pragma once
#include <stdint.h>

static const int32_t window_width = 800;
static const int32_t window_height = 800;

static const int32_t cell_height = 5;
static const int32_t cell_width = 5;


#define CELLS_PER_ROW window_width / cell_width
#define CELLS_PER_COL window_height / cell_height

static const int32_t max_initial_fuel = 10;
static const float max_initial_temperature = 150.0f;
