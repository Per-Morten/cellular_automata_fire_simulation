#include <cp_clock.h>
#include <cmath>
#include <chrono>

using clock_type = std::chrono::high_resolution_clock;
using time_point_type = std::chrono::time_point<clock_type>;

cp_time_point
cp_time_point_create()
{
    auto value = new time_point_type();
    return value;
}

void
cp_time_point_destroy(cp_time_point time_point)
{
    auto tmp = reinterpret_cast<time_point_type*>(time_point);
    delete tmp;
}

void
cp_clock_now(cp_time_point time_point)
{
    time_point_type* tmp = reinterpret_cast<time_point_type*>(time_point);
    *tmp = clock_type::now();
}

float
cp_clock_difference(cp_time_point lhs,
                     cp_time_point rhs,
                     cp_time_unit unit)
{
    const auto lhs_time = *reinterpret_cast<time_point_type*>(lhs);
    const auto rhs_time = *reinterpret_cast<time_point_type*>(rhs);

    switch (unit)
    {
        case cp_time_unit_nanoseconds:
            return std::chrono::duration<float, std::chrono::nanoseconds::period>(lhs_time - rhs_time).count();
            break;
        case cp_time_unit_microseconds:
            return std::chrono::duration<float, std::chrono::microseconds::period>(lhs_time - rhs_time).count();
            break;
        case cp_time_unit_milliseconds:
            return std::chrono::duration<float, std::chrono::milliseconds::period>(lhs_time - rhs_time).count();
            break;
        case cp_time_unit_seconds:
            return std::chrono::duration<float, std::chrono::seconds::period>(lhs_time - rhs_time).count();
            break;
        case cp_time_unit_minutes:
            return std::chrono::duration<float, std::chrono::minutes::period>(lhs_time - rhs_time).count();
            break;
        case cp_time_unit_hours:
            return std::chrono::duration<float, std::chrono::hours::period>(lhs_time - rhs_time).count();
            break;
        default:
            break;
    }
    return NAN;

}
