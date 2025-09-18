#ifndef Application_Machine_OnOff_H
#define Application_Machine_OnOff_H

#include "Middle_Flow.h"
#include "Middle_Parameter.h"
#include "Middle_ComfortFunction.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "event_groups.h"

/* Machine on task. */
void App_Machine_OnOff_Task(void *pvParameter);


#endif


