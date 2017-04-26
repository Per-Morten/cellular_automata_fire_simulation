#pragma once
#include <cp_sdl.h>
#include <cpu_cell.h>

void
draw(cp_sdl_api* api,
     const cpu_cell** cells,
     const size_t rows,
     const size_t columns);

void
update_cell_color(cpu_cell** cells,
                  const size_t rows,
                  const size_t columns);

void
update_cell_temprature(cpu_cell** read_cells,
                       cpu_cell** write_cells,
                      const size_t rows,
                      const size_t colums);

cpu_cell**
create_grid(const size_t rows,
            const size_t columns);

cpu_cell**
copy_grid(cpu_cell** destination,
          cpu_cell** source,
          const size_t rows,
          const size_t columns);
void
destroy_grid(cpu_cell** cells,
             const size_t count);
