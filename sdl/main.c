#include <stdio.h>
#include <cp_sdl.h>

#include <cp_common.h>

#undef main

int
main(CP_UNUSED int argc,
     CP_UNUSED char** argv)
{
    cp_log_init();
    cp_sdl_api sdl_api;
    int32_t result = cp_sdl_init(&sdl_api, "cpu implementation",
                                 640, 480);

    cp_sdl_cell cell =
    {
        .x = 20,
        .y = 20,
        .h = 100,
        .w = 25,

        .border_width = 5,

        .color =
        {
            .r = 0xF0,
            .g = 0x00,
            .b = 0x00,
            .a = 0xFF,
        },

        .border_color =
        {
            .r = 0x00,
            .g = 0xF0,
            .b = 0x00,
            .a = 0xFF,
        },
    };

    if (result == CP_SUCCESS)
    {
        while (cp_sdl_handle_events())
        {
            cp_sdl_clear(&sdl_api);
            cp_sdl_render_cell(&sdl_api, &cell);

            cp_sdl_present(&sdl_api);
        }
    }

    cp_sdl_shutdown(&sdl_api);
    cp_log_shutdown();

    return 0;
}
