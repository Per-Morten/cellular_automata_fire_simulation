#pragma once
#include <CL/cl.h>
#include <stddef.h>
#include <cp_common.h>

///////////////////////////////////////////////////////////////////////////////
/// \brief Simple enum to indicate the verbosity level to use when setting up
///        opencl.
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    info_verbosity_platform = 1 << 0,
    info_verbosity_device = 1 << 1,
} info_verbosity;

///////////////////////////////////////////////////////////////////////////////
/// \brief Setups up opencl with the device type indicated by device type,
///        also creates a context and command queue.
///
/// \param device_type the type of device to be used.
/// \param out_device_id pointer to the variable to store the device id in.
/// \param out_context pointer to the variable to store context in.
/// \param out_command_queue pointer to the variable to store
///        the command queue in.
///
/// \return CP_SUCCESS on success, CP_FAILURE otherwise.
///
/// \note in case of failure, all memory allocated in function is deallocated,
///       and none of the parameters will be usable.
///////////////////////////////////////////////////////////////////////////////
int32_t
setup_opencl(cl_device_id* out_device_id,
             cl_context* out_context,
             cl_command_queue* out_command_queue,
             cl_device_type device_type,
             info_verbosity verbosity);

///////////////////////////////////////////////////////////////////////////////
/// \brief Cleans up the context and command queue handed in as parameters.
///
/// \param context the context to clean up.
/// \param command_queue the command_queue to clean up.
///////////////////////////////////////////////////////////////////////////////
void
cleanup_opencl(cl_context context,
               cl_command_queue command_queue);

///////////////////////////////////////////////////////////////////////////////
/// \brief Checks whether or not the status is an opencl error, in which case
///        it returns a string containing the error type.
///        returns NULL on success.
///
/// \note  pointer is to stack allocated memory, and must not be freed.
///
/// \param status the cl status to check.
///////////////////////////////////////////////////////////////////////////////
const char*
get_error(const cl_int status);

///////////////////////////////////////////////////////////////////////////////
/// \brief Prints certain device related info of the device indicated by
///        device_id.
///
/// \param device_id the id to print info of.
///
/// \todo Add support for: CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE
///                        CL_DEVICE_PARENT_DEVICE
///                        CL_DEVICE_PARTITION_PROPERTIES
///                        CL_DEVICE_PARTITION_TYPE
///////////////////////////////////////////////////////////////////////////////
void
print_device_info(const cl_device_id device_id);

///////////////////////////////////////////////////////////////////////////////
/// \brief Prints certain platform related info of the platform indicated by
///        platform_id.
///
/// \param platform_id the id to print info of.
///////////////////////////////////////////////////////////////////////////////
void
print_platform_info(const cl_platform_id platform_id);
