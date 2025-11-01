#ifndef FreeRTOS_Define_H
#define FreeRTOS_Define_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define Max_SoftwarePriority		8
#define Min_SoftwarePriority		1

/* -----------------Task----------------- */
extern  TaskHandle_t 					CONBlowerTaskHandle;
#define CONBlowerTask_DERTH			    100	
#define CONBlowerTask_Priority		    Max_SoftwarePriority

extern  TaskHandle_t 					MachineOnOffTaskHandle;
#define MachineOnOffTask_DERTH			1000	
#define MachineOnOffTask_Priority		6

extern  TaskHandle_t 					TestMaskTaskHandle;
#define TestMaskTask_DERTH			    500	
#define TestMaskTask_Priority			5


// extern  TaskHandle_t 					AlarmTaskHandle;
// #define AlarmTask_Depth                 100
// #define AlarmTask_Priority			    8

extern  TaskHandle_t 					RECMotorTaskHandle;
#define RECMotorTask_DERTH				100	
#define RECMotorTask_Priority			4

// extern  TaskHandle_t 					RGBTaskHandle;
// #define RGBTask_DERTH					100	
// #define RGBTask_Priority				2	

// extern  TaskHandle_t 					WCALDataTaskHandle;
// #define WCALDataTask_DERTH				100	
// #define WCALDataTask_Priority			2	

extern  TaskHandle_t 					CALDataTaskHandle;
#define CALDataTask_DERTH				100	
#define CALDataTask_Priority			2	

extern  TaskHandle_t 					MeasureTaskHandle;
#define MeasureTask_DERTH				60	
#define MeasureTask_Priority			1


/* -----------------Queue----------------- */
extern  QueueHandle_t                   BlowerStateQueue_Handle;
extern  QueueHandle_t 					BlowerRECQueue_Handle;
extern  QueueHandle_t					BlowerSendQueue_Handle;
extern  QueueHandle_t					CALDataQueue_Handle;
extern  QueueHandle_t					RunParamQueue_Handle;


/* -----------------Semaphore----------------- */
extern  SemaphoreHandle_t				PowerOnSemaphore_Handle;

/* -----------------EventGroup(32bit)----------------- */
extern EventGroupHandle_t 				MachineStateEvent_Handle;
#define Machine_On_Event				(0x01<<0)
#define MachineOn_PID_Event			    (0x01<<1)
#define CalibrateStart_Event			(0x01<<2)
#define CalibrateStartBlower_Event		(0x01<<3)
#define BlueTooth_On_Event			    (0x01<<4)
#define AlarmState_On_Event			    (0x01<<5)
#define TestMask_Start_Event			(0x01<<6)

extern EventGroupHandle_t 				FeedDogEvent_Handle;




#endif
