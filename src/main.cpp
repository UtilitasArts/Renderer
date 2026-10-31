#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>

#include "Time.h"

bool is_running = false;
SDL_Window*   window    = NULL;
SDL_Renderer* renderer  = NULL;

uint32_t*    frame_buffer  = NULL;
SDL_Texture* frame_buffer_texture = NULL;

uint32_t window_width  = 800;
uint32_t window_height = 600;

bool InitializeWindow(void) {    
    if (!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr,"Unable to initialize SDL\n");
        return false;
    }    

  
    const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(1);
    window_width  = display_mode->w;
    window_height = display_mode->h;   


    //create a window
    window = SDL_CreateWindow("Renderer", window_width, window_height,0);
    if(!window){ 
    fprintf(stderr,"Error Creating Window"); 
    return false;
    }   

    //Create SDL renderer
    renderer = SDL_CreateRenderer(window, NULL);        
    if(!renderer){
        fprintf(stderr,"Error Creating Renderer\n");
        return false;
    }
    return true; 
}

void Setup(void) {
    // frame_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
    frame_buffer         = new uint32_t[window_width * window_height];
    frame_buffer_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,window_width,window_height);
}


void DestroyWindow(void) {
    // free(frame_buffer);
    delete(frame_buffer);
    SDL_DestroyTexture(frame_buffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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


void Update(void)
{
    
}

void render_frame_buffer(void) {
    SDL_UpdateTexture(frame_buffer_texture,NULL,frame_buffer,(int)window_width * sizeof(uint32_t));
    SDL_RenderTexture(renderer,frame_buffer_texture,NULL,NULL);
}


void clear_frame_buffer(uint32_t color){
    for(int y = 0; y < window_height;y++){
        for(int x = 0; x < window_width; x++)
        {
            frame_buffer[ (window_width*y) + x] = color;
        }
    }
}


void Render(void) {
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    SDL_RenderClear(renderer);

    render_frame_buffer();
    clear_frame_buffer(0xFFFFFF00);

    SDL_RenderPresent(renderer);
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

    is_running =InitializeWindow();    
    return 0;    
}