#ifndef Application_Calibration_H
#define Application_Calibration_H

#include <math.h>
#include "Middle_RTC.h"
#include "Middle_Pressure.h"
#include "Middle_Blower.h"
#include "Middle_Flash.h"
#include "Use_Struct.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/* Calibrate pressure and flow. */
void App_Calibration_Task(void *pvParameter);
/* Debug function. */
void App_Cali_Debug_Function(void *pvParameter);


#endif
