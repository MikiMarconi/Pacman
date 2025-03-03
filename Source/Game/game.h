#ifndef 	__GAME_H
#define 	__GAME_H

#include "Entities/entities.h"
#include "Draw/draw.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "GLCD/GLCD.h"
#include "Draw/draw.h"
#include "stdio.h"
#include "stdbool.h"
#include "music/music.h"

#define MAP_X 3
#define MAP_Y 30
#define ROWS 31
#define COLS 26
#define RANDOM_PILLS	6
#define NP_SCORE 10
#define PP_SCORE 50
#define LIFE_POINT 1000
#define PAUSE_TEXT "PAUSE"
#define GOVER_TEXT "GAME OVER"
#define GVICTORY_TEXT "VICTORY!"
#define COUNTDOWN 60
#define GHOST_DELAY 5
typedef enum{START,UP,DOWN,RIGHT,LEFT} DIRECTION;
typedef enum{CHASE,FRIGHTENED} GHOST_STATE;
typedef struct {int i; int j;} coord;

uint32_t moltiplica_e_converti(float numero);
void game_init();
void check_position(int old_x,int old_y,int new_x, int new_y);
void update_score();
void game_pause();
void game_over();
void game_victory();
void ghost_update(int x,int y);
bool check_ghost();
void moveGhostGreedy(coord *ghostPos, coord pacManPos);
void draw_timer(int time);
void draw_lifecan(int lifes);
void draw_score(int score);

#endif