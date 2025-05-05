#include "keypad.h"
#include <avr/io.h>
#include <util/delay.h>

/* Mapa de caracteres  filas/columnas */
static const char keymap[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

/* bits de cada FILA (tres en PORTB, una en PORTD) */
static const uint8_t row_bits[4] = { PB4, PB3, PB0, PD7 };

/* bits de cada COLUMNA (todas en PORTD) */
static const uint8_t col_bits[4] = { PD3, PD5, PD4, PD2 };

void keypad_init(void)
{
    /* filas como entrada con pull-up */
    DDRB &= ~(_BV(PB4) | _BV(PB3) | _BV(PB0));
    PORTB |=  _BV(PB4) | _BV(PB3) | _BV(PB0);

    DDRD &= ~_BV(PD7);
    PORTD |=  _BV(PD7);

    /* columnas en alta Z + pull-up mientras no se escanean */
    DDRD  &= ~(_BV(PD2)|_BV(PD3)|_BV(PD4)|_BV(PD5));
    PORTD |=  (_BV(PD2)|_BV(PD3)|_BV(PD4)|_BV(PD5));
}

uint8_t keypad_scan(char *key)
{
    /* Recorremos las 4 columnas */
    for (uint8_t c = 0; c < 4; c++)
    {
        //Columna ‘c’ a LOW (los demás quedan en pull-up)
        DDRD  |=  _BV(col_bits[c]);      //Configurar como salida
        PORTD &= ~_BV(col_bits[c]);      
        _delay_us(3);                    

        /* 2. Recorremos las 4 filas */
        for (uint8_t r = 0; r < 4; r++)
        {
            uint8_t pressed;

            if (r == 3)  /* FILA3 = PD7 */
                pressed = !(PIND & _BV(PD7));
            else         /* FILA0-2  en PORTB */
                pressed = !(PINB & _BV(row_bits[r]));

            if (pressed)
            {
                *key = keymap[r][c];

                /* liberar la columna antes de salir */
                PORTD |=  _BV(col_bits[c]);   /* pull-up otra vez */
                DDRD  &= ~_BV(col_bits[c]);   /* alta Z           */
                return 1;
            }
        }

        /* 3. Ninguna tecla en esta columna ? la liberamos */
        PORTD |=  _BV(col_bits[c]);
        DDRD  &= ~_BV(col_bits[c]);
    }
    return 0;   
}
