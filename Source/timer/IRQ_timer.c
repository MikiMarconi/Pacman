/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "Game/game.h"
#include "Astar/Astar.h"
#include "CAN/CAN.h"
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/
volatile int i=0;
unsigned char ledval = 0xA5;
extern DIRECTION current_direction;
extern int current_x;
extern int current_y;
extern char map[ROWS][COLS];
extern DIRECTION direction;
extern coord tel_l;
extern coord tel_r;
extern int current_score;
extern int current_pill;
volatile int countdown=COUNTDOWN;
extern char score_text[5];
extern char countdown_text[5];
extern coord randompillpos[RANDOM_PILLS];
extern int randompillspawn[RANDOM_PILLS];
volatile int h=0;
extern coord current_ghost;
extern ghost_start_delay;
extern current_state;
extern ghost_frightened_time;
int count_time=6;
volatile int speed_change=30;
extern int current_life;


uint16_t SinTable[45] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER0_IRQHandler (void)
{
	static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	LPC_DAC->DACR = currentValue <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;

	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	count_time--;
	/// ROBA CHE ERA NELLO 0
	if(count_time<=0){
	if(ghost_start_delay!=0){
			ghost_start_delay--;
		}
		
		if(ghost_frightened_time!=0 && current_state==FRIGHTENED){
			ghost_frightened_time--;
		}
		if(countdown==0){
			game_over();
		}else{
			countdown--;
			CAN_TxMsg.data[3] = (current_score & 0xFF00)>>8;
			CAN_TxMsg.data[2] = (current_score & 0xFF);
			CAN_TxMsg.data[1] = (current_life & 0xFF);
			CAN_TxMsg.data[0] = countdown;
			CAN_TxMsg.len = 4;
			CAN_TxMsg.id = 2;
			CAN_TxMsg.format = STANDARD_FORMAT;
			CAN_TxMsg.type = DATA_FRAME;
			CAN_wrMsg (1, &CAN_TxMsg);               /* transmit message */

			
			if(h<RANDOM_PILLS & countdown==randompillspawn[h] ){
				if(map[randompillpos[h].i][randompillpos[h].j]==' '){
					current_pill++;
				}
				map[randompillpos[h].i][randompillpos[h].j]='*';
				draw_circle(randompillpos[h].i,randompillpos[h].j,MAP_X,MAP_Y,2,CELL_SIZE,PP_COLOR);
				h++;
				if(randompillspawn[h]==randompillspawn[h-1]){
					randompillspawn[h]--;
				}
				
			}
		}
		speed_change--;
		count_time=6;
	}
	
	//ROBA CHE ERA NELL 1
	if(LPC_TIM1->IR & 1) // MR0
	{ 
		switch(current_direction){
			case RIGHT:
				if(map[current_x][current_y]=='r'){
					check_position(current_x,current_y,tel_l.i,tel_l.j);
				}else{
					check_position(current_x,current_y,current_x,current_y+1);
				}
			break;
			case UP:
				check_position(current_x,current_y,current_x-1,current_y);
			break;
			case DOWN:
				check_position(current_x,current_y,current_x+1,current_y);
			break;
			case LEFT:
				if(map[current_x][current_y]=='l'){
					check_position(current_x,current_y,tel_r.i,tel_r.j);
				}else{
					check_position(current_x,current_y,current_x,current_y-1);
				}
			break;
			default:
				break;
		}
		
		if(ghost_start_delay==0){
			coord current_pac;
			current_pac.i=current_x;
			current_pac.j=current_y;
			draw_square(current_ghost.i,current_ghost.j,MAP_X,MAP_Y,CELL_SIZE,SECONDARY_COLOR);
			if(map[current_ghost.i][current_ghost.j]=='*'){
				draw_circle(current_ghost.i,current_ghost.j,MAP_X,MAP_Y,2,CELL_SIZE,PP_COLOR);
			}else if(map[current_ghost.i][current_ghost.j]=='.'){
				draw_circle(current_ghost.i,current_ghost.j,MAP_X,MAP_Y,1,CELL_SIZE,NP_COLOR);
			}
		}
		
		LPC_TIM1->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM1->IR & 2){ // MR1
		LED_Out(ledval);
		LPC_TIM1->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 4){ // MR2
		// your code	
		LPC_TIM1->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 8){ // MR3
		// your code	
		LPC_TIM1->IR = 8;			// clear interrupt flag 
	} 
	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
if(LPC_TIM2->IR & 1) // MR0
	{ 
		if(ghost_start_delay==0){
			
			coord current_pac;
			current_pac.i=current_x;
			current_pac.j=current_y;
			draw_square(current_ghost.i,current_ghost.j,MAP_X,MAP_Y,CELL_SIZE,SECONDARY_COLOR);
			if(map[current_ghost.i][current_ghost.j]=='*'){
				draw_circle(current_ghost.i,current_ghost.j,MAP_X,MAP_Y,2,CELL_SIZE,PP_COLOR);
			}else if(map[current_ghost.i][current_ghost.j]=='.'){
				draw_circle(current_ghost.i,current_ghost.j,MAP_X,MAP_Y,1,CELL_SIZE,NP_COLOR);
			}
			
			if(current_state==CHASE){
				if(speed_change<=0){
					init_timer(2, 0, 0, 3, moltiplica_e_converti(0.14));
				}
				aggiornaPercorsoFantasma(&current_ghost,current_pac);
				ghost_update(current_ghost.i,current_ghost.j);
				if(current_ghost.i==current_x && current_ghost.j==current_y){
					check_ghost();
    			}	
			}else if(current_state==FRIGHTENED && ghost_frightened_time>0){
				init_timer(2, 0, 0, 3, moltiplica_e_converti(0.25));
				aggiornaPercorsoFantasma(&current_ghost,trovaPosizioneLontanaDaPacman(current_pac));
				draw_ghost(current_ghost.i,current_ghost.j,MAP_X,MAP_Y,Blue);
				if(current_ghost.i==current_x && current_ghost.j==current_y){
					check_ghost();
    			}	
			}else if(current_state==FRIGHTENED && ghost_frightened_time==0){
				current_state=CHASE;
				init_timer(2, 0, 0, 3, moltiplica_e_converti(0.1666));
				ghost_update(current_ghost.i,current_ghost.j);
				if(current_ghost.i==current_x && current_ghost.j==current_y){
					check_ghost();
    			}	
			}
		}
		
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM2->IR & 2){ // MR1
		LED_On(0);
		LPC_TIM2->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 4){ // MR2
		// your code	
		LPC_TIM2->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM2->IR & 8){ // MR3
		// your code	
		LPC_TIM2->IR = 8;			// clear interrupt flag 
	} 
	return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void){
	disable_timer(0);
  	LPC_TIM3->IR = 1;			/* clear interrupt flag */
  	return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/

