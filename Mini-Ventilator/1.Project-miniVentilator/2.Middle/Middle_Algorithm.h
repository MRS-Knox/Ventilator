#ifndef Middle_Algorithm_H
#define Middle_Algorithm_H

#include "stm32f4xx.h"
#include "Use_Struct.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Add_Function.h"
#include "Middle_ComfortFunction.h"

/* Calculate running parameters.Judge inspiration and expiration. */
void Mid_JudgeInsEx(void *pvParameter);
/* Inspiration stage process function. */
uint16_t Mid_Inspiration_Stage(short set_press,eTherapy_Mode therapy_mode,eBreathe_Stage ins_stage);
/* Expiration stage process function. */
uint16_t Mid_Expiration_Stage(short set_press,eTherapy_Mode therapy_mode,eBreathe_Stage ex_stage);


#endif
