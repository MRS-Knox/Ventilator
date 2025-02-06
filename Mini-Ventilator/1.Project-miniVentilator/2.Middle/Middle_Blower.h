#ifndef Middle_Blower_H
#define Middle_Blower_H

#include "Driver_Blower.h"
#include "Use_Struct.h"

#define MOTOR_SPEED_MAX			40000
#define MOTOR_SPEED_MIN			12000
#define SEND_RPM				0x01
#define SEND_COMMAND			0xFF
#define BLOWER_FRAMEHEADER 		0x09	
#define BLOWER_FRAMEEND 		0x0123

/* This initial function about blower. */
void Mid_Blower_Init(void);
/* This function about send data to blower chip. */
void Mid_SendToBlower(uint8_t data_type,uint16_t send_data);
/* Main chip receives data from blower chip. */
void Mid_ReciveMotor(void);

#endif
