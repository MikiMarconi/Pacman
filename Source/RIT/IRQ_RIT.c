/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "RIT.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int downrit=0;
extern DIRECTION current_direction;
extern NOTE pacman_intro[];
extern NOTE ghost_frightened[];
extern NOTE pacman_death[];
extern GHOST_STATE current_state;
extern bool game_status;
extern coord current_ghost;
extern int current_x;
extern int current_y;
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1
volatile int IntrocurrentNote=0;
volatile int GhostcurrentNote=0;
volatile int DeathcurrentNote=0;

#define DEBOUNCE_CYCLES 5  // Numero di cicli del RIT da attendere prima di aggiornare la direzione

static int debounce_counter_joystick = 0; // Contatore per il debounce del joystick

// Funzione di gestione dell'interrupt RIT
void RIT_IRQHandler(void) {
	disable_RIT();
    LPC_RIT->RICTRL |= 0x1;
    // Gestione dei pulsanti separata
    if(downrit >= 1) { 
        if((LPC_GPIO2->FIOPIN & (1 << 10)) == 0) {  // KEY1 pressed
            switch(downrit) {
                case 2: 
                    game_pause();
                    break;
                default:
                    break;
            }
            downrit++;
        }
        else {  // Button released
            downrit = 0;
            NVIC_EnableIRQ(EINT0_IRQn);  // Abilita gli interrupt dei pulsanti
            LPC_PINCON->PINSEL4 |= (1 << 20);  // Seleziona il pin per l'interrupt esterno
        }
    }
    if(current_ghost.i==current_x && current_ghost.j==current_y){
        if(check_ghost()){
            return;
        }
    }

    // Gestisci il debounce e i pulsanti separatamente
    if (debounce_counter_joystick == 0) {  // Solo se il contatore di debounce � 0, aggiorna la direzione
        // Esamina lo stato dei pulsanti (joystick)
        if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {  // UP
            if (current_direction != UP) {
                current_direction = UP;
                debounce_counter_joystick = DEBOUNCE_CYCLES;  // Imposta il contatore di debounce
            }
        }
        else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {  // RIGHT
            if (current_direction != RIGHT) {
                current_direction = RIGHT;
                debounce_counter_joystick = DEBOUNCE_CYCLES;
            }
        }
        else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {  // LEFT
            if (current_direction != LEFT) {
                current_direction = LEFT;
                debounce_counter_joystick = DEBOUNCE_CYCLES;
            }
        }
        else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {  // DOWN
            if (current_direction != DOWN) {
                current_direction = DOWN;
                debounce_counter_joystick = DEBOUNCE_CYCLES;
            }
        }
    }

    // Decrementa il contatore di debounce
    if (debounce_counter_joystick > 0) {
        debounce_counter_joystick--;
    }
   
    // GESTIONE MUSICA
        if(current_state==FRIGHTENED){
            if(GhostcurrentNote>=GHOST_NOTES){
                GhostcurrentNote=0;
            }
            static int ticks = 0;
            if(!isNotePlaying()){
                ++ticks;
                if(ticks == UPTICKS){
                    ticks = 0;
                    playNote(ghost_frightened[GhostcurrentNote++]);
                }
            }
    }else if(current_state==CHASE){
            if(IntrocurrentNote>=INTRO_NOTES){
            IntrocurrentNote=0;
            }
            static int ticks = 0;
            if(!isNotePlaying()){
                ++ticks;
                if(ticks == UPTICKS){
                    ticks = 0;
                    playNote(pacman_intro[IntrocurrentNote++]);
                }
            }
        }
				
        enable_RIT();
    }
	
/******************************************************************************
**                            End Of File
******************************************************************************/
