#ifndef Middle_BreatheStage_H
#define Middle_BreatheStage_H

#include <stdlib.h>
#include "stm32f4xx.h"
#include "Use_Struct.h"
#include "FreeRTOS_Define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Add_Function.h"
#include "Middle_ComfortFunction.h"


/* Judge the breathe stage. */
void Mid_Judge_BreatheStage(int *pflow_buff,int *pexend_flow,int *pstd_buff,eBreathe_Stage *pnow_stage);

#endif
