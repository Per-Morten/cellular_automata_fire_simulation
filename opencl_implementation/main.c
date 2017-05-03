#include <stdio.h>
#include <cp_sdl.h>

#include <cp_common.h>
#include <opencl_utility.h>

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
