/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : WDTInit.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-07
    Description    : This file contains WDT function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <MyProject.h>
#include <FU68xx_4_MCU.h>

/* -------------------------------------------------------------------------------------------------
    Function Name  : WatchDogConfig
    Description    : 看门狗初始化配置函数
    Date           : 2021-12-07
    Parameter      : wdtArrValue: [输入]:wdtArrValueValue--定时时间
------------------------------------------------------------------------------------------------- */
void WatchDogConfig(uint16 wdtArrValue)
{
    SetBit(CCFG1, WDT_EN);                                                      //看门狗使能
    WDT_ARR = ((uint16)(65532 - (uint32)wdtArrValue * 32768 / 1000) >> 8);
    ClrBit(WDT_CR, WDTF);                                                       //看门狗复位标志清零
    SetBit(WDT_CR, WDTRF);                                                      //看门狗初始化
}