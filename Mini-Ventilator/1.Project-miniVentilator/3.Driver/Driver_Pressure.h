#ifndef Driver_Pressure_H
#define Driver_Pressure_H

#include "stm32f4xx_conf.h"
#include "Add_Function.h"

#define PRESS_Pin		GPIO_Pin_0
#define PRESS_Group		GPIOB

/* This initial function about pressure hardware. */
void Dri_PHardware_Init(void);
/* This function is about read pressure sensor output voltage. */
uint16_t Dri_ReadPRESS_V(void);

#endif
