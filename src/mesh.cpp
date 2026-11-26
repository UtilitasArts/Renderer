#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>
#include <SDL3/SDL.h>
#include <limits>
#include "mesh.h"
#include "vector.h"
#include "triangle.h"

vec3_t cube_vertices[N_CUBE_VERTICES]= {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

    //----------------------------------------------------------------------------//
   // Each face has vertex id's, We use those Id's to grab the vertex data       //
  // Keep in Mind that each Vertex is 1 higher than it's actual array version,  //
 // Because those start at 0.                                                  //
//============================================================================// 

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

mesh_t mesh{};

void load_cube_mesh_data(){
    for(int i = 0; i < N_CUBE_VERTICES; i++ ){
        mesh.vertices.push_back(cube_vertices[i]);

    }
    for(int i = 0; i < N_CUBE_FACES; i++ ){
        mesh.faces.push_back(cube_faces[i]);

    }
}

void load_obj_file_data(const char *filename) {
    namespace fs = std::filesystem;

    fs::path Folder = SDL_GetBasePath();
    while(Folder.parent_path().filename().string() != "Renderer"){
        Folder = Folder.parent_path();
    }
    
    fs::path FilePath = Folder.parent_path().string() + "/assets/" + std::string(filename) + ".obj";  

    if(!FilePath.has_filename()) {
        std::cout << "No such file In Asset Folder: " << std::endl;       
        return;
    }
    else{
         std::cout << "File Name: " << FilePath.filename().string() << std::endl;
    }

    std::ifstream File;    
    File.open(FilePath);
    

    // std::string Content;
    std::string Type;  

    // while(std::getline(File,Content)) {            
    //     File >> Type;

    //     if(Type == "v"){
    //         float x,y,z;
    //         File >> x >> y >> z;
    //         vec3_t vertex{.x = x, .y = y,.z = z};
    //         mesh.vertices.push_back(vertex);  
    //     }
    //     if(Type == "f"){
    //         int v1, b, c, v2, e, f, v3, h, i;

    //         File >> v1; File.ignore(1);
    //         File >> b;  File.ignore(1);
    //         File >> c;  File.ignore(1);
    //         File >> v2; File.ignore(1);
    //         File >> e;  File.ignore(1);
    //         File >> f;  File.ignore(1);
    //         File >> v3; File.ignore(1);
    //         File >> h;  File.ignore(1);
    //         File >> i;               
            
    //         face_t face{.a = v1, .b = v2, .c = v3};
    //         mesh.faces.push_back(face); 
    //     }   
    // } 
    

    while(true){        
        File >> Type;

        if(File.eof()) {
            return;
        }

        if(Type == "v"){
            float x,y,z;
            File >> x >> y >> z;
            vec3_t vertex{.x = x, .y = y,.z = z};
            mesh.vertices.push_back(vertex);  
        }
        if(Type == "f"){
            int v1, b, c, v2, e, f, v3, h, i;
            File >> v1; File.ignore(1);
            File >> b;  File.ignore(1);
            File >> c;  File.ignore(1);
            File >> v2; File.ignore(1);
            File >> e;  File.ignore(1);
            File >> f;  File.ignore(1);
            File >> v3; File.ignore(1);
            File >> h;  File.ignore(1);
            File >> i;               
            
            face_t face{.a = v1, .b = v2, .c = v3};
            mesh.faces.push_back(face); 
        }

        File.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    }

    File.close();
}


