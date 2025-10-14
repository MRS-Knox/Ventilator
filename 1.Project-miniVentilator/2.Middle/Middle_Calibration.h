#ifndef Middle_Calibration_H
#define Middle_Calibration_H

#include "Middle_Flow.h"
#include "Middle_Pressure.h"
#include "Middle_Blower.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "event_groups.h"

/* Calibrate flow and pressure. */
void Mid_CalFlow_Pressure(void *pvParameter);

/* Debug function. */
void Mid_CalFlow_Pressure_Debug_Function(void *pvParameter);

#endif
