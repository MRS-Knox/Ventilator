/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : FocControl.h
    Author         : Fortiortech  Appliction Team
    Version        : V1.0
    Date           : 2021-12-28
    Description    : This file contains XX-XX-XX parameter used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __FOC_CONTROL_H_
#define __FOC_CONTROL_H_

typedef enum
{
    mcReady     = 0,
    mcInit      = 1,
    mcCharge    = 2,
    mcTailWind  = 3,
    mcAlign     = 5,
    mcStart     = 6,
    mcRun       = 7,
    mcStop      = 8,
    mcFault     = 9,
    mcBrake     = 11
}MotStaType;

typedef union
{
    struct
    {
        uint8 CalibFlag        :1;                                              // 电流校准的标志位
        uint8 ChargeSetFlag    :1;                                              // 预充电配置标志位
        uint8 AlignSetFlag     :1;                                              // 预定位配置标志位
        uint8 TailWindSetFlag  :1;                                              // 顺逆风配置标志位
        uint8 StartSetFlag     :1;                                              // 启动配置标志位
    } SetFlag;
}MotStaM;

extern MotStaType xdata mcState;
extern MotStaM  xdata  McStaSet;
extern void MC_Control(void);
extern void MotorcontrolInit(void);
#endif


