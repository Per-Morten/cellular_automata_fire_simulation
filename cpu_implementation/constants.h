#pragma once
const static int window_width = 640;
const static int window_height = 640;
const static int cell_width = 10;
const static int cell_height = 10;
const static int32_t max_fuel = 10;
const static float max_initial_temprature = 200.f;

const static float temprature_spread_up = 0.05f;
const static float temprature_spread_diagonal_up = 0.025f;
const static float temprature_spread_horizontal = 0.02f;
const static float temprature_spread_diagonal_down = 0.01f;
const static float temprature_spread_down = 0.01f;

const static float burning_threshold = 100.f;
const static float fuel_consumption_heat_increase = 100.f;