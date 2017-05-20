#pragma once
#include <cp_sdl.h>
#include <cp_common.h>
#include <cpu_cell.h>

///////////////////////////////////////////////////////////////////////////////
/// \brief Renders all cells pointed to by cells to the renderer within the
///        api.
///
/// \param api      the cp_sdl_api holding the renderer and window.
/// \param cells    the cells to render.
/// \param rows     the number of rows in cells.
/// \param columns  the number of columns in cells.
///////////////////////////////////////////////////////////////////////////////
void
draw(cp_sdl_api* api,
     const cpu_cell** cells,
     const size_t rows,
     const size_t columns);

///////////////////////////////////////////////////////////////////////////////
/// \brief Updates the colors to all the cells pointed to by cells.
///        The color is based on the temperature of the different cells.
///
/// \param cells    the cells whose colors is to be updated.
/// \param rows     the number of rows in cells.
/// \param columns  the number of columns in cells.
///////////////////////////////////////////////////////////////////////////////
void
update_cell_color(cpu_cell** cells,
                  const size_t rows,
                  const size_t columns);

///////////////////////////////////////////////////////////////////////////////
/// \brief Updates the temperature to all the cells pointed to by write_cells.
///        The temperature is based on the values within read_cells.
///
/// \param read_cells  the cells to read current temperatures from.
/// \param write_cells the cells to store the updated temperatures in.
/// \param rows        the number of rows in read_cells and write_cells.
/// \param columns     the number of columns in read_cesll and write_cells
///
/// \warning read_cells and write_cells may not alias,
///          i.e. point to the same objects.
///////////////////////////////////////////////////////////////////////////////
void
update_cell_temperature(cpu_cell** CP_RESTRICT read_cells,
                        cpu_cell** CP_RESTRICT write_cells,
                        const size_t rows,
                        const size_t colums);

///////////////////////////////////////////////////////////////////////////////
/// \brief Creates a cpu_cell grid with the dimensions indicated by rows and
///        columns.
///        The grid is actually one layer larger in each dimension, with
///        zeroed out memory, meaning that the user will not have to check
///        for going out of bounds +-1 in either row or column direction,
///        as long as 0 values does not affect calculations.
///
/// \param rows     the row dimension of the grid to create.
/// \param columns  the column dimension of the grid to create.
///
/// \note  A cpu_cell grid created with create_grid must be destroyed using
///        destroy_grid to ensure proper cleanup without leaks.
///////////////////////////////////////////////////////////////////////////////
cpu_cell**
create_grid(const size_t rows,
            const size_t columns);

///////////////////////////////////////////////////////////////////////////////
/// \brief Copies the grid pointed to by source, into the grid pointed to
///        by destination.
///
/// \param destination where to store the grid.
/// \param source the grid to copy.
/// \param rows the number of rows in source and destination.
/// \param columns the number of columns in source and destination.
///
/// \warning destination and source may not alias,
///          i.e. point to the same objects.
///////////////////////////////////////////////////////////////////////////////
cpu_cell**
copy_grid(cpu_cell** CP_RESTRICT destination,
          cpu_cell** CP_RESTRICT source,
          const size_t rows,
          const size_t columns);

///////////////////////////////////////////////////////////////////////////////
/// \brief Destroys the grid pointed to by cells, cleaning up its memory.
///
/// \param cells the grid to destroy.
/// \param rows the number of rows in the grid.
///////////////////////////////////////////////////////////////////////////////
void
destroy_grid(cpu_cell** cells,
             const size_t rows);
