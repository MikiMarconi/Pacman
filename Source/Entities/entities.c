#include "entities.h"

extern char map[ROWS][COLS];

void DrawPacman(int centerX, int centerY, int size, int direction);
void draw_pacman(int mat_x, int mat_y,int map_x, int map_y, int direction);

void DrawPacman(int centerX, int centerY, int size, int direction) {
    int r = size / 2;  // Raggio del cerchio
    int x, y;
    for (y = centerY - r; y <= centerY + r; y++) {
        for (x = centerX - r; x <= centerX + r; x++) {
            int dist = (int)sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
            if (dist <= r) {
                switch (direction) {
                    case UP:
                        if (y < centerY && x >= centerX - r / 2 && x <= centerX + r / 2) {
                            continue; 
                        }
                        break;
                    case DOWN:
                        if (y > centerY && x >= centerX - r / 2 && x <= centerX + r / 2) {
                            continue;  
                        }
                        break;
                    case LEFT:
                        if (x < centerX && y >= centerY - r / 2 && y <= centerY + r / 2) {
                            continue;  
                        }
                        break;
                    case RIGHT:
                        if (x > centerX && y >= centerY - r / 2 && y <= centerY + r / 2) {
                            continue;  
                        }
                        break;
                }

                LCD_DrawLine(x, y, x, y, Yellow); 
            }
        }
    }
}


void draw_pacman(int mat_x, int mat_y,int map_x, int map_y, int direction){
	DrawPacman((mat_y*CELL_SIZE)+4+ map_x,(mat_x*CELL_SIZE)+4+map_y,6,direction);
}

