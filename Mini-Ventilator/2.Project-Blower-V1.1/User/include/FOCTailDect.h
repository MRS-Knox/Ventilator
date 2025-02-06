/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : FOCTailDect.h
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-27
    Description    : This file contains FOCTailDect datas parameter used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __F_O_C_TAIL_DECT_H_
#define __F_O_C_TAIL_DECT_H_

#include <FU68xx_4.h>
#include <Myproject.h>

typedef struct
{
    uint16 TailWindSpeed;               // 顺逆风速度
    uint8  MotorDir;                    // 电机方向
    uint8  AngleState;                  // 角度状态
    uint8  MotorTailWindState;          // 电机顺逆风状态
    uint8  SpeedStoreNum;               // 速度计算的次数
    uint16 SpeedCountTimer;             // 速度检测计时
    uint8  SpeedTimerClearStatus;       // 速度检测计时的清零标志位
    uint32 TempThailWindSpeedBase;      // 顺逆风速度基准
    uint8  ShakeTimes;                  // 1和3状态来回抖动的次数
    uint8  AntiFlag;                    // 不同转速的反转标志
    uint8  SpeedOverFlowStatus;
}MotorTailWindTypeDef;


#define NormalTailWind                  (0)
#define BrakeTailWind                   (1)
#define CW                              (0)
#define CCW                             (1)

#define ThailWindSpeedBase              ((1000/2) * 60 / Pole_Pairs)

#define TailWindStartMinSpeed           (uint16)(1000.0/(TailWind_Time*2)*60.0/Pole_Pairs)      //顺风启动的最低转速

extern MotorTailWindTypeDef xdata  TailWindDetect;

extern void TailWindDetectInit(void);
extern void TailWindSpeedDetect(void);
extern void FOCTailWindDealwith(void);
extern void PLL_TailWindDealwith(void);
extern void SMO_TailWindDealwith(void);
extern void FOCTailWindTimeLimit(void);
extern void FOCCloseLoopStart(void);

#endif

