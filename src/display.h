#pragma once
#include "SDL3/SDL.h"
#include "mesh.h"
#include "triangle.h"

extern SDL_Window*   window;
extern SDL_Renderer* renderer;

extern uint32_t*     frame_buffer;
extern SDL_Texture*  frame_buffer_texture;

extern uint32_t      window_width;
extern uint32_t      window_height;

bool InitializeWindow(void);
void DestroyWindow(void);
void clear_frame_buffer(uint32_t color);
void render_frame_buffer(void); 

bool mod_dist(int32_t center, int32_t dist, int32_t x);
uint32_t col_select(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha);

void draw_dots();
void draw_pixel(int32_t x, int32_t y, int32_t r, uint32_t color);

void draw_line_low_angle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t r, uint32_t color);
void draw_line_high_angle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t r, uint32_t color);
void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t r, uint32_t color);

void draw_triangle(triangle_t& triangle, int32_t r,uint32_t color, bool verts = false );
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, int32_t r, uint32_t color, bool verts = false);
void draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void draw_circle(int32_t posx, int32_t posy, int32_t r, int32_t width);

