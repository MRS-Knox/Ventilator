#ifndef Middle_RGB_H
#define Middle_RGB_H

#include "Driver_RGB.h"
#include "FreeRTOS_Define.h"
#include "Use_Struct.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Initialize RGB light. */
void Mid_RGB_Init(void);
/* Close RGB lights. */
void Mid_RGB_Close(void);
/* RGB lights are flash. */
void Mid_RGB_Flash(eRGB_Light light_color,uint8_t flash_frequency,uint8_t base_time,FlagStatus flag_clear);
/* RGB light task. */
void Mid_RGB_Task(void *pvParameter);
	
#endif


