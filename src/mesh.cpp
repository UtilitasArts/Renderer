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
    while(Folder.filename().string() != "Renderer"){
        Folder = Folder.parent_path();
    }
    
    fs::path FilePath = Folder/"assets"/ (std::string(filename) + ".obj");  
    // FilePath /= "assets";
    // FilePath /= 


    if(!FilePath.has_filename()) {
        std::cout << "No such file In Asset Folder: " << std::endl;       
        return;
    }
    else{
         std::cout << "File Name: " << FilePath.string() << std::endl;
    }


    std::string filepath = FilePath.string();



    // FILE* file;
    // file = fopen(filepath.c_str(), "r");
    // char line[1024];

    // while(fgets(line,1024,file)) {   
    //     if(strncmp(line,"v ",2) == 0)	{
    //         // printf("Line = %s", line);
    //         vec3_t vertex;
    //         sscanf(line,"v %f %f %f", &vertex.x,&vertex.y,&vertex.z); 
    //         mesh.vertices.push_back(vertex); 
    //     }
    //     if(strncmp(line,"f ",2) == 0)	{
    //         // printf("Line = %s", line);
    //         int vertex_indices[3];
    //         int texture_indices[3];
    //         int normal_indices[3];	

    //         sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",     
    //         &vertex_indices[0],&texture_indices[0],&normal_indices[0],
    //         &vertex_indices[1],&texture_indices[1],&normal_indices[1],
    //         &vertex_indices[2],&texture_indices[2],&normal_indices[2]
    //         );

    //         face_t face{ .a = vertex_indices[0],
    //                      .b = vertex_indices[1],
    //                      .c = vertex_indices[2]
    //                     };

    //         mesh.faces.push_back(face);
    //     }
    // }

    std::ifstream File;    
    File.open(FilePath);    
    std::string Type;  
    char line[512];

    while(File.getline(line,sizeof(line))) {   
                     
        if(strncmp(line,"v ",2) == 0)	{
            // printf("Line = %s", line);
            vec3_t vertex;
            sscanf(line,"v %f %f %f", &vertex.x,&vertex.y,&vertex.z); 
            mesh.vertices.push_back(vertex); 
        }
        else if(strncmp(line,"f ",2) == 0)	{
            // printf("Line = %s", line);
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];	

            sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",     
            &vertex_indices[0],&texture_indices[0],&normal_indices[0],
            &vertex_indices[1],&texture_indices[1],&normal_indices[1],
            &vertex_indices[2],&texture_indices[2],&normal_indices[2]
            );

            face_t face{ .a = vertex_indices[0],
                         .b = vertex_indices[1],
                         .c = vertex_indices[2]
                        };

            mesh.faces.push_back(face);
        }
    }

    File.close();
}


