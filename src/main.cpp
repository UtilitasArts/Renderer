#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>

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
    window_width  = display_mode->w ;
    window_height = display_mode->h;

    //create a window
    window = SDL_CreateWindow("Renderer", window_width, window_height,0);
    if(!window){ 
    fprintf(stderr,"Error Creating Window"); 
    return false;
    }   

    SDL_SetWindowFullscreen(window,true);

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
        for(int x = 0; x < window_width; x++) {

            frame_buffer[ (window_width*y) + x] = color;            
        }
    }
}

bool mod_dist(int32_t center, int32_t dist, int32_t x){      
    int32_t half_point = center/2;
    int32_t distance = half_point - std::abs(half_point - x %center);     
    return (distance < dist);       
}

uint32_t col_select(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha) {
    uint32_t color = blue+green*256+red*256*256+alpha*256*256*256;
    return color;
    
}

void draw_grid() {

        uint32_t color;         
        uint32_t cell_width = 400;  
        uint32_t sub_cell_width =   cell_width / 2;   
        uint32_t sub_sub_cell_width =   sub_cell_width / 2;    

        for(int y = 0; y < window_height;y++){
        for(int x = 0; x < window_width; x++) {
            
            color = 0xFF000000;           

            if( mod_dist(cell_width,3,y) || mod_dist(cell_width,3,x) ) {
               color = col_select(255,255,255,255);               
            }
            else if( mod_dist(sub_cell_width,2,y) || mod_dist(sub_cell_width,2,x) ) {
                color = col_select(150,150,150,255);  
            }
            else if( mod_dist(sub_sub_cell_width,1,y) || mod_dist(sub_sub_cell_width,1,x) ) {
                color = col_select(50,50,50,255);  
            }
            
            frame_buffer[ (window_width*y) + x] = color;            
        }
    }
}

void draw_grid_recursive() {
        int32_t color;     
        int32_t value;
        int32_t cell_width;         

        const int32_t max_cell_width = 400;  
        const int32_t subdivs = 4;    
        const int32_t linewidth = 1;        
    
        for(int y = 0; y < window_height;y++){
        for(int x = 0; x < window_width; x++) { 
            color = 0xFF000000;              

            for(int32_t i = 0; i < subdivs; i++) {  
                uint32_t powi = 1 << i;   
                value      = 255 / powi;  
                cell_width = max_cell_width / powi;       

                if(( mod_dist(cell_width,linewidth,y) || mod_dist(cell_width,linewidth,x)) ) {
                    color = col_select(value,value,value,255); 
                    break;                    
                }                      
            }

            frame_buffer[ (window_width*y) + x] = color;   
        }
    }
}

void Render(void) {
    // SDL_SetRenderDrawColor(renderer,255,0,0,255);
    // SDL_RenderClear(renderer);

    draw_grid_recursive();

    render_frame_buffer();

    clear_frame_buffer(0xFF000000);

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