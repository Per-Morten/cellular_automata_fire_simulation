#include <cp_lib_types.h>
#define TEMPERATURE_SPREAD_DIAGONAL_DOWN 0.01f
#define TEMPERATURE_SPREAD_DIAGONAL_UP 0.025f
#define TEMPERATURE_SPREAD_DOWN 0.01f
#define TEMPERATURE_SPREAD_HORIZONTAL 0.02f
#define TEMPERATURE_SPREAD_UP 0.05f
#define BURNING_THRESHOLD 100.f
#define FUEL_CONSUMPTION_HEAT_INCREASE 25.f

size_t calculate_index(size_t x_offset, size_t y_offset)
{
    size_t x_id = get_global_id(0);
    size_t y_id = get_global_id(1);

    size_t total_offset = get_global_offset(0) + get_global_offset(1);
    return ((y_id + y_offset) * (get_global_size(0) + total_offset)) + (x_id + x_offset);
}

__kernel void update_temperature(__global const float* restrict read_temperatures,
                                 __global const int32_t* restrict read_fuel,
                                 __global float* restrict write_temperatures,
                                 __global int32_t* restrict write_fuel)
{
    size_t x_id = get_global_id(0);
    size_t y_id = get_global_id(1);

    size_t total_offset = get_global_offset(0) + get_global_offset(1);
    size_t index = (y_id * (get_global_size(0) + total_offset)) + x_id;

    float initial_temperature = read_temperatures[index];

    float sum_temperature = initial_temperature -
                (initial_temperature * TEMPERATURE_SPREAD_UP +
                 initial_temperature * TEMPERATURE_SPREAD_DOWN +
                 initial_temperature * TEMPERATURE_SPREAD_DIAGONAL_DOWN * 2 +
                 initial_temperature * TEMPERATURE_SPREAD_DIAGONAL_UP * 2 +
                 initial_temperature * TEMPERATURE_SPREAD_HORIZONTAL * 2);

    //
    //add from below
    //add from above
    sum_temperature += read_temperatures[calculate_index( 1, 0)] * TEMPERATURE_SPREAD_UP;
    sum_temperature += read_temperatures[calculate_index(-1, 0)] * TEMPERATURE_SPREAD_DOWN;
    //add from diagonal below
    sum_temperature += read_temperatures[calculate_index( 1, +1)] * TEMPERATURE_SPREAD_DIAGONAL_UP;
    sum_temperature += read_temperatures[calculate_index( 1, -1)] * TEMPERATURE_SPREAD_DIAGONAL_UP;
    //add from horizontal
    sum_temperature += read_temperatures[calculate_index( 0, +1)] * TEMPERATURE_SPREAD_HORIZONTAL;
    sum_temperature += read_temperatures[calculate_index( 0, -1)] * TEMPERATURE_SPREAD_HORIZONTAL;
    //add from diagonal above
    sum_temperature += read_temperatures[calculate_index(-1, +1)] * TEMPERATURE_SPREAD_DIAGONAL_DOWN;
    sum_temperature += read_temperatures[calculate_index(-1, -1)] * TEMPERATURE_SPREAD_DIAGONAL_DOWN;


    if (initial_temperature > BURNING_THRESHOLD && read_fuel[index] > 0)
    {
        sum_temperature += FUEL_CONSUMPTION_HEAT_INCREASE;
        write_fuel[index] = read_fuel[index] - 1;
    }

    write_temperatures[index] = sum_temperature;
}


__kernel void swap_buffers(__global const float* restrict source_temperatures,
                           __global const int32_t* restrict source_fuel,
                           __global float* restrict destination_temperatures,
                           __global int32_t* restrict destination_fuel)
{
    size_t id = get_global_id(0);

    destination_temperatures[id] = source_temperatures[id];
    destination_fuel[id] = source_fuel[id];
}
