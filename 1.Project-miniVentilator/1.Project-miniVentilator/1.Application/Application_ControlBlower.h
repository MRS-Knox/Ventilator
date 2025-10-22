#ifndef Application_ControlBlower_H
#define Application_ControlBlower_H

#include "Middle_Blower.h"
#include "Middle_PID.h"
#include "Middle_Pressure.h"
#include "Use_Struct.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "task.h"

/* Control blower. */
void App_ControlBlower_Task(void *pvParameter);

#endif





