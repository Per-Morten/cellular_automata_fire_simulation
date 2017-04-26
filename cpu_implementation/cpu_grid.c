#include <cpu_grid.h>
#include <constants.h>
#include <stdlib.h>
#include <limits.h>

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
            if (cells[i][j].temprature > UCHAR_MAX)
            {
                cells[i][j].graphics.color.r = UCHAR_MAX;
            }
            else
            {
                cells[i][j].graphics.color.r = (uint8_t)cells[i][j].temprature;
            }
            cells[i][j].graphics.color.b = (uint8_t)(cells[i][j].fuel * UCHAR_MAX / max_fuel);
        }
    }
}

cpu_cell**
create_grid(const size_t rows,
            const size_t columns)
{
    cpu_cell** cells;

    cells = (cpu_cell**)malloc(rows * sizeof(cpu_cell*));
    for (size_t i = 0; i != rows; ++i)
    {
        cells[i] = (cpu_cell*)malloc(columns * sizeof(cpu_cell));
    }

    for (size_t i = 0; i != rows; ++i)
    {
        for (size_t j = 0; j != columns; ++j)
        {
            cells[i][j].graphics.h = cell_height;
            cells[i][j].graphics.w = cell_width;
            cells[i][j].graphics.x = cell_width * i;
            cells[i][j].graphics.y = cell_height * j;

            cells[i][j].graphics.border_width = 1;

            cells[i][j].graphics.border_color.r = 0x00;
            cells[i][j].graphics.border_color.g = 0xF0;
            cells[i][j].graphics.border_color.b = 0x00;
            cells[i][j].graphics.border_color.a = 0xFF;

            cells[i][j].graphics.color.r = 0x00;
            cells[i][j].graphics.color.g = 0x00;
            cells[i][j].graphics.color.b = 0x00;
            cells[i][j].graphics.color.a = 0x00;

            cells[i][j].fuel = rand() % max_fuel;
            cells[i][j].temprature = rand() % max_initial_temprature;
        }
    }

    return cells;
}

void
destroy_grid(cpu_cell** cells,
             const size_t count)
{
    for (size_t i = 0; i != count; ++i)
    {
        free(cells[i]);
    }
    free(cells);
}
