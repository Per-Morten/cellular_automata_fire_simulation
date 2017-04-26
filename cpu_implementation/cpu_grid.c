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
            cells[i][j].graphics.color.r = (cells[i][j].temprature > UCHAR_MAX)
                                         ? UCHAR_MAX
                                         : (uint8_t)cells[i][j].temprature;

            cells[i][j].graphics.color.b = (uint8_t)(cells[i][j].fuel * UCHAR_MAX / max_fuel);
        }
    }
}

void
update_cell_temprature(cpu_cell** read_cells,
                       cpu_cell** write_cells,
                       const size_t rows,
                       const size_t colums)
{
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < colums; ++j)
        {
            //give heat to nearby cells
            float initialTemprature = read_cells[i][j].temprature;

            write_cells[i][j].temprature = initialTemprature -
                (initialTemprature*temprature_spread_up +
                 initialTemprature*temprature_spread_down +
                 initialTemprature*temprature_spread_diagonal_down * 2 +
                 initialTemprature*temprature_spread_diagonal_up * 2 +
                 initialTemprature*temprature_spread_horizontal * 2);

            //Add temprature from nearby cells
            //add from below
            write_cells[i][j].temprature += read_cells[i][j + 1].temprature*temprature_spread_up;
            //add from above
            write_cells[i][j].temprature += read_cells[i][j - 1].temprature*temprature_spread_down;
            //add from diagonal below
            write_cells[i][j].temprature += read_cells[i + 1][j + 1].temprature*temprature_spread_diagonal_up;
            write_cells[i][j].temprature += read_cells[i - 1][j + 1].temprature*temprature_spread_diagonal_up;
            //add from horizontal
            write_cells[i][j].temprature += read_cells[i + 1][j].temprature*temprature_spread_horizontal;
            write_cells[i][j].temprature += read_cells[i - 1][j].temprature*temprature_spread_horizontal;
            //add from diagonal above
            write_cells[i][j].temprature += read_cells[i + 1][j - 1].temprature*temprature_spread_diagonal_down;
            write_cells[i][j].temprature += read_cells[i - 1][j - 1].temprature*temprature_spread_diagonal_down;

            if (read_cells[i][j].temprature > burning_threshold
                && read_cells[i][j].fuel > 0)
            {
                write_cells[i][j].temprature += fuel_consumption_heat_increase;
                write_cells[i][j].fuel = read_cells[i][j].fuel - 1;
            }
        }
    }
}

cpu_cell**
create_grid(const size_t rows,
            const size_t columns)
{
    cpu_cell** cells;
    /*We need a outline for the grid to remove out of bounds checking.
    therefore we allocate a layer of cells outside the grid with values that will not affect the simulation.
    we went for this solution as the bounds checking is quite expensive, and not wanted on the cpu, even less on the gpu
    */
    size_t outerRows = rows + 2;
    size_t outerColumns = columns + 2;

    cells = (cpu_cell**)malloc(outerRows * sizeof(cpu_cell*));
    for (size_t i = 0; i != outerRows; ++i)
    {

        cells[i] = (cpu_cell*)malloc(outerColumns * sizeof(cpu_cell));
        memset(cells[i], 0, outerColumns * sizeof(cpu_cell));
        /*
        increment the pointer to the first element after creation, so the outer layer will not be seen by external code
        */
        cells[i]++;
    }
    /*
    increment the pointer to the first array, so the outer layer will not be seen by external code
    */
    cells++;

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
            cells[i][j].temprature = (float)(rand() % (int)max_initial_temprature);
        }
    }

    return cells;
}

cpu_cell**
copy_grid(cpu_cell** destination,
          cpu_cell** source,
          const size_t rows,
          const size_t columns)
{
    size_t outerRows = rows + 2;
    size_t outerColumns = columns + 2;

    --source;
    --destination;
    for (size_t i = 0; i < outerRows; ++i)
    {
        memcpy(destination[i] - 1, source[i] - 1, sizeof(cpu_cell) * outerColumns);
    }

    return destination;
}

void
destroy_grid(cpu_cell** cells,
             const size_t count)
{
    /*
    at this point we need to revert the pointers back to the start of their respective arrays. further explanation in create_grid.
    */
    cells--;
    for (size_t i = 0; i != count; ++i)
    {
        free(--cells[i]);
    }
    free(cells);
}
