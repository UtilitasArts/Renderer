#pragma once

#include <vector>
#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8 
#define N_CUBE_FACES (6*2) 

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

  //----------------------//
 // Dynamic Sized Meshes //
//======================//
struct mesh_t {
    std::vector<vec3_t> vertices; //dynamic array of vertices
    std::vector<face_t> faces;    //dynamic array of faces
    vec3_t rotation;              //rotation vector
};

extern mesh_t mesh;

void load_cube_mesh_data();
void load_obj_file_data(const char* filename);