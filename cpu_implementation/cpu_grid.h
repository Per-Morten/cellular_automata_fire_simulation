#pragma once
#include <cp_sdl.h>
#include <cp_common.h>
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
update_cell_temperature(cpu_cell** CP_RESTRICT read_cells,
                        cpu_cell** CP_RESTRICT write_cells,
                        const size_t rows,
                        const size_t colums);

cpu_cell**
create_grid(const size_t rows,
            const size_t columns);

cpu_cell**
copy_grid(cpu_cell** CP_RESTRICT destination,
          cpu_cell** CP_RESTRICT source,
          const size_t rows,
          const size_t columns);

void
destroy_grid(cpu_cell** cells,
             const size_t rows);
