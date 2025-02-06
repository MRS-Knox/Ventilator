/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : Parameter.h
    Author         : Fortiortech  Appliction Team
    Version        : V1.0
    Date           : 2021-12-08
    Description    : This file contains all FOC debugs parameter used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __Parameter_H_
#define __Parameter_H_

#define _Q7(A)                          (int8)(A * 0x7f)
#define _Q8(A)                          (int16)(A * 0xff)
#define _Q10(A)                         (int16)(A * 0x3ff)                                             // Q15 format
#define _Q11(A)                         (int16)(A * 0x7ff)                                             // Q12 format
#define _Q12(A)                         (int16)(A * 0xfff)                                             // Q12 format
#define _Q13(A)                         (int16)(A * 0x1fff)                                             // Q12 format
#define _Q14(A)                         (int16)(A * 0x3fff)                                             // Q12 format
#define _Q15(A)                         (int16)(A * 0x7fff)                                            // Q15 format
#define _2PI                            (3.1415926 * 2)                                               // 2 PI value
#define _Q16                            (65535.0)                                                     // Q16 format value

/*芯片参数值------------------------------------------------------------------*/
/*CPU and PWM Parameter*/
#define PWM_CYCLE                       (1000.0 / PWM_FREQUENCY)                      // 周期us
#define SAMP_FREQ                       (PWM_FREQUENCY * 1000)                        // 采样频率(HZ)
#define TPWM_VALUE                      (1.0 / SAMP_FREQ)                             // 载波周期(S)
#define PWM_VALUE_LOAD                  (uint16)(MCU_CLOCK * 1000 *2 / PWM_FREQUENCY) // PWM 定时器重载值 

#define I_ValueX(Curr_Value)            (Curr_Value * HW_RSHUNT * HW_AMPGAIN / HW_ADC_REF)
#define I_Value(Curr_Value)             _Q15(I_ValueX(Curr_Value))

/*硬件板子参数设置值------------------------------------------------------------------*/
/*hardware current sample Parameter*/
/*电流基准的电路参数*/
#define HW_BOARD_CURR_MAX               (HW_ADC_REF / 2 / HW_AMPGAIN / HW_RSHUNT) // 最大采样电流,2.702A
#define HW_BOARD_CURR_MIN               (-HW_BOARD_CURR_MAX)                                        // 最小采样电流,-2.702A
#define HW_BOARD_CURR_BASE              (HW_BOARD_CURR_MAX * 2)                                       // 电流基准//5.4A

/*hardware voltage sample Parameter*/
/*母线电压采样分压电路参数*/
#define HW_BOARD_VOLTAGE_BASE           (HW_BOARD_VOLT_MAX / 1.732)                              // 电压基准
#define HW_BOARD_VOLTAGE_VC             ((RV1 + RV2 + RV3 * VC1) / (RV3 * VC1))
#define HW_BOARD_VOLTAGE_BASE_Start     (HW_ADC_REF * HW_BOARD_VOLTAGE_VC / 1.732)               // 电压基准


#define HW_ADC_REF                     (5.0)                                    // (V)  ADC参考电压


/*保护参数值------------------------------------------------------------------*/
/* protect value */
#define OVER_PROTECT_VALUE              _Q15(Over_Protect_Voltage  / HW_BOARD_VOLT_MAX)
#define UNDER_PROTECT_VALUE             _Q15(Under_Protect_Voltage / HW_BOARD_VOLT_MAX)
#define OVER_RECOVER_VALUE              _Q15(Over_Recover_Vlotage  / HW_BOARD_VOLT_MAX)
#define UNDER_RECOVER_VALUE             _Q15(Under_Recover_Vlotage / HW_BOARD_VOLT_MAX)

/* motor speed set value */
#define Motor_Open_Ramp_ACC             _Q15(MOTOR_OPEN_ACC     / MOTOR_SPEED_BASE)
#define Motor_Open_Ramp_Min             _Q15(MOTOR_OPEN_ACC_MIN / MOTOR_SPEED_BASE)

#define Motor_Omega_Ramp_Min            _Q15(MOTOR_OMEGA_ACC_MIN / MOTOR_SPEED_BASE)
#define Motor_Omega_Ramp_End            _Q15(MOTOR_OMEGA_ACC_END / MOTOR_SPEED_BASE)

#define Motor_Loop_Speed                _Q15(MOTOR_LOOP_RPM / MOTOR_SPEED_BASE)
#define Motor_Speed(setspeedvalue)		_Q15(setspeedvalue   / MOTOR_SPEED_BASE)				
#define Motor_Max_Speed                 _Q15(MOTOR_SPEED_MAX_RPM   / MOTOR_SPEED_BASE)
#define Motor_Min_Speed                 _Q15(MOTOR_SPEED_MIN_RPM   / MOTOR_SPEED_BASE)
#define MOTOR_ONOFF_Speed               _Q15(MOTOR_ONOFF_RPM   / MOTOR_SPEED_BASE)  

#define SPEED_K                         (float)(Motor_Max_Speed - Motor_Min_Speed)/(MAXPWMDuty - MINPWMDuty)

#define POWER_K                         (float)(MOTOR_POWER_MAX_RPM - MOTOR_POWER_MIN_RPM)/(MAXPWMDuty - MINPWMDuty)

#define Motor_Limit_Speed               _Q15(MOTOR_SPEED_LIMIT_RPM / MOTOR_SPEED_BASE)
#define Motor_Stop_Speed                _Q15(MOTOR_SPEED_STOP_RPM  / MOTOR_SPEED_BASE)

#define Motor_Stall_Min_Speed           _Q15(MOTOR_SPEED_STAL_MIN_RPM / MOTOR_SPEED_BASE)
#define Motor_Stall_Max_Speed           _Q15(MOTOR_SPEED_STAL_MAX_RPM / MOTOR_SPEED_BASE)

#define VC1                            (1.0)                                   // 电压补偿系数
#define RV                             ((RV1 + RV2 + RV3) / RV3)               // 分压比
#define HW_BOARD_VOLT_MAX              (HW_ADC_REF * RV)                       // (V)  ADC可测得的最大母线电压

/*Current Calib:enable or disable*/
#define Disable                         (0)
#define Enable                          (1)
#define CalibENDIS                      (Enable)

#define Calib_Time                     (1000)                                  // 校正次数，固定1000次，单位:次

#define QOUTMAXVALUE                   I_Value(QOUTCURRENT)

#define ATT_COEF                       (0.85)                                  // 无需改动

#define AMP0GAIN_2x                     (1)                                     // 芯片内部2倍放大
#define AMP0GAIN_4x                     (2)                                     // 芯片内部4倍放大
#define AMP0GAIN_8x                     (3)                                     // 芯片内部8倍放大
#define AMP0GAIN_16x                    (4)                                     // 芯片内部16倍放大
#define AMP0GAIN_32x                    (5)                                     // 芯片内部32倍放大

#define Inter_AMP0                      (0)                                     // 内部运放0
#define Exter_AMP0                      (1)                                     // 外部运放0

#define Inter_VOLTAGE                   (0)
#define Exter_VOLTAGE                   (1)

#define Ratio_12                        (0)
#define Ratio_6_5                       (1)

#define Single_Resistor                 (0)                                     // 单电阻电流采样模式
#define Double_Resistor                 (1)                                     // 双电阻电流采样模式

#define NoTailWind                      (0)                                     // 无逆风顺风判断
#define TailWind                        (1)                                     // 逆风顺风判断

#define OUTLoop_Disable                (0)                                     // 关闭外环
#define OUTLoop_Enable                 (1)                                     // 使能外环

#define POWER_LOOP_CONTROL             (0)                                     //恒功率
#define SPEED_LOOP_CONTROL             (1)                                     //恒转速  

#define SMO                             (0)                                     // SMO ,滑膜估算
#define PLL                             (1)                                     // PLL ,锁相环

#define Hardware_CMP_Protect           (1)                                     // Hardware  overcurrent protection enable 
#define Hardware_Protect_Disable       (0)                                     // 硬件过流保护禁止，用于安规实验  

#define NoTailWind                      (0)                                     // 没有顺逆风
#define BEMFMethod                      (1)                                     // BEMF方法
#define FOCMethod                       (2)                                     // FOC计算方法


#define PWMMODE                         (0)                                     // PWM调速
#define SREFMODE                        (1)                                     // 模拟调速
#define NONEMODE                        (2)                                     // 直接给定值，不调速


#define IPMtest                         (0)                                     // IPM测试或者MOS测试，MCU输出固定占空比
#define NormalRun                       (1)                                     // 正常按电机状态机运行

#define CW                              (0)
#define CCW                             (1)

#define Open_Start                      (0)                                     // 开环强拖启动
#define Omega_Start                     (1)                                     // Omega启动
#define Open_Omega_Start                (2)                                     // 先开环启，后Omega启动
#define PLL_Start                       (3)                                     // 先开环启，后Omega启动

#endif