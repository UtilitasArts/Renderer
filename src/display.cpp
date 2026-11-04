#include "display.h"
#include <iostream>
#include <cmath>

SDL_Window*   window              = NULL;
SDL_Renderer* renderer            = NULL;
uint32_t*    frame_buffer         = NULL;
SDL_Texture* frame_buffer_texture = NULL;

uint32_t window_width  = 1920;
uint32_t window_height = 1080;

bool InitializeWindow(void)  {    
    if (!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr,"Unable to initialize SDL\n");
        return false;
    }

    const SDL_DisplayMode* display_mode = SDL_GetCurrentDisplayMode(1);
    // window_width  = display_mode->w ;
    // window_height = display_mode->h;

    //create a window
    window = SDL_CreateWindow("Renderer", window_width, window_height,0);
    if(!window){ 
    fprintf(stderr,"Error Creating Window"); 
    return false;
    }   

    // SDL_SetWindowFullscreen(window,true);

    //Create SDL renderer
    renderer = SDL_CreateRenderer(window, NULL);        
    if(!renderer){
        fprintf(stderr,"Error Creating Renderer\n");
        return false;
    }

    int interval = 0;
    SDL_GetRenderVSync(renderer, &interval);
    printf("VSync: %d\n", interval);
 
    return true; 
}

void DestroyWindow(void) {
    delete(frame_buffer);
    SDL_DestroyTexture(frame_buffer_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void clear_frame_buffer(uint32_t color) {
    for(int y = 0; y < window_height;y++){
        for(int x = 0; x < window_width; x++) {

            frame_buffer[ (window_width*y) + x] = color;            
        }
    }
}

void render_frame_buffer(void) {
    SDL_UpdateTexture(frame_buffer_texture,NULL,frame_buffer,(int)window_width * sizeof(uint32_t));
    SDL_RenderTexture(renderer,frame_buffer_texture,NULL,NULL);
}

bool mod_dist(int32_t center, int32_t dist, int32_t x) {      
    int32_t half_point = center/2;
    int32_t distance = half_point - std::abs(half_point - x %center);     
    return (distance < dist);       
}

uint32_t col_select(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha) {
    uint32_t color = blue+green*256+red*256*256+alpha*256*256*256;
    return color;    
}

void draw_dots() {
    int32_t value = 100;   
    int32_t cell_width = 10;
    int32_t centerx = window_width>>1;

    for(int x = cell_width; x <= centerx; x+= cell_width){ 
        for(int y = cell_width; y < window_height; y+= cell_width){        
            draw_pixel(x                     , y,1,col_select(value,value,value,255));
            draw_pixel(centerx + x-cell_width, y,1,col_select(value,value,value,255));                       
        }  
    }      
}

void draw_pixel(int32_t x, int32_t y, int32_t r, uint32_t color) {   
    if(r > 1)
    {
        for(int i = -r; i < r; i++){    
            int jmax = sqrt(r*r - i*i );   
            for(int j = -jmax; j < jmax; j++){            
                uint32_t curx = i + x ;
                uint32_t cury = j + y ;  

                if((window_width*cury) + curx <= window_width * window_height)
                {
                    frame_buffer[ (window_width*cury) + curx] = color;  
                }            
            }      
        } 
    }
    else 
    {
        if(x >= 0 && x < window_width && y >= 0 &&  y < window_height ){
            frame_buffer[(window_width*y) + x] = color;
        } 
    }   
}

void draw_line(int32_t x, int32_t y, int32_t x2, int32_t y2, int32_t r, uint32_t color) {
    uint32_t line_length = sqrt((x2-x)*(x2-x) + (y2-y)*(y2-y));
    float x_step = (float)(x2-x) / (float)line_length;
    float y_step = (float)(y2-y) / (float)line_length;

    for(int i = 0; i < line_length; i++) {    
        int32_t curx = x +  x_step * (float)i;
        int32_t cury = y +  y_step * (float)i;
        draw_pixel(curx, cury, r, color);      
    }
}

void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for(uint32_t i = 0; i < w; i++) {
        for(uint32_t j = 0; j < h; j++) {
            uint32_t curx =  i + x - w/2;
            uint32_t cury =  j + y - h/2;
            draw_pixel(curx,cury,1,color);
        }
    }
}

void draw_circle(int32_t posx, int32_t posy, int32_t r, int32_t width)  {
    for(int y = 0; y < window_height;y++){
            for(int x = 0; x < window_width; x++){              

            int32_t xoffset = abs(x - posx);
            int32_t yoffset = abs(y - posy);          
            
            uint32_t disc = sqrt(xoffset*xoffset + yoffset*yoffset);         
            
            if(disc <= r && disc >= r-width)
                frame_buffer[ (window_width*y) + x] = col_select(2,255,255,255);                 
        }                
    }
}



