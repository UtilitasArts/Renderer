#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>
#include <numbers>


#include "Time.h"

bool is_running = false;
SDL_Window*   window    = NULL;
SDL_Renderer* renderer  = NULL;

uint32_t*    frame_buffer  = NULL;
SDL_Texture* frame_buffer_texture = NULL;

uint32_t window_width  = 800;
uint32_t window_height = 600;

class ImageBuffer {
    public:
    ImageBuffer(uint32_t w, uint32_t h){
         buffer = new uint32_t[w * h];    
    } 
    ~ImageBuffer() {
        delete(buffer);
    }
    uint32_t* buffer  = NULL;
    bool drawn{false};
};

ImageBuffer* grid = nullptr;
ImageBuffer* dots = nullptr;
ImageBuffer* background = nullptr;


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
    frame_buffer = new uint32_t[window_width * window_height];
    grid         = new ImageBuffer(window_width,window_height);
    dots         = new ImageBuffer(window_width,window_height);
    background   = new ImageBuffer(window_width,window_height);

    frame_buffer_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,window_width,window_height);
}


void DestroyWindow(void) {
    // free(frame_buffer);
    delete(frame_buffer);
    delete(grid);
    delete(dots);
    delete(background);

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

uint32_t col_blend(uint32_t a, uint32_t b, float alpha){
    float ra = ((a & 0x00FF0000)>>16); 
    float ga = ((a & 0x0000FF00)>>8);
    float ba =  (a & 0x000000FF);  

    float rb = ((b & 0x00FF0000)>>16); 
    float gb = ((b & 0x0000FF00)>>8);
    float bb =  (b & 0x000000FF);  

    uint32_t blend = col_select((uint8_t)std::lerp(ra,rb,alpha),(uint8_t)std::lerp(ga,gb,alpha),(uint8_t)std::lerp(ba,bb,alpha),255);

    return blend;        
}

void draw_grid() {
    if(grid == nullptr) return;
    if(!grid->drawn)
    {
            int32_t color;     
            int32_t value;
            int32_t cell_width;      

            const int32_t subdivs = 6;   
            const int32_t min_cell_width = 10;
            const int32_t max_cell_width = min_cell_width << subdivs;   
            const int32_t linewidth = 1;        
        
            for(int y = 0; y < window_height;y++){
            for(int x = 0; x < window_width; x++) { 
                color = 0x00000000;              

                for(int32_t i = 0; i <= subdivs; i++) {       
                    value      = 255 >> i;  
                    cell_width = max_cell_width >> i;       

                    if(( mod_dist(cell_width,linewidth,y) || mod_dist(cell_width,linewidth,x)) ) {
                        color = col_select(value,value,value,value); 
                        break;                    
                    }                      
                }
                grid->buffer[ (window_width*y) + x] = color;                 
            }
        }
        grid->drawn = true;  
    }   
}

void draw_dots(){
    int32_t color;    
    int32_t value = 255;
    int32_t cell_width = 10;
    
    const int32_t linewidth = 1;   

    if(dots == nullptr) return;

    if(!dots->drawn){ 

        for(int y = 0; y < window_height;y++){
            for(int x = 0; x < window_width; x++){ 
                color = 0xFF000000;              

                if(( mod_dist(cell_width,linewidth,y) || mod_dist(cell_width,linewidth,x)) ) {
                        color = col_select(value,value,value,255);                                            
                }      
               
                dots->buffer[ (window_width*y) + x] = color;
                }  
        }

        dots->drawn = true;
    }

    for(int y = 0; y < window_height;y+= cell_width){
        for(int x = 0; x < window_width; x+= cell_width) {     
            
            frame_buffer[ (window_width*y) + x] = dots->buffer[ (window_width*y) + x];                 
        }
    }  
}

void draw_rectangle(int32_t posx, int32_t posy, int32_t w, int32_t h) {
    for(int y = 0; y < window_height;y++){
        for(int x = 0; x < window_width; x++){                       

            int32_t xoffset = abs(x - posx);
            int32_t yoffset = abs(y - posy);            
            
            if(xoffset <= w && yoffset < h){
                frame_buffer[ (window_width*y) + x] = col_select(255,255,255,255);                 
            }                
        }
    }
}

void draw_circle(int32_t posx, int32_t posy, int32_t r, int32_t width) {
    for(int y = 0; y < window_height;y++){
        for(int x = 0; x < window_width; x++){              

            int32_t xoffset = abs(x - posx);
            int32_t yoffset = abs(y - posy);          
            
            uint32_t disc = sqrt(xoffset*xoffset + yoffset*yoffset);         
            
            if(disc <= r && disc >= r-width)
                frame_buffer[ (window_width*y) + x] = col_select(255,255,255,255);                 
            }                
        }
}

void draw_background(){    
    if(background == nullptr) return;
    if(!background->drawn)
    {            
        using namespace std::numbers;

        for(int y = 0; y < window_height;y++){
            for(int x = 0; x < window_width; x++){                 

                float pos_fvalue = 1.f * (-std::cos(y*pi/window_height)+1) /2.f;
                float neg_fvalue = 1.f * (std::cos(y*pi/window_height)+1) /2.f;                        

                float fvalue = pos_fvalue * neg_fvalue;

                fvalue = (cos( (2*pi*y/window_height) - pi ) + 1.f ) /2.f;

                float rvalue  = (0.01f * fvalue) * 255.f;
                float gvalue  = (0.16f * fvalue)  * 255.f;
                float bvalue  = (0.2f  * fvalue)  * 255.f;

                uint32_t r = (uint32_t)rvalue;
                uint32_t g = (uint32_t)gvalue;
                uint32_t b = (uint32_t)bvalue;  
                
                background->buffer[ (window_width*y) + x] = col_select(r,g,b,255);            
            }
        }
        background->drawn = true;        
    }
}

void compose_grid_and_background()
{
    if(background->drawn && grid->drawn) {

        for(int y = 0; y < window_height;y++){
            for(int x = 0; x < window_width; x++) {             

            float value = ((grid->buffer[ (window_width*y) + x] & 0xFF000000)>>24)/255.f;         
      
            uint32_t prev_buffer = background->buffer[ (window_width*y) + x];            
            uint32_t color = col_blend(prev_buffer,col_select(255,255,255,255),value); 
            frame_buffer[ (window_width*y) + x] = color;
            }
        }
    }      
}

void Render(void) {
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);

    // draw_dots();  

    draw_background();
    draw_grid();
    compose_grid_and_background();

    draw_rectangle(window_width/2, window_height/2,100,100);
    // draw_circle(window_width/2, window_height/2,300,1);
    // draw_circle(window_width/2-600, window_height/2,300,1);
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
    
    DestroyWindow();

    return 0;    
}