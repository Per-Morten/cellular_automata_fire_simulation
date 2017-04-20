#include <cp_sdl.h>

#include <cp_common.h>

int32_t
cp_sdl_init(cp_sdl_api* out_api,
            const char* window_title,
            const int window_width,
            const int window_height)
{
    out_api->window = NULL;
    out_api->renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        goto failure;
    }

    out_api->window = SDL_CreateWindow(window_title,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       window_width,
                                       window_height,
                                       SDL_WINDOW_SHOWN);

    if (out_api->window == NULL)
    {
        goto failure;
    }

    out_api->renderer = SDL_CreateRenderer(out_api->window,
                                           -1,
                                           SDL_RENDERER_ACCELERATED);

    if (out_api->renderer == NULL)
    {
        goto failure;
    }

    return CP_SUCCESS;

failure:
    CP_WARN("SDL_Error: %s", SDL_GetError());
    SDL_DestroyRenderer(out_api->renderer);
    SDL_DestroyWindow(out_api->window);
    return CP_FAILURE;
}

void
cp_sdl_shutdown(cp_sdl_api* api)
{
    SDL_DestroyRenderer(api->renderer);
    SDL_DestroyWindow(api->window);
    SDL_Quit();
}

void
cp_sdl_clear(cp_sdl_api* api)
{
    SDL_RenderClear(api->renderer);
}

void
cp_sdl_present(cp_sdl_api* api)
{
    SDL_RenderPresent(api->renderer);
}

void
cp_sdl_render_cell(cp_sdl_api* api,
                   const cp_sdl_cell* cell)
{
    cp_sdl_color prev_color;
    SDL_GetRenderDrawColor(api->renderer,
                           &prev_color.r,
                           &prev_color.g,
                           &prev_color.b,
                           &prev_color.a);

    SDL_SetRenderDrawColor(api->renderer,
                           cell->border_color.r,
                           cell->border_color.g,
                           cell->border_color.b,
                           cell->border_color.a);

    // Draw "border rect" first.
    SDL_Rect rect =
    {
        .x = cell->x,
        .y = cell->y,
        .w = cell->w,
        .h = cell->h,
    };

    SDL_RenderFillRect(api->renderer, &rect);

    // Now to draw the actual rect.
    rect.x = cell->x + cell->border_width;
    rect.y = cell->y + cell->border_width;
    rect.w = cell->w - (cell->border_width * 2);
    rect.h = cell->h - (cell->border_width * 2);

    SDL_SetRenderDrawColor(api->renderer,
                           cell->color.r,
                           cell->color.g,
                           cell->color.b,
                           cell->color.a);

    SDL_RenderFillRect(api->renderer, &rect);

    SDL_SetRenderDrawColor(api->renderer,
                           prev_color.r,
                           prev_color.g,
                           prev_color.b,
                           prev_color.a);
}

bool
cp_sdl_handle_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYUP &&
             event.key.keysym.sym == SDLK_ESCAPE))
        {
            return false;
        }
    }
    return true;
}
