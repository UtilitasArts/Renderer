#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "FixedPointMath.h"

int main(int, char**) {

    fp_t a = FP_FROM_DECIMAL(1, 5);
    fp_print(a);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* Window{ SDL_CreateWindow(
      "Hello Window", 800, 300, 0
    ) };
    SDL_GetWindowSurface(Window);
    SDL_UpdateWindowSurface(Window);

    bool IsRunning = true;
    SDL_Event Event;
    while (IsRunning) {
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_EVENT_QUIT) {
                IsRunning = false;
            }
        }
    }

    SDL_DestroyWindow(Window);
    SDL_Quit();


    return 0;
}