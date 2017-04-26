#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <cp_clock.h>
#include <cp_common.h>

static cp_time_point g_log_clock_start;
static cp_time_point g_log_clock_current;

void
cp_log_init()
{
    g_log_clock_start = cp_time_point_create();
    g_log_clock_current = cp_time_point_create();
    cp_clock_now(g_log_clock_start);
}

void
cp_log_shutdown()
{
    cp_time_point_destroy(g_log_clock_start);
    cp_time_point_destroy(g_log_clock_current);
    g_log_clock_start = NULL;
    g_log_clock_current = NULL;
}

void
cp_log(FILE* file,
       const char* type,
       const char* filepath,
       const char* func,
       const int   line,
       const char* fmt,
       ...)
{
    cp_clock_now(g_log_clock_current);

    va_list args1;
    va_start(args1, fmt);
    va_list args2;
    va_copy(args2, args1);
    size_t size = 1 + vsnprintf(NULL, 0, fmt, args1);
    char* buffer = malloc(size);

    va_end(args1);
    vsnprintf(buffer, size, fmt, args2);
    va_end(args2);

    float time_diff = cp_clock_difference(g_log_clock_current,
                                          g_log_clock_start,
                                          cp_time_unit_milliseconds);

    const char* filename = strrchr(filepath, '/');
    ++filename;

    fprintf(file, "[%.5f][%-5s]: %-10s: %-25s:%4d: %s\n",
            time_diff, type,
            filename, func,
            line, buffer);

    fflush(file);

    free(buffer);
}
