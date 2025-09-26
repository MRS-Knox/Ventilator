#ifndef Application_Machine_OnOff_H
#define Application_Machine_OnOff_H

#include "Middle_Flow.h"
#include "Middle_CalculateParameter.h"
#include "Middle_PID.h"
#include "Middle_BreatheStage.h"
#include "Middle_ComfortFunction.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "event_groups.h"

/* Machine on task. */
void App_Machine_OnOff_Task(void *pvParameter);
/* This function is about set parameters when the machine is on. */
void App_MachineOn_SetParam(void);
/* This function is about clear parameters when the machine is off. */
void App_MachineOff_ClearParam(void);

#endif


