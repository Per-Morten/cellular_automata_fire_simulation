#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include <cp_sdl.h>
#include <cp_common.h>
#include <cp_clock.h>
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

            cells[x][y].border_width = 0;
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
                       const float* temperatures,
                       const int32_t* fuel)
{
    for (size_t row = 0; row < CELLS_PER_ROW; ++row)
    {
        for (size_t col = 0; col < CELLS_PER_COL; ++col)
        {
            const size_t index = (CELLS_PER_COL + 2) * (row + 1) + col + 1;

            cells[row][col].color.r = (temperatures[index] > UCHAR_MAX)
                                    ? UCHAR_MAX
                                    : (uint8_t)temperatures[index];

            cells[row][col].color.b = (uint8_t)(fuel[index] * UCHAR_MAX / max_initial_fuel);
        }
    }
}

void create_temperatures(size_t count,
                         float** out_temperature,
                         int32_t** out_fuel)
{
    *out_temperature = malloc(count * sizeof(float));
    *out_fuel = malloc(count * sizeof(int32_t));
    memset(*out_temperature, 0, count * sizeof(float));
    memset(*out_fuel, 0, count * sizeof(int32_t));

    for (size_t row = 0; row < CELLS_PER_ROW; ++row)
    {
        for (size_t col = 0; col < CELLS_PER_COL; ++col)
        {
            int index = (CELLS_PER_COL + 2) * (row + 1) + col + 1;

            (*out_temperature)[index] = rand() % (int32_t)max_initial_temperature;
            (*out_fuel)[index] = rand() % max_initial_fuel;
        }
    }
}

#undef main

int
main(CP_UNUSED int argc,
     CP_UNUSED char** argv)
{
    cp_time_point stop = cp_time_point_create();
    cp_time_point start = cp_time_point_create();
    cp_clock_now(start);

    srand(time(NULL));
    cp_log_init();
    cp_sdl_api sdl_api;
    int32_t result = cp_sdl_init(&sdl_api, "opencl implementation",
                                 window_width, window_height);

    if (result != CP_SUCCESS)
    {
        cp_log_shutdown();
        return -1;
    }

    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    result = setup_opencl(&device, &context, &queue,
                          CL_DEVICE_TYPE_CPU,
                          info_verbosity_platform |
                          info_verbosity_device);

    if (result != CP_SUCCESS)
    {
        cp_log_shutdown();
        return -1;
    }


    char kernel_filepath[] = "kernels/cellular_automata_fire.cl";
    cl_program program;
    result = create_program(context, device, &program,
                            kernel_filepath,
                            "-Ikernels/cp_lib/ -Werror -cl-std=CL1.2");

    if (result != CP_SUCCESS)
    {
        cp_log_shutdown();
        return -1;
    }

    cl_int error;

    cl_kernel update_kernel = clCreateKernel(program,
                                             "update_temperature",
                                             &error);

    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not create update kernel, error: %s",
                get_error(error));
        goto cleanup;
    }

    const int32_t cell_count = (CELLS_PER_COL + 2) * (CELLS_PER_ROW + 2);

    float* h_read_temperatures = NULL;
    int32_t* h_read_fuel = NULL;

    create_temperatures(cell_count,
                        &h_read_temperatures,
                        &h_read_fuel);

    cl_mem d_read_temperatures = clCreateBuffer(context,
                                                CL_MEM_READ_WRITE |
                                                CL_MEM_COPY_HOST_PTR |
                                                CL_MEM_HOST_READ_ONLY,
                                                sizeof(float) * cell_count,
                                                h_read_temperatures,
                                                &error);

    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not create d_read_temperatures, error: %s",
                get_error(error));
        goto cleanup;
    }


    cl_mem d_read_fuel = clCreateBuffer(context,
                                        CL_MEM_READ_WRITE |
                                        CL_MEM_COPY_HOST_PTR |
                                        CL_MEM_HOST_READ_ONLY,
                                        sizeof(int32_t) * cell_count,
                                        h_read_fuel,
                                        &error);

    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not create d_read_fuel, error: %s",
                get_error(error));
        goto cleanup;
    }

    cl_mem d_write_temperatures = clCreateBuffer(context,
                                                 CL_MEM_READ_WRITE |
                                                 CL_MEM_HOST_NO_ACCESS |
                                                 CL_MEM_COPY_HOST_PTR,
                                                 sizeof(float) * cell_count,
                                                 h_read_temperatures,
                                                 &error);
    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not create d_write_temperature, error: %s",
                get_error(error));
        goto cleanup;
    }

    cl_mem d_write_fuel = clCreateBuffer(context,
                                         CL_MEM_READ_WRITE |
                                         CL_MEM_HOST_NO_ACCESS |
                                         CL_MEM_COPY_HOST_PTR,
                                         sizeof(int32_t) * cell_count,
                                         h_read_fuel,
                                         &error);
    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not create d_write_fuel, error: %s",
                get_error(error));
        goto cleanup;
    }

    error  = clSetKernelArg(update_kernel, 0, sizeof(cl_mem), &d_read_temperatures);
    error |= clSetKernelArg(update_kernel, 1, sizeof(cl_mem), &d_read_fuel);
    error |= clSetKernelArg(update_kernel, 2, sizeof(cl_mem), &d_write_temperatures);
    error |= clSetKernelArg(update_kernel, 3, sizeof(cl_mem), &d_write_fuel);

    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not create set kernel args, error: %s",
                get_error(error));
        goto cleanup;
    }

    cp_sdl_cell** grid = create_draw_grid();
    bool should_continue = true;

    while (should_continue)
    {
        should_continue = cp_sdl_handle_events();

        size_t work_dim[2] = { CELLS_PER_ROW, CELLS_PER_COL };
        size_t offset[2] = {1, 1};
        error = clEnqueueNDRangeKernel(queue,
                                       update_kernel, 2,
                                       (const size_t*)&offset,
                                       work_dim,
                                       NULL, 0, NULL, NULL);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not enqueue kernel, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clEnqueueCopyBuffer(queue, d_write_temperatures, d_read_temperatures,
                                    0, 0, cell_count * sizeof(float),
                                    0, NULL, NULL);

        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not enqueue copy temperatures, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clEnqueueCopyBuffer(queue, d_write_fuel, d_read_fuel,
                                    0, 0, cell_count * sizeof(int32_t),
                                    0, NULL, NULL);

        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not enqueue copy fuel, error: %s",
                    get_error(error));
            goto cleanup;
        }

        update_cell_color(grid, h_read_temperatures, h_read_fuel);
        cp_sdl_clear(&sdl_api);
        draw_grid(&sdl_api, grid);
        cp_sdl_present(&sdl_api);

        error = clFinish(queue);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not finish queue, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clEnqueueReadBuffer(queue, d_read_temperatures, CL_FALSE,
                                    0, cell_count * sizeof(float),
                                    h_read_temperatures,
                                    0, NULL, NULL);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not enqueue read temperatures, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clEnqueueReadBuffer(queue, d_read_fuel, CL_FALSE,
                                    0, cell_count * sizeof(int32_t),
                                    h_read_fuel,
                                    0, NULL, NULL);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not enqueue read fuel, error: %s",
                    get_error(error));
            goto cleanup;
        }

        error = clFinish(queue);
        if (error != CL_SUCCESS)
        {
            CP_WARN("Could not finish queue, error: %s",
                    get_error(error));
            goto cleanup;
        }

        SDL_Delay(30);

    }

    cp_clock_now(stop);

    float delta = cp_clock_difference(stop, start, cp_time_unit_seconds);
    CP_INFO("Time: %.5f", delta);

    cleanup:
    clReleaseKernel(update_kernel);

    clReleaseMemObject(d_read_temperatures);
    clReleaseMemObject(d_read_fuel);
    clReleaseMemObject(d_write_temperatures);
    clReleaseMemObject(d_write_fuel);

    clReleaseProgram(program);

    destroy_draw_grid(grid);
    free(h_read_temperatures);
    free(h_read_fuel);

    cleanup_opencl(context, queue);
    cp_sdl_shutdown(&sdl_api);
    cp_log_shutdown();
    return 0;
}

