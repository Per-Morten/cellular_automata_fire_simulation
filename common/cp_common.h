#pragma once
#include <CL/cl.h>
#include <stdio.h>
#include <stdarg.h>

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to stop sdl from defining main, as that sometimes leads
///        to issues.
///////////////////////////////////////////////////////////////////////////////
#undef main

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to stop warnings on variables we know are unused.
///        mainly in the main function.
///////////////////////////////////////////////////////////////////////////////
#if defined(__GNUC__)
#define CP_UNUSED __attribute__((unused))
#else
#define CP_UNUSED
#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief Restrict macro allowing for C99 use of the keyword restrict with
///        microsoft compilers.
///////////////////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#define CP_RESTRICT __restrict
#else
#define CP_RESTRICT restrict
#endif

#if defined(_MSC_VER)
#define CP_PUSH_WARNING_DEPRECATED \
_Pragma("warning(push)")           \
_Pragma("warning(disable:4996)")
#else
#define CP_PUSH_WARNING_DEPRECATED \
_Pragma("GCC diagnostic push")     \
_Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#endif

#if defined(_MSC_VER)
#define CP_POP_WARNING \
_Pragma("warning(pop)")
#else
#define CP_POP_WARNING \
_Pragma("GCC diagnostic pop")
#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to log unrecoverable errors.
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt.
///
/// \warning Will terminate the program.
///////////////////////////////////////////////////////////////////////////////
#define CP_ERROR(fmt, ...) \
cp_log(stdout, "ERROR", __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__); \
exit(EXIT_FAILURE);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to log recoverable errors and warn about behavior.
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt.
///////////////////////////////////////////////////////////////////////////////
#define CP_WARN(fmt, ...) \
cp_log(stdout, "WARN", __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used to log general info.
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt.
///////////////////////////////////////////////////////////////////////////////
#define CP_INFO(fmt, ...) \
cp_log(stdout, "INFO", __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);

///////////////////////////////////////////////////////////////////////////////
/// \brief Used for debug log, i.e. not on by default.
///
/// \param fmt The format of the message to be logged.
/// \param ... va_list containing arguments to fmt.
///////////////////////////////////////////////////////////////////////////////
#ifdef CP_DEBUG_ON
#define CP_DEBUG(fmt, ...) \
cp_log(stdout, "DEBUG", __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__);
#else
#define CP_DEBUG(fmt, ...)
#endif

///////////////////////////////////////////////////////////////////////////////
/// \brief Constant value used to indicate failure of a function to do it's
///        work.
///////////////////////////////////////////////////////////////////////////////
#define CP_FAILURE 0

///////////////////////////////////////////////////////////////////////////////
/// \brief Constant value used to indicate success of a function to do it's
///        work.
///////////////////////////////////////////////////////////////////////////////
#define CP_SUCCESS 1

///////////////////////////////////////////////////////////////////////////////
/// \brief Function used for logging to the specified output.
///
/// \warning DO NOT use this function, use the macros CP_INFO & CP_ERROR
///          instead!
///////////////////////////////////////////////////////////////////////////////
void
cp_log(FILE* file,
       const char* type,
       const char* filepath,
       const char* func,
       const int line,
       const char* fmt,
       ...);

///////////////////////////////////////////////////////////////////////////////
/// \brief Initializes the logging system.
///        cp_log_shutdown must also be called, to release memory used by
///        the logging system.
///////////////////////////////////////////////////////////////////////////////
void
cp_log_init();

///////////////////////////////////////////////////////////////////////////////
/// \brief Shuts down the logging system, releasing all its memory.
///////////////////////////////////////////////////////////////////////////////
void
cp_log_shutdown();
