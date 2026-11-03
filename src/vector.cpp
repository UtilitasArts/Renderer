#include <iostream>
#include <cmath>
#include "vector.h"

vec3_t vec3_rotate(vec3_t v, Axis axis, float angle){    
    if(angle == 0) return v;
    
    vec3_t rotated_vector{0,0,0};
    float sine   = sin(angle);
    float cosine = cos(angle);   

    if(axis == X_AXIS){
        rotated_vector = {
        .x = v.x,
        .y = v.y * cosine - v.z*sine,
        .z = v.y * sine   + v.z*cosine
        };
	}	 

    if(axis == Y_AXIS){
        rotated_vector = {
        .x = v.x * cosine - v.z * sine,
        .y = v.y,
        .z = v.x * sine   + v.z* cosine
        };
    }  

    if(axis == Z_AXIS){
        rotated_vector = {
        .x = v.x * cosine - v.y*sine,
        .y = v.x * sine   + v.y*cosine,
        .z = v.z        
        };
    }

    return rotated_vector;
}

vec3_t vec3_rotate(vec3_t v,vec3_t angles){
    vec3_t rotated_vector{0,0,0};
    rotated_vector = vec3_rotate(v             ,X_AXIS,angles.x);
    rotated_vector = vec3_rotate(rotated_vector,Y_AXIS,angles.y);
    rotated_vector = vec3_rotate(rotated_vector,Z_AXIS,angles.z);  
    return rotated_vector;
}

void vec3_print(vec3_t v){
    printf("%f, %f, %f \n", v.x, v.y, v.z);
}


vec3_t vec3_rotate_y(vec3_t v, float angle) {
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };
    return rotated_vector;
}
