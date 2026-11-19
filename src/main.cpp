#include <stdio.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>
#include <numbers>
#include <vector>

#include "timer.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"

  //------------------//
 // Global Variables //
//==================//
Timer::TimeStepManager time_manager;
std::vector<triangle_t> triangles_to_render;

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

    load_cube_mesh_data();
    load_obj_file_data("RubberToyTriangulated");
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
//=========//
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
    mesh.rotation.y += (2*pi) / 10.0   * time_manager.fixed_dt; 
    mesh.rotation.z += (2*pi) / 30.0   * time_manager.fixed_dt; 

      //--------------------------------------//
     // Loop over all the faces or our mesh  // 
    //======================================// 
    for(int32_t i = 0; i < mesh.faces.size(); i++){
        face_t cube_face = mesh.faces[i];
        
          //----------------------------------------------------------------------//
         // Each face has vertex id's, We use those Id's to grab the vertex data // 
        //======================================================================// 
        vec3_t face_vertices[3];
        face_vertices[1] = mesh.vertices[cube_face.b -1];
        face_vertices[0] = mesh.vertices[cube_face.a -1];
        face_vertices[2] = mesh.vertices[cube_face.c -1];

        triangle_t projected_triangle;
          //-------------------------------------------------------------------//
         // Loop over all the vertices of our face and transform & project it // 
        //===================================================================//         
        for (int32_t j = 0; j < 3; j++){
              //--------------------//
             // Transform Vertices // 
            //====================// 
            vec3_t transformed_vertex = face_vertices[j];
            transformed_vertex    = vec3_rotate(transformed_vertex,mesh.rotation);
            transformed_vertex.z -= camera_position.z;
            
              //------------------//
             // Project Vertices // 
            //==================//
            vec2_t projected_point = Project(transformed_vertex);   

              //-----------------------------------------------//
             // Translate Projected Vertices To Center Screen // 
            //===============================================//
            projected_point.x  += window_width/2;
            projected_point.y  += window_height/2;
            
            projected_triangle.points[j] = projected_point;            
        }

        triangles_to_render.push_back(projected_triangle);    
    }  
}

  //--------//
 // Render //
//========//
void Render(void) { 
   draw_dots(); 
   
   for(int i = 0; i < triangles_to_render.size(); i++) {
        triangle_t triangle = triangles_to_render[i];
        uint32_t color = col_select(255,255,0,255);         
        draw_triangle(triangle, 1, color, true);
   }
   
   triangles_to_render.clear();

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