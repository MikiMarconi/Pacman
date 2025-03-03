#include "draw.h"  

extern char map[ROWS][COLS];
extern int randompills[RANDOM_PILLS];
extern int current_pill;
extern int current_x;
extern int current_y;
extern coord randompillpos[RANDOM_PILLS];
extern coord ghost_coord;
extern coord pac_coord;
extern coord current_ghost;
coord tel_l;
coord tel_r;


// draw rectangle
void LCD_FillRect(uint16_t m_x0, uint16_t m_y0, uint16_t m_x1, uint16_t m_y1, uint16_t color) {
  uint16_t i;
  for (i = m_y0; i <= m_y1; i++) {
    LCD_DrawLine(m_x0, i, m_x1, i, color);
  }
}

// draw square wrapper
void draw_square(int mat_x, int mat_y, int map_x, int map_y, int dimension, int color){
    LCD_FillRect((mat_y * dimension) +map_x, (mat_x * dimension)+map_y, ((mat_y + 1) * dimension -1)+map_x, ((mat_x + 1) * dimension) +map_y , color);
}

// draw circle wrapper
void draw_circle(int mat_x, int mat_y,int map_x,int map_y, int r,int dimension, int color){
    DrawFilledCircle((mat_y*dimension)+4+map_x,(mat_x*dimension)+4+map_y,r,color);
}

// draw circle
void DrawFilledCircle(int x, int y, int r, int color) {
    int16_t x0 = x, y0 = y;
    int16_t x1, y1;
    int16_t dx, dy;
    int16_t d;  
    x1 = 0;
    y1 = r;
    d = 3 - 2 * r;  
    while (x1 <= y1) {
        LCD_DrawLine(x0 - x1, y0 - y1, x0 + x1, y0 - y1, color); 
        LCD_DrawLine(x0 - x1, y0 + y1, x0 + x1, y0 + y1, color); 
        LCD_DrawLine(x0 - y1, y0 - x1, x0 + y1, y0 - x1, color); 
        LCD_DrawLine(x0 - y1, y0 + x1, x0 + y1, y0 + x1, color); 
        if (d < 0) {
            d += 4 * x1 + 6;
        } else {
            d += 4 * (x1 - y1) + 10;
            y1--;
        }
        x1++;
        if (x1 <= y1) {
            LCD_DrawLine(x0 - x1, y0 - y1, x0 + x1, y0 - y1, color); 
            LCD_DrawLine(x0 - x1, y0 + y1, x0 + x1, y0 + y1, color); 
        } else {
            break;
        }
    }
}

// draw map
void draw_map(int map_x, int map_y) {
	int i,j,count=0;
	volatile int k=0;
    for ( i = 0; i < ROWS; i++) {
        for ( j = 0; j < COLS; j++) {
            switch (map[i][j]) {
                case '.':  
					draw_square(i,j,map_x,map_y,CELL_SIZE,SECONDARY_COLOR);
					if(k<RANDOM_PILLS && randompills[k]==count){
						randompillpos[k].i=i;
						randompillpos[k].j=j;
						map[i][j]='.';
						draw_circle(i,j,map_x,map_y,1,CELL_SIZE,NP_COLOR);
						k++;
					}else{
                    draw_circle(i,j,map_x,map_y,1,CELL_SIZE,NP_COLOR);
					}
                    count++;
                    break;
				case ' ':
                    draw_square(i,j,map_x,map_y,CELL_SIZE,SECONDARY_COLOR);
					break;
				case 'p':
                current_x=i;
                current_y=j;
				pac_coord.i=i;
				pac_coord.j=j;
				draw_square(i,j,map_x,map_y,CELL_SIZE,SECONDARY_COLOR);
				draw_pacman(j,i,map_x,map_y,0);
					break;
				case 'g':
					draw_square(i,j,map_x,map_y,CELL_SIZE,SECONDARY_COLOR);
					ghost_coord.i=i;
					ghost_coord.j=j;
					current_ghost.i=i;
					current_ghost.j=j;
				break;
				case 'r':
					tel_r.i=i;
					tel_r.j=j;
				draw_square(i,j,map_x,map_y,CELL_SIZE,SECONDARY_COLOR);
				break;
				case 'l':
					tel_l.i=i;
					tel_l.j=j;
				draw_square(i,j,map_x,map_y,CELL_SIZE,SECONDARY_COLOR);
				break;
            }
        }
    }
	current_pill=count;
}

void draw_life(int life_x, int life_y, int lifes){
	int i;
	for(i=0;i<3;i++){
		draw_square(0,i,life_x,life_y,CELL_SIZE,SECONDARY_COLOR);
	}
	for(i=0;i<lifes;i++){
		draw_pacman(0,i,life_x,life_y,4);
	}
}

void draw_ghost(int mat_x, int mat_y,int map_x, int map_y, int color){
    DrawGhost((mat_y*CELL_SIZE)+4+ map_x,(mat_x*CELL_SIZE)+4+map_y,9,color);
}

void DrawGhost(int centerX, int centerY, int size, int color) {
    int halfSize = size / 2;
    int top = centerY - halfSize;  
    int left = centerX - halfSize;  
    LCD_DrawLine(left, top, left + size - 1, top, 0);
    LCD_DrawLine(left, top + 1, left + size - 1, top + 1, 0);
    LCD_DrawLine(left + 3, top + 2, left + 5, top + 2, color);
    LCD_DrawLine(left + 2, top + 3, left + 6, top + 3, color);
    LCD_DrawLine(left + 1, top + 4, left + 7, top + 4, color);
    LCD_DrawLine(left + 1, top + 5, left + 7, top + 5, color);
    LCD_DrawLine(left + 1, top + 6, left + 1, top + 6, color); 
    LCD_DrawLine(left + 4, top + 6, left + 4, top + 6, color);
    LCD_DrawLine(left + 7, top + 6, left + 7, top + 6, color); 
    LCD_DrawLine(left + 1, top + 7, left + 1, top + 7, color);
    LCD_DrawLine(left + 4, top + 7, left + 4, top + 7, color); 
    LCD_DrawLine(left + 7, top + 7, left + 7, top + 7, color); 
    LCD_DrawLine(left, top + 8, left + size - 1, top + 8, 0);
}





	



