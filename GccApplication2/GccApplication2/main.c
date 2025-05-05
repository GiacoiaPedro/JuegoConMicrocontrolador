/*
 * main.c 
 *
 * Created: 28/4/2025 18:19:21
 * Author : lisan
 */ 

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include "dict.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"
#include "fsm.h"




int main(void)
{	
	keypad_init();
	LCDinit();
	timer_init();
	fsm_init();	

	
    while (1) 
    {
		// Nada aqu�, todo se maneja por clk_tick() llamado en el Timer
		
	}
}

