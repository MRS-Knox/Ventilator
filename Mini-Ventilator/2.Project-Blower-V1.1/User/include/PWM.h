/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : PWM.h
    Author         : Lewis.Wang
    Version        : V1.0
    Date           : 2021-12-05
    Description    : This file contains PWM datas used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __P_W_M_H_
#define __P_W_M_H_

//PWMduty
#define PWMDuty_10                     (0x0E35)                            
#define PWMDuty_15                     (0x14A3)                             
#define PWMDuty_22                     (0x1DA5)                             
#define PWMDuty_28                     (0x2559)                             
#define PWMDuty_35                     (0x2E57)                              
#define PWMDuty_40                     (0x34A7)                             
#define PWMDuty_45                     (0x3B13)                             
#define PWMDuty_50                     (0x417E)                             
#define PWMDuty_55                     (0x47EB)                            
#define PWMDuty_59                     (0x4D0E)                              
#define PWMDuty_65                     (0x54A7)                              
#define PWMDuty_70                     (0x5B13)                            
#define PWMDuty_75                     (0x618C)                              
#define PWMDuty_80                     (0x67F0)                             
#define PWMDuty_85                     (0x6E43)                              
#define PWMDuty_90                     (0x74AC)                             
#define PWMDuty_95                     (0x7B31) 

//功率限制曲线
#define OUTDuty_10                     (400)                              
#define OUTDuty_15                     (600)                            
#define OUTDuty_22                     (800)                             
#define OUTDuty_28                     (1000)                             
#define OUTDuty_35                     (1400)                             
#define OUTDuty_40                     (1800)                              
#define OUTDuty_45                     (2200)                              
#define OUTDuty_50                     (2600)                            
#define OUTDuty_55                     (2900)                           
#define OUTDuty_59                     (3100)                              
#define OUTDuty_65                     (3300)                              
#define OUTDuty_70                     (3500)                              
#define OUTDuty_75                     (3700)                             
#define OUTDuty_80                     (4000)                             
#define OUTDuty_85                     (4200)                            
#define OUTDuty_90                     (4800)                              
#define OUTDuty_95                     (5400)                             
#define OUTDuty_100                    (8000) 
 
//开环占空比曲线
#define Limit_Duty_10                   _Q15(0.20)                             
#define Limit_Duty_15                   _Q15(0.24)                            
#define Limit_Duty_22                   _Q15(0.275)                              
#define Limit_Duty_28                   _Q15(0.325)                            
#define Limit_Duty_35                   _Q15(0.398)                            
#define Limit_Duty_40                   _Q15(0.42)                              
#define Limit_Duty_45                   _Q15(0.46)                            
#define Limit_Duty_50                   _Q15(0.49)                           
#define Limit_Duty_55                   _Q15(0.52)                           
#define Limit_Duty_59                   _Q15(0.57)                             
#define Limit_Duty_65                   _Q15(0.60)                             
#define Limit_Duty_70                   _Q15(0.63)                            
#define Limit_Duty_75                   _Q15(0.65)                             
#define Limit_Duty_80                   _Q15(0.68)                             
#define Limit_Duty_85                   _Q15(0.72)                             
#define Limit_Duty_90                   _Q15(0.75)                             
#define Limit_Duty_95                   _Q15(0.85)                             
#define Limit_Duty_100                  _Q15(0.888)


/* 频率调速时需要设置的参数 */ 
#define ONPWMFre                      (50)                                    // 开机频率
#define MINPWMFre                     (50)                                    // 速度曲线上最小PWM占空比
#define Speed_PerFre                  (4.0)                                   // 目标转速与输入频率的比例系数  例：Speed_PerFre = 4，Fre = 75;则目标转速为300
#define MAXPWMFre                     (int)(((MOTOR_SPEED_MAX_RPM - MOTOR_SPEED_MIN_RPM)/4) + MINPWMFre)                               


#endif