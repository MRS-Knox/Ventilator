/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : FocControlFunction.h
    Author         : Lewis.Wang
    Version        : V1.0
    Date           : 2021-12-07
    Description    : This file contains FocControl parameter used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __FOC_CONTROL_FUNCTION_H_
#define __FOC_CONTROL_FUNCTION_H_

extern void GetCurrentOffset(void);
extern void Motor_Ready (void);
extern void Motor_Init (void);
extern void FOC_Init(void);
extern void Motor_Charge(void);
extern void Motor_Align(void);
extern void MotorcontrolInit(void);
extern void Motor_Open(void);

#endif

