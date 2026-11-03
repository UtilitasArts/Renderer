#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>

#include "Time.h"
#include "display.h"
#include "vector.h"

float fov_factor = 200.f;

const int32_t n_points = 9*9*9;
vec3_t cube_points[n_points];
vec2_t projected_points[n_points];

bool is_running = false;

void Setup(void) {
    // frame_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
    frame_buffer = new uint32_t[window_width * window_height];
    frame_buffer_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,window_width,window_height);

    uint32_t point_count = 0;

    for(int x = 0; x < 9; x++){
         for(int y = 0; y < 9; y++){
            for(int z = 0; z < 9; z++){                
                float xpos = -1.f + (2.f/8.f * (float)x);
                float ypos = -1.f + (2.f/8.f * (float)y);
                float zpos = -1.f + (2.f/8.f * (float)z);               

                vec3_t new_point = {.x=xpos, .y=ypos, .z=zpos};                  
                cube_points[point_count++] = new_point;                
            }            
        }   
    }



    // for(float x = -1.f; x < 1.f ; x += 2.f/9.f ){
    //     for(float y = -1.f ; y < 1.f ; y += 2.f/9.f ){
    //         for(float z = -1.f; z < 1.f ; z += 2.f/9.f ){
    //             vec3_t new_point = {x,y,z};                    
                
    //                 cube_points[point_count++] = new_point;    
    //         }            
    //     }
    // }

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

vec2_t Project(vec3_t point ){
    vec2_t projected_point = { 
    .x= fov_factor*point.x / point.z,
    .y= fov_factor*point.y / point.z
    } ;
    return projected_point;
}

void Render(void) { 

    draw_dots();
   for(uint32_t i = 0; i < n_points; i++){

        vec2_t projected_point = projected_points[i];                 
        draw_pixel(projected_point.x + window_width/2,projected_point.y+ window_height/2, 2,col_select(255,255,255,255) );
        // draw_rect(projected_point.x + window_width/2  ,projected_point.y+ window_height/2  ,4,4,col_select(255,255,255,255));
   } 
   
    render_frame_buffer();  
    clear_frame_buffer(0xFF000000);
    SDL_RenderPresent(renderer);
}


void Update(void) {
    for(int i = 0; i < n_points; i++){
        vec3_t point = cube_points[i];
        point.z += 2;

        vec2_t projected_point = Project(point);
        projected_points[i] = projected_point;
    }
    
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