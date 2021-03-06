#pragma once
#include <stdio.h>
#include <stdbool.h>
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
/// \brief Structure for representing color.
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} cp_sdl_color;

///////////////////////////////////////////////////////////////////////////////
/// \brief Structure for representing a graphical cell.
///
/// \note border_width goes towards center.
///////////////////////////////////////////////////////////////////////////////
typedef struct
{
    int x;
    int y;
    int w;
    int h;

    int border_width;
    cp_sdl_color color;
    cp_sdl_color border_color;
} cp_sdl_cell;

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

///////////////////////////////////////////////////////////////////////////////
/// \brief  Calls clear on the renderer within the api.
///
/// \param  api the cp_sdl_api containing the renderer to clear.
///////////////////////////////////////////////////////////////////////////////
void
cp_sdl_clear(cp_sdl_api* api);

///////////////////////////////////////////////////////////////////////////////
/// \brief  Calls present on the renderer within the api.
///
/// \param  api the cp_sdl_api containing the renderer to present.
///////////////////////////////////////////////////////////////////////////////
void
cp_sdl_present(cp_sdl_api* api);

///////////////////////////////////////////////////////////////////////////////
/// \brief  Renders the cell pointed to by cell.
///
/// \param  api the cp_sdl_api containing the renderer to render in.
/// \param  cell the cell to render.
///////////////////////////////////////////////////////////////////////////////
void
cp_sdl_render_cell(cp_sdl_api* api,
                   const cp_sdl_cell* cell);

///////////////////////////////////////////////////////////////////////////////
/// \brief  Checks if the program should continue to run, by polling events
///         from sdl.
///
/// \return true if the program should continue running, false if the user
///         has requested the program to end.
///////////////////////////////////////////////////////////////////////////////
bool
cp_sdl_handle_events();
