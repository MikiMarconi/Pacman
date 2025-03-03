/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "Draw/draw.h"
#include "Entities/entities.h"
#include "Game/game.h"
#include "RIT/RIT.h"
#include "music//music.h"
#include "ADC/adc.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void)
{ 
  SystemInit();  												/* System Initialization (i.e., PLL)  */
  LCD_Initialization();
	CAN_Init();
	// Init timer
	LPC_SC -> PCONP |= (1 << 22);  // Turn on timer 2
	LPC_SC -> PCONP |= (1 << 23);  // Turn on timer 3
	// Init DAC (Speaker) 
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	BUTTON_init();
	init_timer(1, 0, 0, 3, moltiplica_e_converti(0.1666));
	init_timer(2, 0, 0, 3, moltiplica_e_converti(0.1666));
	init_RIT(moltiplica_e_converti(0.05));	
	game_init();
	// POWER MODE
	LPC_SC->PCON |= 0x1;																			
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
