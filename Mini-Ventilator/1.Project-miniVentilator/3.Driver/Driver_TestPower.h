#ifndef Driver_TestPower_H
#define Driver_TestPower_H

#include "stm32f4xx.h"

#define TestPower_Pin            GPIO_Pin_1
#define TestPower_Group          GPIOB

/* Initialize test power adc hardware. */
void Dri_TestPowerHardware_Init(void);
/* Test PB1 status. */
FlagStatus Dri_TestPB1_State(void);   

#endif




