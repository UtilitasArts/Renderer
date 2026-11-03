#pragma once

struct vec2_t{
float x;
float y;
};

struct vec3_t{
float x;
float y;
float z;
};

enum Axis{
    X_AXIS = 0,
    Y_AXIS = 1,
    Z_AXIS = 2
};

vec3_t vec3_rotate(vec3_t v, Axis axis, float angle);
vec3_t vec3_rotate(vec3_t v, vec3_t angles);

void vec3_print(vec3_t v);

vec3_t vec3_rotate_y(vec3_t v, float angle);


