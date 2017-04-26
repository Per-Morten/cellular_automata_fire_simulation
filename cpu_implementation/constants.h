#pragma once
#include <stdint.h>

static const int window_width = 640;
static const int window_height = 640;
static const int cell_width = 10;
static const int cell_height = 10;
static const int32_t max_fuel = 10;
static const float max_initial_temperature = 200.f;

static const float temperature_spread_up = 0.05f;
static const float temperature_spread_diagonal_up = 0.025f;
static const float temperature_spread_horizontal = 0.02f;
static const float temperature_spread_diagonal_down = 0.01f;
static const float temperature_spread_down = 0.01f;

static const float burning_threshold = 100.f;
static const float fuel_consumption_heat_increase = 100.f;
