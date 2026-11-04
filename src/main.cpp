#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>
#include <numbers>

#include "timer.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

  //------------------//
 // Global Variables //
//==================//
Timer::TimeStepManager time_manager;
triangle_t triangles_to_render[N_MESH_FACES];
  //--------//
 // Camera //
//========//
vec3_t camera_position = {0,0,-5};
vec3_t cube_rotation   = {0,0,0};
float  fov_factor      = 640.f;

bool is_running = false;

  //-------//
 // Setup //
//=======//
void Setup(void) {    
    frame_buffer = new uint32_t[window_width * window_height];
    frame_buffer_texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,window_width,window_height);
    
    time_manager.Start();    
}

  //--------//
 // Inputs //
//========//
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

  //---------//
 // Project //
//========//
vec2_t Project(vec3_t point ){
    vec2_t projected_point = { 
    .x= fov_factor * point.x / point.z,
    .y= fov_factor * point.y / point.z
    };
    
    return projected_point;
}

  //--------//
 // update //
//========//
void Update(void) {
    using namespace std::numbers; 
    // cube_rotation.x += (2*pi) / 10.0  * time_manager.fixed_dt;  
    cube_rotation.y += (2*pi) / 10.0   * time_manager.fixed_dt; 
    // cube_rotation.z += (2*pi) / 3.0   * time_manager.fixed_dt; 

      //------------------------------------------------//
     // Loop over all the faces or our cubed-mesh      // 
    //================================================// 
    for(int32_t i = 0; i < N_MESH_FACES; i++){
        face_t mesh_face = mesh_faces[i];
        
        vec3_t face_vertices[3];
        face_vertices[0] = mesh_vertices[mesh_face.a -1];
        face_vertices[1] = mesh_vertices[mesh_face.b -1];
        face_vertices[2] = mesh_vertices[mesh_face.c -1];

        triangle_t projected_triangle;

        for (int32_t j = 0; j < 3; j++){
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex    = vec3_rotate(transformed_vertex,cube_rotation);
            transformed_vertex.z -= camera_position.z;

            vec2_t projected_point = Project(transformed_vertex);
            
            projected_point.x  += window_width/2;
            projected_point.y  += window_height/2;
            
            projected_triangle.points[j] = projected_point;
        }

        triangles_to_render[i] = projected_triangle;
    }  
}

  //--------//
 // Render //
//========//
void Render(void) { 
   draw_dots();
   for(int i = 0; i < N_MESH_FACES; i++) {
        triangle_t triangle = triangles_to_render[i];  
        uint32_t color = col_select(255,255,0,255); 
        
        draw_line(triangle.points[0].x,
                  triangle.points[0].y,
                  triangle.points[1].x,
                  triangle.points[1].y, 4, col_select(255,255,0,255));

        draw_line(triangle.points[1].x,
                  triangle.points[1].y,
                  triangle.points[2].x,
                  triangle.points[2].y, 4,col_select(255,0,255,255));

        draw_line(triangle.points[2].x,
                  triangle.points[2].y,
                  triangle.points[0].x,
                  triangle.points[0].y, 4,col_select(0,255,255,255));
                  

        draw_pixel(triangle.points[0].x ,triangle.points[0].y, 5, color);
        draw_pixel(triangle.points[1].x ,triangle.points[1].y, 5, color);
        draw_pixel(triangle.points[2].x ,triangle.points[2].y, 5, color); 
   }

    render_frame_buffer();  
    clear_frame_buffer(0xFF000000);
    SDL_RenderPresent(renderer);
}

  //---------------//
 // main function //
//===============//
int main(void) {      
    is_running = InitializeWindow();
    Setup();              
    while(is_running){          
        time_manager.MeasureTimeStep();
        while(time_manager.IncTimeStep()){ 
            ProcessInput();  
            Update();                       
            Render();   
        }   
    }

    DestroyWindow(); 
    time_manager.log();
    return 0;    
}