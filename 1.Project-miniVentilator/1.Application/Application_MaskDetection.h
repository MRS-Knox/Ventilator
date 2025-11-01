#ifndef Application_MaskDetection_H
#define Application_MaskDetection_H

#include "Use_Struct.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "Middle_BreatheStage.h"
#include "Middle_ComfortFunction.h"
#include "Middle_CalculateParameter.h"


/* Mask detection. */
void App_MaskDetection_Task(void *pvPatameter);

#endif


