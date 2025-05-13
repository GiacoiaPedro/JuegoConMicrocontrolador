
#include "fsm.h"
#include "dict.h"
#include "lcd.h"
#include "keypad.h"
#include "timer.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define WORD_LEN        5
#define TIME_SHOW_WORD  2000u  
#define TIME_FINAL      5000u  

typedef enum {
    ST_IDLE,
    ST_SHOW_WORD,
    ST_TYPING,
    ST_CHECK,
    ST_VICTORY,
    ST_DEFEAT
} fsm_state_t;

static fsm_state_t state;

static char secretWord[WORD_LEN + 1];
static char typedWord [WORD_LEN + 1];
static uint8_t idx;               //indice de palabra actual
static uint8_t errorCount;
static uint8_t asciiDigits[3];    
static uint8_t asciiIndex;
static uint32_t t_ref;            //timestamp en ms
static char lastKeyPressed = '\0';
static char lastCharTyped;
static uint8_t first;
static uint32_t t_typing_start;
             


//Metodos auxiliares
static void lcd_idle(void)
{
    LCDclr();
    LCDGotoXY(0,0);
    LCDstring((uint8_t *)"Presione * para", 15);
    LCDGotoXY(0,1);
    LCDstring((uint8_t *)"    iniciar...", 14);
}
static void lcd_show_word(void)
{
    LCDclr();
    LCDGotoXY(0,0);
    LCDstring((uint8_t *)secretWord, WORD_LEN);
}
static void lcd_prepare_input(void)
{
    LCDclr();
    LCDGotoXY(0,1);           
}
static void lcd_line_clear(uint8_t line)
{
    LCDGotoXY(0,line);
    for (uint8_t i=0;i<16;i++) LCDsendChar(' ');
    LCDGotoXY(0,line);
}

static uint8_t digitsToAscii(const uint8_t d[], uint8_t count)
{
    uint16_t v = 0;
    for (uint8_t i=0;i<count;i++) v = v*10 + d[i];
    return (v <= 127) ? (uint8_t)v : 0;
}

void fsm_init(void)
{
    state = ST_IDLE;
    first = 1;
}

void clk_tick(void)
{
    char key;

    switch (state)
    {
    case ST_IDLE:
        if (first) {
            first = 0;
            lcd_idle();
            idx = errorCount = asciiIndex = 0;
        }
        if (keypad_scan(&key) && key == '*') {
            dict_seed(ticksMS);
            dict_get_random_word(secretWord);
            state = ST_SHOW_WORD;
            first = 1;
            return;
        }
        break;

    case ST_SHOW_WORD:
		if (first) {
			first = 0;
			lcd_show_word();
			t_ref = ticksMS;
		}
		if (ticksMS - t_ref >= TIME_SHOW_WORD) {
			lcd_prepare_input();
			// aquí acaba la muestra de la palabra: arrancamos el contador
			t_typing_start = ticksMS;
			// reutilizamos t_ref para medir el display final
			t_ref = ticksMS;
			state = ST_TYPING;
			lastKeyPressed = '\0';
			asciiIndex = 0;
			memset(asciiDigits, 0, sizeof(asciiDigits));
		}
		break;

    case ST_TYPING:
        if (keypad_scan(&key)) {
            if (key == lastKeyPressed){ 
				break;
			}
            lastKeyPressed = key;

            if (key == '#') {
                if (asciiIndex >= 2) {
                    lastCharTyped = (char)digitsToAscii(asciiDigits, asciiIndex);
                    asciiIndex = 0;
                    lcd_line_clear(1);
                    state = ST_CHECK;
                    first = 1;
                } else {                            
                    errorCount++;
                    asciiIndex = 0;
                    lcd_line_clear(1);
                    if (errorCount >= 3) { state = ST_DEFEAT; first = 1; }
                }
                break;
            }

            if (key >= '0' && key <= '9' && asciiIndex < 3) {
                asciiDigits[asciiIndex++] = key - '0';
                LCDsendChar(key);
            }
        } else {
            lastKeyPressed = '\0';
        }
        break;

    case ST_CHECK:
        if (first) { 
			first = 0; 
		}
        bool fallo = false;

        if (!((lastCharTyped>='A'&&lastCharTyped<='Z')||
              (lastCharTyped>='a'&&lastCharTyped<='z')))
            fallo = true;
        else if (lastCharTyped == secretWord[idx]) {
            typedWord[idx] = lastCharTyped;
            LCDGotoXY(idx,0);
            LCDsendChar(lastCharTyped);
            idx++;
            if (idx == WORD_LEN) { state = ST_VICTORY; first = 1; break; }
        } else {
            fallo = true;
        }

        if (fallo) {
            errorCount++;
            if (errorCount >= 3) { state = ST_DEFEAT; first = 1; break; }
        }

        lcd_line_clear(1);
        state = ST_TYPING;
        break;

    case ST_VICTORY:
    if (first) {
	    first = 0;
	    LCDclr();
	    LCDGotoXY(0,0);
	    LCDstring((uint8_t *)"VICTORIA!", 9);

	    // calculamos el tiempo total de tipiado
	    uint32_t elapsed_ms = ticksMS - t_typing_start;
	    uint32_t seconds = elapsed_ms / 1000;

	    char buf[16];
	    snprintf(buf, sizeof(buf), "Tiempo: %lu s", seconds);
	    LCDGotoXY(0,1);
	    LCDstring((uint8_t *)buf, strlen(buf));

	    // iniciamos el temporizador para la pantalla final
	    t_ref = ticksMS;
    }

    // tras TIME_FINAL ms volvemos a IDLE
    if (ticksMS - t_ref >= TIME_FINAL) {
	    state = ST_IDLE;
	    first = 1;
    }
    break;


    case ST_DEFEAT:
        if (first) {
            first = 0;
            LCDclr();
            LCDGotoXY(0,0);
            LCDstring((uint8_t *)"DERROTA!", 8);
            t_ref = ticksMS;
        }
        if (ticksMS - t_ref >= TIME_FINAL) {
            state = ST_IDLE; first = 1;
        }
        break;
    }
}
