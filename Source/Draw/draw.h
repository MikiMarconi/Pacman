#ifndef __DRAW_H
#define __DRAW_H

#include "stdio.h"
#include "stdlib.h"
#include "GLCD/GLCD.h"
#include "math.h"
#include "Entities/entities.h"


#define PRIMARY_COLOR   Blue
#define SECONDARY_COLOR	Black
#define TEXT_COLOR White
#define NP_COLOR  0xFFFFFF 
#define PP_COLOR	Red
#define CELL_SIZE    9     

void draw_square(int mat_x, int mat_y, int map_x, int map_y, int dimension, int color);
void drawMap();
void DrawFilledCircle(int x, int y, int r, int color);
void draw_map(int map_x, int map_y);
void draw_life(int life_x, int life_y, int lifes);
void draw_circle(int mat_x, int mat_y,int map_x,int map_y, int r,int dimension, int color);
void DrawGhost(int centerX, int centerY, int size, int color);
void draw_ghost(int mat_x, int mat_y,int map_x, int map_y, int color);
#endif