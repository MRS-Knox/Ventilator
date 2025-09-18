#ifndef Middle_Alarm_H
#define Middle_Alarm_H

#include "Middle_TestPower.h"
#include "Use_Struct.h"
#include "Middle_RTC.h"
#include "Middle_Voice.h"
#include "Middle_Blower.h"
#include "Middle_RGB.h"
#include "Add_Function.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "FreeRTOS_Define.h"

/* Machine alarm. */
void Mid_MachineAlarm(void *pvParameter);
/* Calculate the usage time of consumables whether over the setting time of consumables. */
void Mid_CALConsumable_ChangeTime(void);
/* Test highest priority. */
void Mid_TestHighPriority(void);
/* Test middle priority. */
void Mid_TestMiddlePriority(void);
/* Test lowest priority. */
void Mid_TestLowPriority(void);


#endif



