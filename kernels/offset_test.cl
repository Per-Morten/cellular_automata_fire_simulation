#include <cp_lib_types.h>

__kernel void offset_test(__global int32_t* values)
{
    size_t x_id = get_global_id(0);
    size_t y_id = get_global_id(1);

    size_t total_offset = get_global_offset(0) + get_global_offset(1);
    size_t pos = (y_id * (get_global_size(0) + total_offset)) + x_id;

    printf("global size: %zu\n", get_global_size(0));

    printf("x: %zu, y: %zu, pos: %zu, val: %d\n",
           x_id, y_id, pos, values[pos]);

    //printf("index: %zu, value: %d\n", index, values[index]);
}
