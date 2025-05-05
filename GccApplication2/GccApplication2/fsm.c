
#include "fsm.h"
#include "dict.h"
#include "lcd.h"
#include "keypad.h"
#include "timer.h"


#include <ctype.h>
#include <string.h>


#define TIME_SHOW_WORD 2000u //ms
#define TIME_FINAL 5000 //ms



typedef enum{
	ST_IDLE,
	ST_TYPING,
	ST_SHOW_WORD,
	ST_CHECK,
	ST_VICTORY,
	ST_DEFEAT
} fsm_game;

static fsm_game state;
static char secretWord[6]; //Array de 6 caracteres ya que almacena la palabra de 5 caracteres + el caracter final /0
static uint8_t index;
static char lastCharTyped;
static uint8_t t_ref;
static char typedWord[6];  // Buffer para la palabra ingresada por el usuario
static uint8_t asciiDigits[3];  // Almacena los dígitos ingresados (0-9)
static uint8_t asciiIndex=0;    //index para el vector que almacena los valores ingresados por keypad que luego se transforman en ascii
static char dummyKey = '\0';
static char lastKeyPressed = '\0';

//Metodos auxiliares


static void lcd_idle(void)
{
	LCDclr();
	LCDGotoXY(0,0);
	char str1[] = "Presione * para";
	char str2[] = "iniciar...";
	LCDstring((uint8_t *)str1, strlen(str1));
	LCDGotoXY(0,1);
	LCDstring((uint8_t *)str2, strlen(str2));
}

uint8_t digitsToAscii(const uint8_t digits[], uint8_t count) {
	uint8_t result = 0;
	
	switch(count) {
		case 3: result = digits[0] * 100 + digits[1] * 10 + digits[2]; break;
		case 2: result = digits[0] * 10 + digits[1]; break;
		case 1: result = digits[0]; break;
		default: return 0; // Error
	}
	
	return (result <= 127) ? result : 0; // Solo ASCII estándar
}

void fsm_init(void){
	state = ST_IDLE;
	lcd_idle();
}

void clk_tick(void)
{
	
	char key;
	
	switch(state)
	{
	//Estado idle. Muestra "Presione * para comenzar..."
	case ST_IDLE:
		if(keypad_scan(&key) && key == '*'){
			state = ST_SHOW_WORD;			
		}
		break;

	case ST_SHOW_WORD:
		dict_seed(ticksMS);
		dict_get_random_word(secretWord);
		LCDclr();
		LCDGotoXY(0,0);
		LCDstring((uint8_t *) secretWord, 5);	//Outputs string to LCD			
		_delay_ms(2000);  // Espera de 2000 ms (2 segundos)
		// Preparar para el siguiente estado
		index = 0;
		memset(typedWord, 0, sizeof(typedWord));  // Limpiar buffer de entrada
		LCDclr();
		LCDGotoXY(0,0);                                          
		state = ST_TYPING;                        // Cambiar de estado
		break;
	

	case ST_TYPING:
		if(keypad_scan(&key)) {
			static char lastKeyPressed = '\0';

			if(key != lastKeyPressed) {
				lastKeyPressed = key;
				asciiDigits[asciiIndex] = key;
				asciiIndex++;
				asciiIndex = asciiIndex % 3;
				lastCharTyped = digitsToAscii((uint8_t*)asciiDigits, asciiIndex);
				LCDsendChar(key);

				if(key == '#') {
					asciiIndex = 0;
					LCDclr();
					state = ST_CHECK;
				}
			}
		}

		// MOVER AQUÍ PARA DETECTAR CUANDO SE SUELTA LA TECLA
		if(!keypad_scan(&dummyKey)) {
			lastKeyPressed = '\0';
		}
	break;

	
	case ST_CHECK:
	if (strcmp(secretWord, typedWord) == 0) {
		state = ST_VICTORY;
		t_ref = ticksMS;
		LCDclr();
		LCDGotoXY(0,0);
		LCDstring((uint8_t *)"CORRECTO!", 9);
		} else {
		state = ST_DEFEAT;
		t_ref = ticksMS;
		LCDclr();
		LCDGotoXY(0,0);
		LCDstring((uint8_t *)"INCORRECTO!", 11);
	}
	break;
	
	case ST_VICTORY:
	if (ticksMS - t_ref >= TIME_FINAL) {
		state = ST_IDLE;
		lcd_idle();
	}
	break;

	case ST_DEFEAT:
	if (ticksMS - t_ref >= TIME_FINAL) {
		state = ST_IDLE;
		lcd_idle();
	}
	break;




			
}
}