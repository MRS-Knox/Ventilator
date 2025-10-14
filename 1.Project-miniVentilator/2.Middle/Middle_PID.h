#ifndef Middle_PID_H
#define Middle_PID_H

#include "Middle_Blower.h"

/* Increase PID algorithme intial parameter function. */
void Mid_IncreasePID_Init(void);
/* Increase PID algorithme function. */
void Mid_IncreasePID(uint16_t target_press,uint16_t measure_press);

#endif
