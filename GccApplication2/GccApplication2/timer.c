#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t ticksMS = 0; //Contador de ms transcurridos

/* ---------- TIMER0 a 1?ms ---------- */
void timer_init(void)
{
    /* F_CPU = 16?MHz  
	   Prescaler 64  --> 16Mhz / 64 = 250?kHz -->  4?microsegundos x tick de CLK
        1?ms --> 250 ticks * 4 microsegundos ? OCR0A = 249                             
	*/
	//Timer en modo compare CTC
    TCCR0A = (1<<WGM01);
	//Prescaler en 64            
    TCCR0B = (1<<CS01) | (1<<CS00);
    OCR0A  = 249;
	//Habilita interrupciones por compare match
    TIMSK0 = (1<<OCIE0A);
	//Habilita interrupciones globales
    sei();
}

/* ---------- ISR única ---------- */
ISR(TIMER0_COMPA_vect)
{
    ++ticksMS; //Paso 1ms, sumo al contador

    extern void fsm_tick(void);
    clk_tick(); 
}
