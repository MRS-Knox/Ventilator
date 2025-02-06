#ifndef FreeRTOS_Define_H
#define FreeRTOS_Define_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define Max_Priority		10
#define Min_Priority		1

/* -----------------Task----------------- */
TaskHandle_t 							PRESSTaskHandle;
#define PRESSTask_DERTH					100	
#define PRESSTask_Priority				Max_Priority	

TaskHandle_t 							MeasureTaskHandle;
#define MeasureTask_DERTH				50	
#define MeasureTask_Priority			Min_Priority


/* -----------------Queue----------------- */
QueueHandle_t 							PRESSQueue_Handle;

#endif
