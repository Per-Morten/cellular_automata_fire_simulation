#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <constants.h>
#include <cp_common.h>
#include <cp_common.h>
#include <cpu_grid.h>
#include <cpu_simulation.h>

void
draw(cp_sdl_api* api,
     const cpu_cell** cells,
     const size_t rows,
     const size_t columns)
{
    cp_sdl_clear(api);
    for (size_t i = 0; i != rows; ++i)
    {
        for (size_t j = 0; j != columns; ++j)
        {
            cp_sdl_render_cell(api, &cells[i][j].graphics);
        }
    }
    cp_sdl_present(api);
}

void
update_cell_color(cpu_cell** cells,
                  const size_t rows,
                  const size_t columns)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
        {
            cells[i][j].graphics.color.r = (cells[i][j].temperature > UCHAR_MAX)
                                         ? UCHAR_MAX
                                         : (uint8_t)cells[i][j].temperature;

            cells[i][j].graphics.color.b = (uint8_t)(cells[i][j].fuel * UCHAR_MAX / max_fuel);
        }
    }
}

void
update_cell_temperature(cpu_cell** CP_RESTRICT read_cells,
                        cpu_cell** CP_RESTRICT write_cells,
                        const size_t rows,
                        const size_t colums)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < colums; ++j)
        {
            //give heat to nearby cells
            float initial_temperature = read_cells[i][j].temperature;

            float sum_temperature = initial_temperature -
                (initial_temperature * temperature_spread_up +
                 initial_temperature * temperature_spread_down +
                 initial_temperature * temperature_spread_diagonal_down * 2 +
                 initial_temperature * temperature_spread_diagonal_up * 2 +
                 initial_temperature * temperature_spread_horizontal * 2);

            //Add temperature from nearby cells
            //add from below
            sum_temperature += read_cells[i][j + 1].temperature * temperature_spread_up;
            //add from above
            sum_temperature += read_cells[i][j - 1].temperature * temperature_spread_down;
            //add from diagonal below
            sum_temperature += read_cells[i + 1][j + 1].temperature * temperature_spread_diagonal_up;
            sum_temperature += read_cells[i - 1][j + 1].temperature * temperature_spread_diagonal_up;
            //add from horizontal
            sum_temperature += read_cells[i + 1][j].temperature * temperature_spread_horizontal;
            sum_temperature += read_cells[i - 1][j].temperature * temperature_spread_horizontal;
            //add from diagonal above
            sum_temperature += read_cells[i + 1][j - 1].temperature * temperature_spread_diagonal_down;
            sum_temperature += read_cells[i - 1][j - 1].temperature * temperature_spread_diagonal_down;

            write_cells[i][j].temperature = sum_temperature;

            if (read_cells[i][j].temperature > burning_threshold &&
                read_cells[i][j].fuel > 0)
            {
                write_cells[i][j].temperature += fuel_consumption_heat_increase;
                write_cells[i][j].fuel = read_cells[i][j].fuel - 1;
            }
        }
    }
}

cpu_cell**
create_grid(const size_t rows,
            const size_t columns)
{
    // An outline is added within the grid to avoid bounds checking,
    // which is more efficient on cpu, and much more efficient on gpu.
    size_t outer_rows = rows + 2;
    size_t outer_columns = columns + 2;

    cpu_cell** cells = (cpu_cell**)malloc(outer_rows * sizeof(cpu_cell*));

    for (size_t i = 0; i != outer_rows; ++i)
    {
        cells[i] = (cpu_cell*)malloc(outer_columns * sizeof(cpu_cell));
        // pointer is incremented to the first element, outer element is not seen by external code.
        memset(cells[i]++, 0, outer_columns * sizeof(cpu_cell));
    }

    // pointer is incremented to the first layer, outer layer is not seen by external code.
    cells++;

    // initialize all "internal" cells
    for (size_t i = 0; i != rows; ++i)
    {
        for (size_t j = 0; j != columns; ++j)
        {
            cells[i][j].graphics.h = cell_height;
            cells[i][j].graphics.w = cell_width;
            cells[i][j].graphics.x = cell_width * i;
            cells[i][j].graphics.y = cell_height * j;

            cells[i][j].graphics.border_width = 0;

            cells[i][j].graphics.border_color.r = 0x00;
            cells[i][j].graphics.border_color.g = 0xF0;
            cells[i][j].graphics.border_color.b = 0x00;
            cells[i][j].graphics.border_color.a = 0xFF;

            cells[i][j].graphics.color.r = 0x00;
            cells[i][j].graphics.color.g = 0x00;
            cells[i][j].graphics.color.b = 0x00;
            cells[i][j].graphics.color.a = 0x00;

            cells[i][j].fuel = rand() % max_fuel;
            cells[i][j].temperature = (float)(rand() % (int)max_initial_temperature);
        }
    }

    return cells;
}

cpu_cell**
copy_grid(cpu_cell** CP_RESTRICT destination,
          cpu_cell** CP_RESTRICT source,
          const size_t rows,
          const size_t columns)
{
    for (size_t i = 0; i < rows; ++i)
    {
        memcpy(destination[i], source[i], sizeof(cpu_cell) * columns);
    }

    return destination;
}

void
destroy_grid(cpu_cell** cells,
             const size_t rows)
{
    // Decrement pointer to get to outer layer.
    --cells;
    for (size_t i = 0; i != rows; ++i)
    {
        free(--cells[i]);
    }
    free(cells);
}
