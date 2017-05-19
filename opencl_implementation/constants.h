#pragma once
#include <stdint.h>

static const int window_width = 800;
static const int window_height = 800;

static const int cell_height = 400;
static const int cell_width = 400;


#define CELLS_PER_ROW window_width / cell_width
#define CELLS_PER_COL window_height / cell_height

static const int max_initial_fuel = 10;
static const float max_initial_temprature = 500;
