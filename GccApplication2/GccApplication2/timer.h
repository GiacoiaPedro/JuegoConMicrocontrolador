#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

//contador global de milisegundos
extern volatile uint32_t ticksMS;

void timer_init(void);

#endif
