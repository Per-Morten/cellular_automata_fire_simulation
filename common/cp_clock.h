#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum
    {
        cp_time_unit_nanoseconds,
        cp_time_unit_microseconds,
        cp_time_unit_milliseconds,
        cp_time_unit_seconds,
        cp_time_unit_minutes,
        cp_time_unit_hours,
    } cp_time_unit;

    typedef void* cp_time_point;

    cp_time_point
    cp_time_point_create();

    void
    cp_time_point_destroy(cp_time_point time_point);

    void
    cp_clock_now(cp_time_point time_point);

    float
    cp_clock_difference(const cp_time_point lhs,
                        const cp_time_point rhs,
                        cp_time_unit unit);

#ifdef __cplusplus
}
#endif
