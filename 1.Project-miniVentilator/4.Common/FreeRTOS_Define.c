#include "FreeRTOS_Define.h"

/* -----------------Task----------------- */
TaskHandle_t 							RECMotorTaskHandle			= NULL;
TaskHandle_t 					        CONBlowerTaskHandle		    = NULL;
TaskHandle_t 							MachineOnOffTaskHandle 	    = NULL;
TaskHandle_t 							MeasureTaskHandle			= NULL;
TaskHandle_t        					TestMaskTaskHandle          = NULL;
// TaskHandle_t 							RGBTaskHandle				= NULL;
// TaskHandle_t 							WCALDataTaskHandle			= NULL;
TaskHandle_t 							CALDataTaskHandle			= NULL;
// TaskHandle_t 					        JudgeInsExTaskHandle        = NULL;
// TaskHandle_t 					        AlarmTaskHandle             = NULL;

/* -----------------Queue----------------- */
QueueHandle_t                           BlowerStateQueue_Handle     = NULL;
QueueHandle_t 							BlowerRECQueue_Handle		= NULL;
QueueHandle_t							BlowerSendQueue_Handle		= NULL;
QueueHandle_t							CALDataQueue_Handle			= NULL;
QueueHandle_t							RunParamQueue_Handle	    = NULL;



/* -----------------Semaphore----------------- */
SemaphoreHandle_t						PowerOnSemaphore_Handle		= NULL;

/* -----------------Event----------------- */
EventGroupHandle_t 						MachineStateEvent_Handle 	= NULL;
EventGroupHandle_t 						FeedDogEvent_Handle 	    = NULL;


