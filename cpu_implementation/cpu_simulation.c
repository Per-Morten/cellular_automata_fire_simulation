#include <cpu_simulation.h>
#include <cpu_grid.h>

cpu_simulation
create_simulation(const size_t cell_count)
{
    cpu_double_buffer* grid = (cpu_double_buffer*)malloc(sizeof(cpu_double_buffer));
    grid->count = cell_count;
    grid->read =  create_grid(cell_count,
                              cell_count);

    grid->write = create_grid(cell_count,
                              cell_count);

    sem_t* start_sync = (sem_t*)malloc(sizeof(sem_t));
    sem_t* finish_sync = (sem_t*)malloc(sizeof(sem_t));
    sem_init(start_sync, 0, 0);
    sem_init(finish_sync, 0, 0);

    bool* should_continue = (bool*)malloc(sizeof(bool));
    *should_continue = true;

    cpu_simulation sim =
    {
        .double_buffer = grid,
        .start_sync = start_sync,
        .finish_sync = finish_sync,
        .should_continue = should_continue,
    };
    return sim;
}

void
destroy_simulation(cpu_simulation* sim)
{
    destroy_grid(sim->double_buffer->read, sim->double_buffer->count);
    destroy_grid(sim->double_buffer->write, sim->double_buffer->count);
    free(sim->double_buffer);
    sem_destroy(sim->start_sync);
    free(sim->start_sync);
    sem_destroy(sim->finish_sync);
    free(sim->finish_sync);
    free(sim->should_continue);
}
