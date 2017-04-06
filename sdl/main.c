#include <stdio.h>
#include <cp_sdl.h>

#include <cp_common.h>

int
main(CP_UNUSED int argc,
     CP_UNUSED char** argv)
{
    cp_log_init();
    cp_sdl_api sdl_api;
    int32_t result = cp_sdl_init(&sdl_api, "cpu implementation",
                                 640, 480);

    if (result == CP_SUCCESS)
    {
        cp_sdl_clear(&sdl_api);

        cp_sdl_present(&sdl_api);
    }

    cp_sdl_shutdown(&sdl_api);
    cp_log_shutdown();

    return 0;
}
