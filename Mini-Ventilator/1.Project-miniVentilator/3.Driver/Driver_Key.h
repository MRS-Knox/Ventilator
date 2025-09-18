#ifndef Driver_Key_H
#define Driver_Key_H

#include "stm32f4xx_conf.h"

#define OpenKey_Pin			GPIO_Pin_1
#define BlueKey_Pin			GPIO_Pin_9
#define OpenKey_Group		GPIOA
#define BlueKey_Group		GPIOB


/* This initial function is about key hardware. */
void Dri_KeyHardware_Init(void);

#endif
