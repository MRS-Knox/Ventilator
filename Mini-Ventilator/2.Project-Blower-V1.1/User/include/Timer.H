/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : TimerInit.h
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-28
    Description    : This file contains Timer parameter used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __TIMER_H_
#define __TIMER_H_

#define TIM2_Fre                       (187500.0)                                
#define TIM4_Fre                       (1500000.0)                                

extern void Timer2_Init(void);
extern void Timer3_Init(void);
extern void Timer4_Init(void);

#endif


