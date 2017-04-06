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

