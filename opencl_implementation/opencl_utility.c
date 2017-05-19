#include <opencl_utility.h>
#include <stdint.h>

int32_t
get_device_id(cl_device_type type,
              cl_device_id* out_device,
              cl_platform_id* out_platform)
{
    // Undefined behavior if there are more than 256 platforms,
    // however that should not be an issue.
    cl_uint id_count;
    cl_platform_id platform_ids[1 << 8];
    cl_int status = clGetPlatformIDs(1 << 8, platform_ids, &id_count);
    if (status != CL_SUCCESS || id_count == 0)
    {
        CP_WARN("Failed to get platform ids, error: %s, id_count: %u",
                get_error(status), id_count);
        return CP_FAILURE;
    }

    for (cl_uint i = 0; i < id_count; ++i)
    {
        status = clGetDeviceIDs(platform_ids[i], type, 1,
                                out_device, NULL);
        *out_platform = platform_ids[i];
        if (status == CL_SUCCESS)
        {
            break;
        }
    }

    if (out_device == NULL)
    {
        CP_WARN("Could not set device_id, error: %s",
                get_error(status));
        return CP_FAILURE;
    }

    return CP_SUCCESS;
}

int32_t
create_command_queue(cl_platform_id platform,
                     cl_context context,
                     cl_device_id device_id,
                     cl_command_queue* out_queue)
{
    char buffer[1024];
    cl_int error = clGetPlatformInfo(platform,
                                     CL_PLATFORM_VERSION,
                                     sizeof(buffer),
                                     buffer,
                                     NULL);

    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not get platform info, error: %s",
                get_error(error));
        return CP_FAILURE;
    }
   
    int32_t version[2] = {0};
    sscanf(buffer,"OpenCL %d.%d", &version[0], &version[1]);

#ifdef _MSC_VER

    *out_queue = clCreateCommandQueue(context, device_id, 0, &error);

#else
    *out_queue = clCreateCommandQueueWithProperties(context, device_id, NULL, &error);
#endif

    if (error != CL_SUCCESS)
    {
        CP_WARN("Could not create command queue, error: %s",
                get_error(error));
        return CP_FAILURE;
    }



    return CP_SUCCESS;
}


int32_t
setup_opencl(cl_device_id* out_device_id,
             cl_context* out_context,
             cl_command_queue* out_command_queue,
             cl_device_type device_type,
             info_verbosity verbosity)
{
    cl_platform_id platform_id;
    int32_t result = get_device_id(device_type,
                                   out_device_id,
                                   &platform_id);
    if (result != CP_SUCCESS)
    {
        return CP_FAILURE;
    }

    if (verbosity & info_verbosity_platform)
    {
        print_platform_info(platform_id);
    }

    if (verbosity & info_verbosity_device)
    {
        print_device_info(*out_device_id);
    }

    cl_int status;
    *out_context = clCreateContext(NULL, 1, out_device_id,
                                   NULL, NULL, &status);
    if (status != CL_SUCCESS)
    {
        CP_WARN("Could not create context, error: %s",
                get_error(status));
        return CP_FAILURE;
    }

    status = create_command_queue(platform_id, *out_context,
                                  *out_device_id, out_command_queue);

    return CP_SUCCESS;
}

void
cleanup_opencl(cl_context context,
               cl_command_queue command_queue)
{
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
}

///////////////////////////////////////////////////////////////////////////////
/// Taken from:
/// https://github.com/HandsOnOpenCL/Exercises-Solutions/blob/master/Exercises/C_common/err_code.h
///////////////////////////////////////////////////////////////////////////////
const char*
get_error(const cl_int status)
{
    switch (status)
    {
        case CL_SUCCESS:
            return "CL_SUCCESS";
        case CL_DEVICE_NOT_FOUND:
            return "CL_DEVICE_NOT_FOUND";
        case CL_DEVICE_NOT_AVAILABLE:
            return "CL_DEVICE_NOT_AVAILABLE";
        case CL_COMPILER_NOT_AVAILABLE:
            return "CL_COMPILER_NOT_AVAILABLE";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case CL_OUT_OF_RESOURCES:
            return "CL_OUT_OF_RESOURCES";
        case CL_OUT_OF_HOST_MEMORY:
            return "CL_OUT_OF_HOST_MEMORY";
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case CL_MEM_COPY_OVERLAP:
            return "CL_MEM_COPY_OVERLAP";
        case CL_IMAGE_FORMAT_MISMATCH:
            return "CL_IMAGE_FORMAT_MISMATCH";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case CL_BUILD_PROGRAM_FAILURE:
            return "CL_BUILD_PROGRAM_FAILURE";
        case CL_MAP_FAILURE:
            return "CL_MAP_FAILURE";
        case CL_MISALIGNED_SUB_BUFFER_OFFSET:
            return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
            return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case CL_INVALID_VALUE:
            return "CL_INVALID_VALUE";
        case CL_INVALID_DEVICE_TYPE:
            return "CL_INVALID_DEVICE_TYPE";
        case CL_INVALID_PLATFORM:
            return "CL_INVALID_PLATFORM";
        case CL_INVALID_DEVICE:
            return "CL_INVALID_DEVICE";
        case CL_INVALID_CONTEXT:
            return "CL_INVALID_CONTEXT";
        case CL_INVALID_QUEUE_PROPERTIES:
            return "CL_INVALID_QUEUE_PROPERTIES";
        case CL_INVALID_COMMAND_QUEUE:
            return "CL_INVALID_COMMAND_QUEUE";
        case CL_INVALID_HOST_PTR:
            return "CL_INVALID_HOST_PTR";
        case CL_INVALID_MEM_OBJECT:
            return "CL_INVALID_MEM_OBJECT";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case CL_INVALID_IMAGE_SIZE:
            return "CL_INVALID_IMAGE_SIZE";
        case CL_INVALID_SAMPLER:
            return "CL_INVALID_SAMPLER";
        case CL_INVALID_BINARY:
            return "CL_INVALID_BINARY";
        case CL_INVALID_BUILD_OPTIONS:
            return "CL_INVALID_BUILD_OPTIONS";
        case CL_INVALID_PROGRAM:
            return "CL_INVALID_PROGRAM";
        case CL_INVALID_PROGRAM_EXECUTABLE:
            return "CL_INVALID_PROGRAM_EXECUTABLE";
        case CL_INVALID_KERNEL_NAME:
            return "CL_INVALID_KERNEL_NAME";
        case CL_INVALID_KERNEL_DEFINITION:
            return "CL_INVALID_KERNEL_DEFINITION";
        case CL_INVALID_KERNEL:
            return "CL_INVALID_KERNEL";
        case CL_INVALID_ARG_INDEX:
            return "CL_INVALID_ARG_INDEX";
        case CL_INVALID_ARG_VALUE:
            return "CL_INVALID_ARG_VALUE";
        case CL_INVALID_ARG_SIZE:
            return "CL_INVALID_ARG_SIZE";
        case CL_INVALID_KERNEL_ARGS:
            return "CL_INVALID_KERNEL_ARGS";
        case CL_INVALID_WORK_DIMENSION:
            return "CL_INVALID_WORK_DIMENSION";
        case CL_INVALID_WORK_GROUP_SIZE:
            return "CL_INVALID_WORK_GROUP_SIZE";
        case CL_INVALID_WORK_ITEM_SIZE:
            return "CL_INVALID_WORK_ITEM_SIZE";
        case CL_INVALID_GLOBAL_OFFSET:
            return "CL_INVALID_GLOBAL_OFFSET";
        case CL_INVALID_EVENT_WAIT_LIST:
            return "CL_INVALID_EVENT_WAIT_LIST";
        case CL_INVALID_EVENT:
            return "CL_INVALID_EVENT";
        case CL_INVALID_OPERATION:
            return "CL_INVALID_OPERATION";
        case CL_INVALID_GL_OBJECT:
            return "CL_INVALID_GL_OBJECT";
        case CL_INVALID_BUFFER_SIZE:
            return "CL_INVALID_BUFFER_SIZE";
        case CL_INVALID_MIP_LEVEL:
            return "CL_INVALID_MIP_LEVEL";
        case CL_INVALID_GLOBAL_WORK_SIZE:
            return "CL_INVALID_GLOBAL_WORK_SIZE";
        case CL_INVALID_PROPERTY:
            return "CL_INVALID_PROPERTY";

        default:
            return "UNKNOWN ERROR";
    }
    return "UNKNOWN ERROR";
}
