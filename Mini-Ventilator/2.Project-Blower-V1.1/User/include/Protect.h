/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      Customer.h
 * @author    Fortiortech  Appliction Team
 * @date      2022-07-13
 * @brief     This file contains protect parameter used for Motor Contro.
 */
/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __PROTECT_H_
#define __PROTECT_H_

/* -------------------------------------保护使能设置---------------------------------------------------- */
#define VoltageProtectEnable           (1)                                     ///< 过欠压保护. 0 :禁止, 1 ：使能.
#define StartProtectEnable             (1)                                     ///< 启动保护. 0 :禁止, 1 ：使能.
#define StallProtectEnable             (1)                                     ///< 堵转保护. 0 :禁止, 1 ：使能.
#define PhaseLossProtectEnable         (1)                                     ///< 缺相保护. 0 :禁止, 1 ：使能.
#define HardwareCurrent_Protect        (1)                                     ///< 硬件过流保护. 0 :禁止, 1 ：使能.
#define GetCurrentOffsetEnable         (1)                                     ///< 偏置电压异常保护. 0 :禁止, 1 ：使能.

/* -------------------------------------保护参数---------------------------------------------------- */
/* -----硬件过流保护 ----- */
#define Compare_DAC                    (0)                                     ///< DAC设置过流阈值
#define Compare_Hardware               (1)                                     ///< 硬件设置过流阈值
#define Compare_Mode                   (Compare_DAC)                           ///< 硬件过流保护阈值来源
#define OverHardcurrentValue           (10.0)                                  ///< DAC模式下硬件过流阈值

/* -----偏置电压保护----- */
#define GetCurrentOffsetValue          _Q14(0.05)                               ///< (%) 偏置电压保护误差范围，超过该范围保护   

/* -----过欠压保护---- */
#define Over_Protect_Voltage           (27)                                    ///< (V) 直流电压过压保护阈值
#define Over_Recover_Vlotage           (26)                                    ///< (V) 直流电压过压保护恢复阈值
#define Under_Protect_Voltage          (12)                                    ///< (V) 直流电压欠压保护阈值
#define Under_Recover_Vlotage          (13)                                    ///< (V) 直流电压欠压保护恢复阈值 
#define VOU_Recover_Time               (50)                                    ///< (ms) 欠过压保护恢复时间

/* -----启动保护----- */
#define StartRecoverTime               (2000)                                  ///< (ms) 启动保护恢复时间
#define StartProtectRestartTimes       (5)                                     ///<  启动保护重启次数

/* -----堵转保护----- */
#define StallRecoverTime               (2000)                                  ///< (ms) 堵转保护恢复时间 
#define StallProtectRestartTimes       (5)                                     ///<  堵转保护重启次数
#define MOTOR_SPEED_STAL_MAX_RPM       (50000.0)                               ///<  堵转保护转速上限阈值    
#define MOTOR_SPEED_STAL_MIN_RPM       (5000.0)                                ///<  堵转保护转速下限阈值

/* -----缺相保护----- */
#define PhaseLossCurrentValue          I_Value(0.3)                            ///<  缺相保护过流阈值
#define PhaseLossRecoverTime           (2000)                                  ///< (ms) 缺相保护恢复时间 
#define PhaseLossRestartTimes          (5)                                     ///<  缺相保护重启次数
#define PhaseLossTimes                 (1.2)           //1.2                        ///<  缺相时电流倍数

/* -----软件过流保护----- */
#define OverSoftCurrentValue           I_Value(20.0)                            ///< 软件过流保护阈值.  
#define OverSoftCurrentTimes           (100)                                   ///< (ms) 软件过流检测时间
#define OverCurrentRestartTimes        (5)                                     ///< 过流恢复次数
#define OverCurrentRecoverTime         (1500)                                  ///< (ms)过流恢复时间

#endif