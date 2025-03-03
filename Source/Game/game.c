#include "game.h"

int current_pill=240;
int current_x,current_y;
DIRECTION current_direction=START;
int current_life=1;
int current_score=1;
int ghost_start_delay=GHOST_DELAY;
int tmp_score=1;
int same=0;
bool game_status=false;
extern int countdown;
char score_text[5];
char countdown_text[5];
coord ghost_coord;
coord pac_coord;
coord current_ghost;
GHOST_STATE current_state;
int ghost_frightened_time=10;
volatile int n_pause=0;


char map[ROWS][COLS] = {
"##########################",
"#........................#",
"#.######################.#",
"#...........##...........#",
"#.#########.##.###.#####.#", 
"#.#########.##.###.#####.#", 
"#.#########.##.###.#####.#", 
"#.#####............#####.#", 
"#.#####.###.######.#####.#", 
"#.#####.###.######.#####.#", 
"#........................#", 
"#.###.##############.###.#", 
"#.###.##############.###.#", 
"#.....       p      .....#", 
"##### #####     ##### ####",
"l     #             #    r",
"##### #      g      # ####",
"##### ############### ####", 
"#........................#", 
"#.#########.##.#########.#", 
"#...........##...........#", 
"#.#########.############.#", 
"#.#########.############.#", 
"#.#########.############.#", 
"#.#########.############.#", 
"#...........##...........#", 
"###.#######.##.########.##",
"###.#######.##.########.##",
"###.........##..........##",
"##########################",
"##########################"
};


int randompills[RANDOM_PILLS];
int randompillspawn[RANDOM_PILLS];
coord randompillpos[RANDOM_PILLS];

void powerpillstime_generation(){
	int i,j,tmp;
	srand(time(NULL));
	for(i=0; i<RANDOM_PILLS;i++){
		randompillspawn[i]= 60-(rand() % 31);
	}
	for(i=0; i<RANDOM_PILLS;i++){
		for(j=i+1;j<RANDOM_PILLS;j++){
				if(randompillspawn[j]>randompillspawn[i]){
					tmp=randompillspawn[i];
					randompillspawn[i]=randompillspawn[j];
					randompillspawn[j]=tmp;
				}
		}
	}
}


// Power_pills position generation

void powerpills_positiongeneration(){
	int i,j,tmp;
	srand(time(NULL));
	for(i=0; i<RANDOM_PILLS;i++){
		randompills[i]= rand() % 241;
	}
	for(i=0; i<RANDOM_PILLS;i++){
		for(j=i+1;j<RANDOM_PILLS;j++){
				if(randompills[j]<randompills[i]){
					tmp=randompills[i];
					randompills[i]=randompills[j];
					randompills[j]=tmp;
				}
		}
	}
}

// Init mappa
void game_lcdinit(){
	LCD_Clear(PRIMARY_COLOR);
	LCD_FillRect(0,0,240,30,SECONDARY_COLOR);
	LCD_FillRect(0,30,3,309,SECONDARY_COLOR);
	LCD_FillRect(236,30,240,309,SECONDARY_COLOR);
	LCD_FillRect(0,310,240,320,SECONDARY_COLOR);
	draw_map(MAP_X,MAP_Y);
	draw_life(MAP_X,310,current_life);
	GUI_Text(103+MAP_X,136+MAP_Y, (uint8_t *)PAUSE_TEXT, Yellow, SECONDARY_COLOR);
	GUI_Text(0, 0, (uint8_t *)"SCORE ", TEXT_COLOR, SECONDARY_COLOR);
	GUI_Text(100, 0, (uint8_t *)"GAME OVER IN  ", TEXT_COLOR, SECONDARY_COLOR);

}

// Game Init
void game_init(){
	powerpills_positiongeneration();
	powerpillstime_generation();
	game_lcdinit();
	enable_RIT();
	while(1);
}


void check_position(int old_x,int old_y,int new_x, int new_y){
	if(current_pill==0){
		game_victory();
	}
	if(map[new_x][new_y]=='#'){
		return;
	}
	
	if(map[new_x][new_y]=='.'){
		play_npill();
		current_pill--;
		current_score+=NP_SCORE;
		tmp_score+=NP_SCORE;
		map[new_x][new_y]=' ';
	}else if(map[new_x][new_y]=='*'){
		play_ppill();
		current_pill--;
		current_score+=PP_SCORE;
		tmp_score+=PP_SCORE;
		map[new_x][new_y]=' ';
		current_state=FRIGHTENED;
		ghost_frightened_time=10;
	}
	
	if( tmp_score>=LIFE_POINT && current_life<3){
		current_life++;
		tmp_score-=LIFE_POINT;
		draw_life(MAP_X,310,current_life);
	}
	current_x=new_x;
	current_y=new_y;
	draw_square(old_x,old_y,MAP_X,MAP_Y,CELL_SIZE,SECONDARY_COLOR);
	draw_pacman(new_x,new_y,MAP_X,MAP_Y,current_direction);
}


void game_pause(){
	if(game_status==true){
		disable_timer(1);
		disable_timer(0);
		disable_timer(2);
		disable_timer(3);
		game_status=false;
		GUI_Text(103+MAP_X,136+MAP_Y, (uint8_t *)PAUSE_TEXT, Yellow, SECONDARY_COLOR);
	}else{
		game_status=true;
		enable_timer(1);
		enable_timer(0);
		enable_timer(2);
		sprintf(countdown_text, "%02d",countdown);
		GUI_Text(210, 0, (uint8_t *)countdown_text, TEXT_COLOR, SECONDARY_COLOR);
		GUI_Text(103+MAP_X,136+MAP_Y, (uint8_t *)PAUSE_TEXT, Black, SECONDARY_COLOR);
		if(n_pause==0){
			ghost_update(ghost_coord.i,ghost_coord.j);
			n_pause++;
		}
		
	}
}

void game_over(){
	disable_timer(1);
	disable_timer(2);
	disable_RIT();
	GUI_Text(86+MAP_X,136+MAP_Y, (uint8_t *)GOVER_TEXT, Red, SECONDARY_COLOR);
}

void game_victory(){
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	disable_timer(3);
	disable_RIT();
	GUI_Text(90+MAP_X,136+MAP_Y, (uint8_t *)GVICTORY_TEXT, Yellow, SECONDARY_COLOR);
}

void ghost_update(int x,int y){
	draw_ghost(x,y,MAP_X,MAP_Y,Green);
}

bool check_ghost(){
		if(current_state==CHASE){
			current_direction=START;
		draw_square(current_x,current_y,MAP_X,MAP_Y,CELL_SIZE,SECONDARY_COLOR);
		current_life--;
		draw_life(MAP_X,310,current_life);
		if(current_life<=0){
			game_over();
			return true;
		}else{
			current_x=pac_coord.i;
			current_y=pac_coord.j;
			ghost_start_delay=GHOST_DELAY;
			current_direction=START;
			draw_pacman(current_y,current_x,MAP_X,MAP_Y,0);
			current_ghost.i=ghost_coord.i;
			current_ghost.j=ghost_coord.j;
			ghost_update(ghost_coord.i,ghost_coord.j);
		}
		}else{			
			current_score+=100;
			ghost_start_delay=3;
			current_ghost.i=ghost_coord.i;
			current_ghost.j=ghost_coord.j;
			ghost_update(ghost_coord.i,ghost_coord.j);
			current_state=CHASE;
		}
	return false;
}


uint32_t moltiplica_e_converti(float numero) {
    // Moltiplica il numero per 25.000.000 e restituisci il risultato come uint32_t
    uint32_t risultato = (uint32_t)(numero * 25000000);

    // Restituisce il risultato come uint32_t, che è già in formato numerico
    return risultato;
}


void draw_score(int score){
	char tmp[5];
	sprintf(tmp,"%d",score);
	GUI_Text(50, 0, (uint8_t *)tmp, TEXT_COLOR, SECONDARY_COLOR);
}

void draw_timer(int time){
	char tmp[5];
		sprintf(tmp, "%02d",time);
		GUI_Text(210, 0, (uint8_t *)tmp, TEXT_COLOR, SECONDARY_COLOR);

}

void draw_lifecan(int lifes){
	draw_life(MAP_X,310,current_life);
}