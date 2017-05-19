#include <stdio.h>
#include <cp_sdl.h>

#include <cp_common.h>
#include <opencl_utility.h>

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
    cp_log_init();
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    setup_opencl(&device, &context, &queue,
                 CL_DEVICE_TYPE_CPU,
                 info_verbosity_platform |
                 info_verbosity_device);

    char kernel_filepath[] = "kernels/offset_test.cl";
    cl_program program;
    int32_t result = create_program(context, device, &program,
                                    kernel_filepath,
                                    "-Ikernels/cp_lib/ -Werror -cl-std=CL1.2");

    if (result == CP_SUCCESS)
    {
        cl_int error;
        cl_kernel offset_kernel = clCreateKernel(program,
                                                 "offset_test",
                                                 &error);

        size_t values_count = 16;
        int32_t* h_values = calloc(values_count, sizeof(int32_t));
        for (size_t i = 0; i < values_count; ++i)
        {
            h_values[i] = i;
        }
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

        cl_mem d_values = clCreateBuffer(context,
                                         CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                         sizeof(int32_t) * values_count,
                                         h_values,
                                         &error);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not create buffer, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clSetKernelArg(offset_kernel, 0, sizeof(cl_mem), &d_values);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not set kernel argument, error: %s",
                    get_error(error));
            goto cleanup;
        }

        size_t work_dim[2] = {2, 2};
        size_t offset[2] = {1, 1};
        error = clEnqueueNDRangeKernel(queue, offset_kernel,
                                       2, (const size_t*)&offset,
                                       work_dim,
                                       NULL, 0, NULL, NULL);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not enqueue kernel, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clFinish(queue);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not finish, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clEnqueueReadBuffer(queue, d_values, CL_TRUE, 0,
                                    sizeof(int32_t) * values_count,
                                    h_values, 0,
                                    NULL, NULL);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not enqueue read, error: %s",
                    get_error(error));
            goto cleanup;
        }

        cleanup:
        clReleaseKernel(offset_kernel);
        clReleaseMemObject(d_values);
        free(h_values);
        clReleaseProgram(program);
    }

    cleanup_opencl(context, queue);
    cp_log_shutdown();
    return 0;
}

