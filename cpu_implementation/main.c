#include <stdio.h>
#include <pthread.h>

#include <cp_sdl.h>

#include <cp_common.h>
#include <cpu_cell.h>
#include <cpu_grid.h>
#include <constants.h>

#undef main

int
main(CP_UNUSED int argc,
     CP_UNUSED char** argv)
{


    cp_log_init();
    cp_sdl_api sdl_api;
    int32_t result = cp_sdl_init(&sdl_api, "cpu implementation",
                                 window_width, window_height);

    if (result == CP_SUCCESS)
    {
        size_t row_count = window_width / cell_width;
        size_t column_count = window_height / cell_height;
        cpu_cell** cells = create_grid(row_count,
                                       column_count);

        while (cp_sdl_handle_events())
        {
            draw(&sdl_api, (const cpu_cell**)cells,
                 row_count, column_count);
        }

        destroy_grid(cells,
                     window_width / cell_width);

    }

    cp_sdl_shutdown(&sdl_api);
    cp_log_shutdown();

    return 0;
}
