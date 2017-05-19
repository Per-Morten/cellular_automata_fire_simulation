#include <stdio.h>

#include <pthread.h>

#include <constants.h>
#include <cp_common.h>
#include <cp_sdl.h>
#include <cpu_cell.h>
#include <cpu_grid.h>
#include <cpu_simulation.h>

// To prepare for exam:
// Algorithm / Transform
// Circular Hough Algorithm
// Linear Hough Algorithm (?)
// Date: 23-24 may.

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
        size_t cell_count = window_width / cell_width;

        cpu_simulation sim_data = create_simulation(cell_count);
        pthread_t simulation_thread;
        pthread_create(&simulation_thread, NULL, run_simulation, &sim_data);
        bool should_continue = true;

        while (should_continue)
        {
            sem_post(sim_data.start_sync);

            draw(&sdl_api, (const cpu_cell**)sim_data.double_buffer->read,
                 cell_count, cell_count);

            should_continue = cp_sdl_handle_events();
            sem_wait(sim_data.finish_sync);

            copy_grid(sim_data.double_buffer->read,
                      sim_data.double_buffer->write,
                      sim_data.double_buffer->count,
                      sim_data.double_buffer->count);

            update_cell_color(sim_data.double_buffer->read,
                              sim_data.double_buffer->count,
                              sim_data.double_buffer->count);

            *sim_data.should_continue = should_continue;
        }
        sem_post(sim_data.start_sync);

        pthread_join(simulation_thread, NULL);
        destroy_simulation(&sim_data);
    }

    cp_sdl_shutdown(&sdl_api);
    cp_log_shutdown();

    return 0;
}
