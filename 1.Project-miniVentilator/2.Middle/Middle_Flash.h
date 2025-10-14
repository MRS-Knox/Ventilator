#ifndef Middle_Flash_H
#define Middle_Flash_H

#include "Driver_Flash.h"
#include "FreeRTOS_Define.h"
#include "Use_Struct.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"


/* This initial function is about flash hardware. */
void Mid_Flash_Init(void);
/* This function is about flash read and write. */
ErrorStatus Mid_WriteCALData(CalibrationData_t *data_t);
/* Read calibration data from flash when machine just power on. */
void Mid_ReadCALData_Power(void);

#endif


