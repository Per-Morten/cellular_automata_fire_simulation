#pragma once
#include <stdio.h>
#include <SDL2/SDL.h>

///////////////////////////////////////////////////////////////////////////////
/// \brief Wrapper for easy access to the most important variables when
///        discussing SDL.
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} cp_sdl_api;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Initializes the out_api, creating a window
///         and renderer that is stored in the cp_sdl_api.
///
/// \param  out_api out parameter, can't be NULL.
/// \param  window_title the title to display in the window bar.
/// \param  window_width the width of the window.
/// \param  window_height the height of the window.
///
/// \return CP_SUCCESS on success, CP_FAILURE otherwise,
///         Function cleans up memory on failure.
///////////////////////////////////////////////////////////////////////////////
int32_t
cp_sdl_init(cp_sdl_api* out_api,
            const char* window_title,
            const int window_width,
            const int window_height);

///////////////////////////////////////////////////////////////////////////////
/// \brief  Cleans up the variables from out_api, and quits SDL.
///
/// \param  api the cp_sdl_api to cleanup.
///////////////////////////////////////////////////////////////////////////////
void
cp_sdl_shutdown(cp_sdl_api* api);

