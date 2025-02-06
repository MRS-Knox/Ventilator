#ifndef Middle_Pressure_H
#define Middle_Pressure_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "Driver_Pressure.h"
#include "Use_Struct.h"
#include "FreeRTOS.h"


/* This initial function is about pressure. */
void Mid_Pressure_Init(void);
/* Voltage converts to pressure. */
void Mid_CalculatePRESS(void *pvPatameter);

#endif

