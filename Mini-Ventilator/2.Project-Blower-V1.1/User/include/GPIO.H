/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : GPIOInit.h
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-28
    Description    : This file contains GPIO parameter used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __G_P_I_O_H_
#define __G_P_I_O_H_

#define FGPin 				GP00
#define SetFGPin			GP00 = 1
#define ResetFGPin		    GP00 = 0
#define FGPinONOFF          GP00 = ~GP00

extern void GPIO_Init(void);
extern void GPIO_DefaultInit(void);
#endif


