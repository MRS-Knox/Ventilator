#ifndef Middle_BreatheStage_H
#define Middle_BreatheStage_H

#include "stm32f4xx.h"
#include "Use_Struct.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Add_Function.h"
#include "Middle_ComfortFunction.h"

/* Judge the breathing stage. */
eBreathe_Stage Mid_Judge_BreatheStage(int *pflow_value,uint16_t count,eBreathe_Stage now_stage);
/* Inspiration stage process function. */
uint16_t Mid_Inspiration_Stage(unsigned short set_press,eBreathe_Stage ins_stage);
/* Expiration stage process function. */
uint16_t Mid_Expiration_Stage(unsigned short set_press,eBreathe_Stage ex_stage);

#endif
