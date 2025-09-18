#ifndef Middle_TestMask_H
#define Middle_TestMask_H

#include "stm32f4xx.h"
#include "Use_Struct.h"
#include "Middle_PID.h"
#include "Middle_Pressure.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Add_Function.h"

/* Test the mask gas tightness. */
void Mid_TestMask(void *pvParameter);

#endif
