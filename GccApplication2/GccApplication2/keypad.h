#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

/* Inicializa GPIO y deja todas las columnas en alta Z con pull-up */
void  keypad_init(void);

// Devuelve 1 si alguna tecla fue presionada y la escribe en *key, 0 si no hay nada. 
uint8_t keypad_scan(char *key);

#endif
