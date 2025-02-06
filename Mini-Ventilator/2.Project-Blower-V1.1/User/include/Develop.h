/**
 * @copyright (C) COPYRIGHT 2022 Fortiortech Shenzhen
 * @file      Develop.h
 * @author    Fortiortech  Appliction Team
 * @date      2022-07-13
 * @brief      This file contains Advanced Applications parameter used for Motor Control.
 */
/* -----Define to prevent recursive inclusion----- */
#ifndef __Develop_H_
#define __Develop_H_


/* -----MCU clock frequency----- */
#define MCU_CLOCK                       (24.0)                                  ///< (MHz) 主频
           
/* -----Private define----- */
#define SystemPowerUpTime               (10000)                                 ///< 上电等待时间，cpu计数时钟

/* -----Minimum sampling window in double resistacne current sampling mode----- */
#define DLL_TIME                        (0.2)                                   ///< (μs) 双电阻最小脉宽设置(us),建议值为死区时间值+0.2us以上

/* -----Sampling masking time in triple resistance sampling mode----- */
#define OVERMOD_TIME                    (2.0)                                   ///< (μs) 三电阻过调制时间(us)，建议值2.0

/* -----Deadtime compensation value----- */
#define DT_TIME                         (0.0)                                   ///< (μs) 死区补偿时间(us)，适用于双电阻和三电阻，建议值是1/2死区时间

/* -----*Narrow pulse elimination----- */
#define GLI_TIME                        (0.0)                                   ///< (μs) 桥臂窄脉宽消除(us),建议值0.5

/* -----Observer parameter at running state----- */
#define OBS_KSLIDE                     _Q15(0.85)                               ///< SMO算法里的滑膜增益值
#define E_BW                           (300.0)                                  ///< PLL算法里的反电动势滤波值
  
/* -----Deadtime Parameter----- */
#define PWM_LOAD_DEADTIME               (PWM_DEADTIME * MCU_CLOCK)              ///< 死区设置值

#define PWM_OVERMODULE_TIME             (OVERMOD_TIME * MCU_CLOCK / 2)          ///< 过调制时间
#define PWM_DLOWL_TIME                  (DLL_TIME * MCU_CLOCK / 2)              ///< 下桥臂最小时间

/* -----Single resistance current sampling parameter----- */
#define PWM_TS_LOAD                     (uint16)(_Q16 / PWM_CYCLE * MIN_WIND_TIME / 16)                   ///< 单电阻采样设置值
#define PWM_DT_LOAD                     (uint16)(_Q16 / PWM_CYCLE * DT_TIME / 16)                         ///< 死区补偿值
#define PWM_TGLI_LOAD                   (uint16)(_Q16 / PWM_CYCLE * (GLI_TIME) / 16)                      ///< 最小脉冲


#define DAC_OvercurrentValue            (uint8)(_Q8(I_ValueX(OverHardcurrentValue)) +(0x7F))

#define Align_Theta                     _Q15((float)Align_Angle / 180.0)

#define BASE_FREQ                       ((MOTOR_SPEED_BASE / 60) * Pole_Pairs)   ///< 基准频率

/* -----OutLoop increment----- */
#if (Motor_Speed_Control_Mode == SPEED_LOOP_CONTROL)

    #define Motor_Speed_Inc                 _Q15(SPEED_INC / MOTOR_SPEED_BASE)
    #define Motor_Speed_Dec                 _Q15(SPEED_DEC / MOTOR_SPEED_BASE)

#elif (Motor_Speed_Control_Mode == POWER_LOOP_CONTROL)

    #define Motor_Speed_Inc                 SPEED_INC
    #define Motor_Speed_Dec                 SPEED_DEC

#endif

#define GetCurrentOffsetValueLow     (16383-GetCurrentOffsetValue)                                                      ///< 偏置电压差值低点 
#define GetCurrentOffsetValueHigh    (16383+GetCurrentOffsetValue)                                                      ///< 偏置电压差值高点 

/* -----obsever parameter set value----- */
#define MAX_BEMF_VOLTAGE                ((MOTOR_SPEED_BASE*Ke)/(1000.0))
#define MAX_OMEG_RAD_SEC                ((float)(_2PI*BASE_FREQ))
#define OBS_K1T                         _Q15(LD/(LD+RS*TPWM_VALUE))
#define OBS_K2T                         _Q13((TPWM_VALUE/(LD+RS*TPWM_VALUE))*(HW_BOARD_VOLTAGE_BASE_Start/HW_BOARD_CURR_BASE))
#define OBS_K2T_SPEED9                  OBS_K2T * 1.3  
#define OBS_K2T_SPEED8                  OBS_K2T * 1.0  
#define OBS_K2T_SPEED7                  OBS_K2T * 1.0    
#define OBS_K2T_SPEED6                  OBS_K2T * 1.0 
#define OBS_K2T_SPEED5                  OBS_K2T * 1.0  
#define OBS_K2T_SPEED4                  OBS_K2T * 1.0  
#define OBS_K2T_SPEED3                  OBS_K2T * 1.0  
#define OBS_K2T_SPEED2                  OBS_K2T * 1.0    
#define OBS_K2T_SPEED1                  OBS_K2T * 0.8  


#define OBS_K2T_SMO                     _Q13((TPWM_VALUE/(LD+RS*TPWM_VALUE))*1.4*(HW_BOARD_VOLTAGE_BASE_Start/HW_BOARD_CURR_BASE))
#define OBS_K2T_Actual                  _Q13(1.732*(TPWM_VALUE/(LD+RS*TPWM_VALUE))*(HW_BOARD_VOLTAGE_BASE/HW_BOARD_CURR_BASE))
#define OBS_K3T                         _Q15((TPWM_VALUE/(LD+RS*TPWM_VALUE))*(MAX_BEMF_VOLTAGE/HW_BOARD_CURR_BASE))
#define OBS_K3T_SPEED9                  OBS_K3T * 1.2
#define OBS_K3T_SPEED8                  OBS_K3T * 1.0 
#define OBS_K3T_SPEED7                  OBS_K3T * 1.0 
#define OBS_K3T_SPEED6                  OBS_K3T * 1.0
#define OBS_K3T_SPEED5                  OBS_K3T * 1.0 
#define OBS_K3T_SPEED4                  OBS_K3T * 1.0 
#define OBS_K3T_SPEED3                  OBS_K3T * 0.80 
#define OBS_K3T_SPEED2                  OBS_K3T * 0.40
#define OBS_K3T_SPEED1                  OBS_K3T * 0.26 

#define OBS_K4T                          _Q15(((LD-LQ)*TPWM_VALUE*MAX_OMEG_RAD_SEC)/(LD+RS*TPWM_VALUE))

#define OBSW_KP_GAIN                     _Q12(2*_2PI*ATT_COEF*ATO_BW/BASE_FREQ)
#define OBSW_KI_GAIN                     _Q15(_2PI*ATO_BW*ATO_BW*TPWM_VALUE/BASE_FREQ)

#define OBSW_KP_GAIN_RUN                 _Q12(2*_2PI*ATT_COEF*ATO_BW_RUN/BASE_FREQ)
#define OBSW_KI_GAIN_RUN                 _Q15(_2PI*ATO_BW_RUN*ATO_BW_RUN*TPWM_VALUE/BASE_FREQ)

#define OBSW_KP_GAIN_RUN1                _Q12(2*_2PI*ATT_COEF*ATO_BW_RUN1/BASE_FREQ)
#define OBSW_KI_GAIN_RUN1                _Q15(_2PI*ATO_BW_RUN1*ATO_BW_RUN1*TPWM_VALUE/BASE_FREQ)
 
#define OBSW_KP_GAIN_RUN2                _Q12(2*_2PI*ATT_COEF*ATO_BW_RUN2/BASE_FREQ)
#define OBSW_KI_GAIN_RUN2                _Q15(_2PI*ATO_BW_RUN2*ATO_BW_RUN2*TPWM_VALUE/BASE_FREQ)

#define OBSW_KP_GAIN_RUN3                _Q12(2*_2PI*ATT_COEF*ATO_BW_RUN3/BASE_FREQ)
#define OBSW_KI_GAIN_RUN3                _Q15(_2PI*ATO_BW_RUN3*ATO_BW_RUN3*TPWM_VALUE/BASE_FREQ)

#define OBSW_KP_GAIN_RUN4                _Q12(2*_2PI*ATT_COEF*ATO_BW_RUN4/BASE_FREQ)
#define OBSW_KI_GAIN_RUN4                _Q15(_2PI*ATO_BW_RUN4*ATO_BW_RUN4*TPWM_VALUE/BASE_FREQ)

#define OBSW_KP_GAIN_RUNSMO              _Q12(2*_2PI*ATT_COEF*ATO_BW_RUNSMO/BASE_FREQ)
#define OBSW_KI_GAIN_RUNSMO              _Q15(_2PI*ATO_BW_RUN4*ATO_BW_RUNSMO*TPWM_VALUE/BASE_FREQ)

#define OBS_FBASE                        _Q15(BASE_FREQ*TPWM_VALUE)        
#define OBS_KLPF                         _Q15(_2PI*BASE_FREQ*TPWM_VALUE)   
#define SPEED_KLPF                       _Q15(_2PI*SPD_BW*TPWM_VALUE)       
#define OBS_EA_KS                        _Q15((2*MOTOR_SPEED_SMOMIN_RPM*_2PI*BASE_FREQ*TPWM_VALUE)/MOTOR_SPEED_BASE)     
 
#define OBSE_PLLKP_GAIN                  _Q11(((2*ATT_COEF*_2PI*E_BW*LD - RS)*HW_BOARD_CURR_BASE)/HW_BOARD_VOLTAGE_BASE)
#define OBSE_PLLKI_GAIN                  _Q11((_2PI*E_BW*_2PI*E_BW*LD*TPWM_VALUE*HW_BOARD_CURR_BASE)/HW_BOARD_VOLTAGE_BASE)

#define SPEED_KLPF_WIND                  _Q15(_2PI*SPD_BW_Wind*TPWM_VALUE)           
#define OBSW_KP_GAIN_WIND                _Q12(2*_2PI*ATT_COEF*ATO_BW_Wind/BASE_FREQ)
#define OBSW_KI_GAIN_WIND                _Q15(_2PI*ATO_BW_Wind*ATO_BW_Wind*TPWM_VALUE/BASE_FREQ)

/* -----Motor speed parameter----- */
#define MOTOR_OPEN_ACC                 (3.0)                                   
#define MOTOR_OPEN_ACC_MIN             (0.0)                                   
#define MOTOR_OPEN_ACC_CNT             (100.0)                                 
#define MOTOR_OPEN_ACC_CYCLE           (20)                                    


/*SVPWM mode*/
#define SVPWM_5_Segment                 (0)                                    
#define SVPWM_7_Segment                 (1)                                     
#define SVPMW_Mode                      (SVPWM_7_Segment)

/*double resistor sample mode*/
#define DouRes_1_Cycle                  (0)                                   ///< 1 周期采样完 ia, ib
#define DouRes_2_Cycle                  (1)                                   ///< 交替采用ia, ib, 2周期采样完成
#define DouRes_Sample_Mode              (DouRes_1_Cycle)

/*PLL start parameters*/
#define DQKP_observer                 _Q12(1.0)                               ///< KP of current loop during angle estimator switch
#define DQKI_observer                 _Q15(0.04)                              ///< KI of current loop during angle estimator switch
#define LD_PLL                        (0.00003)
#define V_factor                      _Q15(1.0*(2*TPWM_VALUE/LD_PLL)*((UDC_rated/1.732)/HW_BOARD_CURR_BASE))
#define V_factor1                     _Q13(0.5*PLL_BEMF_BASE*(2*TPWM_VALUE/LD_PLL)*((UDC_rated/1.732)/HW_BOARD_CURR_BASE))
#define V_factor1_Max                 _Q13(1.5*PLL_BEMF_BASE*(2*TPWM_VALUE/LD_PLL)*((UDC_rated/1.732)/HW_BOARD_CURR_BASE))
#define PLLKP_GAIN                    _Q11(((2*ATT_COEF*_2PI*E_BW_PLL*LD_PLL)*HW_BOARD_CURR_BASE)/(UDC_rated/1.732))
#define PLLKI_GAIN                    _Q14((_2PI*E_BW_PLL*_2PI*E_BW_PLL*LD_PLL*2*TPWM_VALUE*HW_BOARD_CURR_BASE)/(UDC_rated/1.732))
#define U_BASE                        _Q15(UDC_rated / HW_BOARD_VOLT_MAX)
#define CLOSE_SPEED                   (Close_Speed*Pole_Pairs*32.0*65536.0/(PWM_FREQUENCY*500.0*60.0))
#endif