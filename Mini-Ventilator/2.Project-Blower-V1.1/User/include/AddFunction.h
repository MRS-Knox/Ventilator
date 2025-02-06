/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : AddFunction.h
    Author         : Lewis.Wang
    Version        : V1.0
    Date           : 2021-12-05
    Description    : This file contains common datas used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

/* Define to prevent recursive inclusion -------------------------------------------------------- */
#ifndef __ADD_FUNCTION_H_
#define __ADD_FUNCTION_H_
#include <FU68xx_4_Type.h>

#define UART_MAX	100
typedef struct{
	unsigned char uart2_recdata[UART_MAX];
	unsigned char uart2_senddata[UART_MAX];
	unsigned char rec_incount;
	unsigned char rec_outcount;
	unsigned char send_incount;
	unsigned char send_outcount;
}UART2_Struct;

typedef struct{
	uint16 set_speed;		//设置转速
	uint16 actual_speed;	//实际转速
	uint16 rec_command;		//接收的命令
	uint8 flag_bloweronoff;	//电机开关标志位
}Blower_State;

typedef struct
{
    int16 UQ;
    int16 UD; 
    int16 QMAX;
    int16 DMAX; 
    int16 IQ;
    int16 IQREF;
    int16 THETA;
    int16 ETHETA;    
}Variable;

typedef struct
{
  uint32  TIMcalc_temp;
  uint32  TIM3_ARR_TEMP;
  uint32  mcSpeedFlt_temp;	
}FGout;

typedef enum
{
    FaultNoSource      = 0,                                                     // 无故障
    FaultHardOVCurrent = 1,                                                     // 硬件过流
    FaultSoftOVCurrent = 2,                                                     // 软件过流
    FaultUnderVoltage  = 3,                                                     // 欠压保护
    FaultOverVoltage   = 4,                                                     // 过压保护
    FaultLossPhase     = 5,                                                     // 缺相保护
    FaultStall         = 6,                                                     // 堵转保护
    FaultStart         = 7,                                                     // 启动保护
    FaultLVW           = 8,                                                     //低电压保护
    FaultGetOffset     = 13,                                                    // 偏置电压保护 
} FaultStateType;

typedef struct
{
    uint16 ADCDcbus;                                                            // 母线电压
    uint16 ADCSpeed;                                                            // 模拟速度
} ADCSample;

typedef struct
{
    uint32 ONOFF_Times;                                                         // 启停次数
    uint16 ON_Count;                                                            // 运行时间计数
    uint16 OFF_Count;                                                           // 停止时间计数
    uint8  ONOFF_Flag;                                                          // 启停测试中启动标志位
} ONVarible;

typedef struct
{
    uint8  CtrlMode;                                                            // 控制模式                                                      
    uint16 CurrentPower;                                                        // 当前功率
    int16  Powerlpf;                                                            // 功率滤波后的值
    int16  Powerlpf_LSB;                                                        // 功率滤波后的值
	  int16  UQ_LSB;
    int16  UQ_lpf;
    int16  mcIqref;                                                             // Q轴给定电流
    int16  SpeedFlt;                                                            // 当前速度滤波后的值
    int16  SpeedFlt_LSB;                                                        // 当前速度滤波后的值
    uint16 EsValue;                                                             // 当前FOC_ESQU滤波后的值
    uint16 Smo_EK2;                                                             // EK2的调试值
    uint16 EsValue_LSB;                                                         // 当前FOC_ESQU滤波后的值
    uint16 TorqueLoopTime;                                                      // 转矩环时间
    uint16 State_Count;                                                         // 电机各个状态的时间计数
    uint16 TailWindOFTimeCnt; 
    uint8 TailWindStatus;
	  uint16 mcDcbusFlt;                                                          // 母线电压
    int16  mcDcbusFlt_LSB;                                                      // 当前母线电压滤波后的值
}FOCCTRL;

typedef struct
{
    int16  TargetValue;
    int16  ActualValue;
    int16  IncValue;
    int16  DecValue;
    int16  DelayCount;
    int16  DelayPeriod;
    int8   FlagONOFF;
    uint16 VSP_On_count;
    uint16 VSP_Off_count;
}MCRAMP;


typedef struct
{
    uint16 PWMCompare;               // 比较值
    uint16 PWMARR;                   // 周期值
    uint16 PWMDuty;                  // PWM占空比，Q15格式
    int16  PWMUpdateFlag;            // PWM新的duty更新
    int16  OnCount;
    int16  OffCount;
}PWMINPUTCAL;

typedef struct
{
    uint8 FRStatus;
    uint8 TargetFR;                  // 设置的目标方向
} MotorFRTypeDef;

typedef struct
{
  int16   IuOffset;                    //Iu的偏置电压
  int32   IuOffsetSum;                 //Iu的偏置电压总和
  int16   IvOffset;                    //Iv的偏置电压
  int32   IvOffsetSum;                 //Iv的偏置电压总和
  int16   Iw_busOffset;                //Iw或Ibus的偏置电压
  int32   Iw_busOffsetSum;             //Iw或Ibus的偏置电压总和
  int16   OffsetCount;                 //偏置电压采集计数
  int8    OffsetFlag;                  //偏置电压结束标志位
}CurrentOffset;

extern CurrentOffset  xdata mcCurOffset;

extern PWMINPUTCAL    xdata mcPwmInput;
extern ONVarible      idata ONOFFTest;
extern FaultStateType xdata mcFaultSource;
extern ADCSample      xdata AdcSampleValue;
extern FOCCTRL        xdata mcFocCtrl;
extern MCRAMP         xdata mcSpeedRamp;
extern int16          xdata VSP;
extern MotorFRTypeDef xdata mcFRState;
extern Variable       xdata Vari;
extern FGout          xdata FG;
extern UART2_Struct   xdata uart2_data_t;
extern Blower_State	  xdata blower_state_t;
extern uint8 g_1mTick;      

extern void   VariablesPreInit(void);
extern void   Fault_Detection(void);
extern void   Speed_response(void);
extern void   mc_ramp(void);
extern void   VSPSample(void);
extern int16  HW_One_PI3(int16 Xn1);
extern void   FaultProcess(void);
extern void   StarRampDealwith(void);
extern void   ONOFF_Starttest(void);
extern void   PWMInputCapture(void);
extern int16 HW_one_PI2(int16 Xn1);
extern void   Function_1_MS_(uint32 speed);
#endif

