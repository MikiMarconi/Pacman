#include "music.h"

#include "../timer/timer.h"

#define NPILL_NNOTES 4
#define DEATH_NOTES 20
#define PPILL_NOTES 3

int current_npill_note=0;

NOTE pacman_intro[] = {
   	{c2, time_semicroma}, {e2, time_semicroma}, {g2, time_semicroma}, {e2, time_semicroma},
    {g2, time_semicroma}, {a2, time_semicroma}, {g2, time_semicroma}, {e2, time_semicroma},
    {c2, time_croma}, {pause, time_croma},
    {g2, time_semicroma}, {c3, time_semicroma}, {a2, time_semicroma}, {g2, time_semicroma},
    {e2, time_semicroma}, {g2, time_semicroma}, {a2, time_semicroma}, {c3, time_semicroma},
    {g2, time_croma}, {pause, time_croma}
};

NOTE pacman_death[] = {
    {g2, time_semicroma}, {f2, time_semicroma}, {e2, time_semicroma}, {d2, time_semicroma}, {c2, time_croma},
    {g2, time_semicroma}, {f2, time_semicroma}, {e2, time_semicroma}, {d2, time_semicroma}, {c2, time_croma},
    {f2, time_croma}, {g2, time_croma}, {a2, time_croma}, {pause, time_croma},
    {f2, time_semicroma}, {g2, time_semicroma}, {a2, time_semicroma}, {b2, time_semicroma},
    {c3, time_croma}, {pause, time_croma}
};

NOTE pacman_npill[] = {
    {c2, time_semicroma}, {pause, time_semicroma},
    {d2, time_semicroma}, {pause, time_semicroma}
};

NOTE pacman_ppill[] = {
    {c3, time_semicroma}, {c2, time_semicroma}, {g2, time_semicroma}
};

NOTE ghost_frightened[] = {
    {a2, time_croma}, {c2, time_croma},
    {d2, time_croma}, {e2, time_croma}
};


void playNote(NOTE note){
	if(note.freq != pause){
		reset_timer(0);
		init_timer(0,0,0,3, note.freq);
		enable_timer(0);
	}
	reset_timer(3);
	init_timer(3,0,0,7, note.duration);
	enable_timer(3);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM0->TCR != 0) || (LPC_TIM3->TCR != 0));
}

void play_npill(){
	int i;
	for(i=0;i<NPILL_NNOTES;i++){
		playNote(pacman_npill[i]);
	}
}

void play_death(){
	int i;
	for(i=0;i<DEATH_NOTES;i++){
		playNote(pacman_death[i]);
	}
}

void play_ppill(){
	int i;
	for(i=0;i<PPILL_NOTES;i++){

		playNote(pacman_ppill[i]);
	}
}
