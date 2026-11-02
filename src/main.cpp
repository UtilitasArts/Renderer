#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>

#include "Time.h"
#include "display.h"

bool is_running = false;

void Setup(void) {
    // frame_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
    frame_buffer = new uint32_t[window_width * window_height];
    frame_buffer_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,window_width,window_height);
}

void ProcessInput(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type){
        case SDL_EVENT_QUIT:
            is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_ESCAPE)
            is_running = false;
            break;
    }    
}

void Render(void)
{
    // SDL_SetRenderDrawColor(renderer,0,0,0,255);
    // SDL_RenderClear(renderer);

    clear_frame_buffer(0xFF000000);

    draw_dots();
    // draw_rect(window_width/2, window_height/2,300,300,col_select(255,255,255,255));
    draw_pixel(window_width/2, window_height/2,5,col_select(255,0,0,255));

    draw_line(window_width/2,window_height/2,window_width/2 + 100,window_height/2 - 200, 5,col_select(255,0,0,255));

    render_frame_buffer();  

    SDL_RenderPresent(renderer);
}


void Update(void)
{
    
}

int main(void) {      
    is_running = InitializeWindow();
    Setup();

    while(is_running)
    {
        Time::Manager.TakeTimeStep();        
        while( Time::Manager.ShouldUpdate() )
        { 
            Time::Manager.IncTimeStep();  

            ProcessInput();
            Update();
            Render();
        }              
    }  
    
    DestroyWindow();

    return 0;    
}