#include <stdio.h>
#include <cp_sdl.h>

#include <cp_common.h>

#include <constants.h>


cp_sdl_cell** create_draw_grid()
{
    cp_sdl_cell** cells = malloc(CELLS_PER_ROW * sizeof(cp_sdl_cell*));
    for (size_t row = 0; row < CELLS_PER_ROW; ++row)
    {
        cells[row] = malloc(CELLS_PER_COL * sizeof(cp_sdl_cell));
    }

    for (size_t x = 0; x < CELLS_PER_ROW; ++x)
    {
        for (size_t y = 0; y < CELLS_PER_COL; ++y)
        {
            cells[x][y].x = x * cell_width;
            cells[x][y].y = y * cell_height;
            cells[x][y].h = cell_height;
            cells[x][y].w = cell_width;

            cells[x][y].border_width = 1;
            cells[x][y].border_color.r = 0x00;
            cells[x][y].border_color.g = 0x00;
            cells[x][y].border_color.b = 0x00;
            cells[x][y].border_color.a = 0xFF;

            cells[x][y].color.r = 0xFF;
            cells[x][y].color.g = 0x00;
            cells[x][y].color.b = 0x00;
            cells[x][y].color.a = 0xFF;
        }
    }

    return cells;
}

void draw_grid(cp_sdl_api* api,
               cp_sdl_cell** grid)
{
    for (size_t i = 0; i < CELLS_PER_ROW; ++i)
    {
        for (size_t j = 0; j < CELLS_PER_COL; ++j)
        {
            cp_sdl_render_cell(api, &grid[i][j]);
        }
    }
}



void destroy_draw_grid(cp_sdl_cell** cells)
{
    for (size_t row = 0; row < CELLS_PER_ROW; ++row)
    {
        free(cells[row]);
    }
    free(cells);
}

void update_cell_color(cp_sdl_cell** cells,
                       float* temprature_cells)
{
    int index = 0;
    for (size_t row = 0; row < CELLS_PER_ROW; ++row)
    {
        for (size_t col = 0; col < CELLS_PER_COL; ++col)
        {
            index = (CELLS_PER_COL + 2)*(row + 1) + col + 1;
            cells[row][col].color.r = (uint8_t)temprature_cells[index];
        }
    }
}

#undef main

int
main(CP_UNUSED int argc,
     CP_UNUSED char** argv)
{
    cp_sdl_api sdl_api;
    int32_t result = cp_sdl_init(&sdl_api, "cpu implementation",
                                 window_width, window_height);

    int amount_of_simulation_cells = (CELLS_PER_COL + 2)*(CELLS_PER_ROW + 2);

    int* fuel_cells = malloc(amount_of_simulation_cells * sizeof(int));
    float* temperature_cells = malloc(amount_of_simulation_cells * sizeof(float));
    memset(fuel_cells, 0, amount_of_simulation_cells * sizeof(int));
    memset(temperature_cells, 0, amount_of_simulation_cells * sizeof(int));
    temperature_cells[5] = 100;
    temperature_cells[10] = 200;

    if (result == CP_SUCCESS)
    {

        cp_sdl_cell** grid = create_draw_grid();

        bool should_continue = true;

        while (should_continue)
        {
            should_continue = cp_sdl_handle_events();
            update_cell_color(grid, temperature_cells);

            cp_sdl_clear(&sdl_api);
            draw_grid(&sdl_api, grid);
            cp_sdl_present(&sdl_api);

        }

        destroy_draw_grid(grid);
        
    }

    free(fuel_cells);
    cp_sdl_shutdown(&sdl_api);

	return 0;
}

