#ifndef Middle_Blower_H
#define Middle_Blower_H

#include "Driver_Blower.h"
#include "Use_Struct.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "queue.h"


#define MOTOR_SPEED_MAX			45000
#define MOTOR_SPEED_MIN			10000
#define SEND_RPM				0x01
#define SEND_COMMAND			0xFF
#define BLOWER_FRAMEHEADER 		0x09	
#define BLOWER_FRAMEEND 		0x0123

#define START_RPM				15000

/* This initial function about blower. */
void Mid_Blower_Init(void);
/* This function about send data to blower chip. */
void Mid_SendToBlower(uint8_t data_type,uint16_t send_data);
/* Base pressure calculate blower rpm. */
uint16_t Mid_CalBlower_RPM(uint16_t press_value);

#endif
