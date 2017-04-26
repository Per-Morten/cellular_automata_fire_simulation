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

cpu_cell**
create_grid(const size_t rows,
            const size_t columns);
void
destroy_grid(cpu_cell** cells,
             const size_t count);
