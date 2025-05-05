#include "dict.h"
#include <avr/pgmspace.h>
#include <stdlib.h>     // rand, srand
#include <string.h>     // memcpy_P


static const char dict[][WORD_BUF] PROGMEM = {
	"Arbol", "Boton", "CDyMC", "ClavE", "Facil",
	"Gafas", "Hojas", "LiBro", "Lanza", "Nieve",
	"PeRro", "PecES", "PiAno", "PrYKe", "RUEDa",
	"SERIE", "SalUd", "Salud", "Silla", "Tecla",
	"Valor", "Verde", "YnHRz", "hARdD", "silla"
};

#define DICT_SIZE (sizeof(dict) / WORD_BUF)

uint8_t dict_size(void)
{
	return DICT_SIZE;
};

//Toma como "semilla" el valor actual de 16-bit del clock al momento que se llama el metodo 
//para la randomizacion de la palabra
void dict_seed(uint32_t s)
{
	srand((unsigned)s);
}

void dict_get_random_word(char *wrd)
{
	uint8_t idx = (uint8_t)(rand() % DICT_SIZE);   
	memcpy_P(wrd, dict[idx], WORD_BUF);            
}

