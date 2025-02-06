/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : PIInit.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-07
    Description    : This file contains PI function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <FU68xx_4_MCU.h>
#include <Myproject.h>
/* -------------------------------------------------------------------------------------------------
    Function Name  : PI3_Init
    Description    : PI3初始化配置
    Date           : 2021-12-07
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void PI3_Init(void)
{
    PI3_KP  = SKP3;
    PI3_KI  = SKI3;
    PI3_UKH = 0;
    PI3_UKL = 0;
    PI3_EK  = 0;
    PI3_EK1 = 0;
    PI3_EK2 = 0;
    PI3_UKMAX =SOUT3MAX;
    PI3_UKMIN =SOUT3MIN;
    SetBit(PI_CR , PI3STA);         
    while (ReadBit(PI_CR , PIBSY));
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : PI_Init_PLL
    Description    : PLL中PI初始化
    Date           : 2022-12-19
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void PI_Init_PLL(void)
{
    PI3_KP  = PLLKP_GAIN;
    PI3_KI  = PLLKI_GAIN;
    PI3_UKH = 0;
    PI3_UKL = 0;
    PI3_EK  = 0;
    PI3_EK1 = 0;
    PI3_KD = 0;
    PI3_EK2 = 0;
    PI3_UKMAX = 32767;
    PI3_UKMIN = -32767;
    SetBit(PI_CR , PI3STA);          // Start PI
    while (ReadBit(PI_CR , PIBSY));
    PI2_KP  = PLLKP_GAIN;
    PI2_KI  = PLLKI_GAIN;
    PI2_UKH = 0;
    PI2_UKL = 0;
    PI2_EK  = 0;
    PI2_EK1 = 0;
    PI2_KD = 0;
    PI2_EK2 = 0;
    PI2_UKMAX = 32767;
    PI2_UKMIN = -32767;
    SetBit(PI_CR , PI2STA);          // Start PI
    while (ReadBit(PI_CR , PIBSY));
}