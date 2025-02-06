/* --------------------------- (C) COPYRIGHT 2022 Fortiortech ShenZhen -----------------------------
    File Name      : MotorProtect.h
    Author         : Fortiortech  Appliction Team
    Version        : V1.0
    Date           : 2022-12-09
    Description    : This file contains XX-XX-XX parameter used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __MOTOR_PROTECT_H_
#define __MOTOR_PROTECT_H_

typedef struct
{
    uint8 OverCurCnt;                                                          // 软件过流计数
    int16 Max_ia;                                                              // IA的最大值
    int16 Max_ib;                                                              // IB的最大值
    int16 Max_ic;                                                              // IC的最大值
}CurrentVarible;

typedef struct
{
    uint16 SecondStartTimes;                                                    // 二次启动保护的次数
    uint16 StallTimes;                                                          // 堵转保护次数
    uint16 LossPHTimes;                                                         // 缺相保护次数
    uint16 CurrentPretectTimes;                                                 // 过流保护次数
    uint8  StartFlag;                                                           // 启动保护的标志位，用于判断哪个方法起作用
    uint8  StallFlag;                                                           // 堵转保护的标志位，用于判断哪个方法起作用
}ProtectVarible;

typedef struct
{
    uint16 segment;                                                             // 分段执行
    uint16 OverVoltDetecCnt;                                                    // 过压检测计数
    uint16 UnderVoltDetecCnt;                                                   // 欠压检测计数
    uint16 VoltRecoverCnt;                                                      // 过压恢复计数
    uint16 CurrentRecoverCnt;                                                   // 过流保护恢复计数
    uint16 StallDelayCnt;                                                       // 堵转延迟判断计时
    uint16 StallDectEs;                                                         // method 1，与ES相关
    uint16 StallTimes;
    uint16 StallDectSpeed;                                                      // method 2，与速度相关
    uint16 StallDectESSpeed;                                                    // method 3，与ES和速度相关
    uint16 StallReCount;                                                        // 堵转保护恢复计数
    uint16 StartReCount;
    uint16 Lphasecnt;                                                           // 缺相保护计时
    uint16 LPhasetimes;
    uint16 AOpencnt ;                                                           // A缺相计数
    uint16 BOpencnt ;                                                           // B缺相计数
    uint16 ABCOpencnt;
    uint16 COpencnt ;                                                           // C缺相计数
    uint16 mcLossPHRecCount;                                                    // 缺相恢复计数
    int16 StartESCount;                                                         // 启动保护判断ES的计数
    int16 StartEsCnt;                                                           // 启动保护判断ES的计时
    int16 StartDelay;                                                           // 启动保护判断ES的延迟
    int16 StartFocmode;                                                         // 启动保护判断FOCMODE状态的计时
    int16 StartSpeedCnt;                                                        // 启动保护判断速度和ES的计时

}FaultVarible;

extern CurrentVarible xdata mcCurVarible;
extern ProtectVarible xdata mcProtectTime;
extern FaultVarible   xdata mcFaultDect;

extern void   Fault_Start(void);
extern void   Fault_OverUnderVoltage(void);
extern void   Fault_Overcurrent(void);
extern void   Fault_Stall(void);
extern void   Fault_phaseloss(void);
extern void Fault_GetCurrentOffset(void);
extern void Fault_Recovery(void);
#endif