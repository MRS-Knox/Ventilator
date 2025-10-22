#ifndef Middle_ComfortFunction_H
#define Middle_ComfortFunction_H

#include "Use_Struct.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "FreeRTOS_Define.h"

/* This function is about ventilator delay up pressure function. */
uint16_t Mid_DelayIncreasePRESS(FlagStatus flag_delay,uint16_t real_time,uint8_t delay_min,uint16_t start_press,uint16_t point_press);
/* Auto open machine and auto close machine. */
void Mid_AutoOn_AutoOff(int flow_data,EventBits_t event_bit);
/* Release pressure when the breathing stage is expiration. */
void Mid_EPR(uint16_t *prun_press,uint16_t set_press,eBreathe_Stage now_stage);


#endif
