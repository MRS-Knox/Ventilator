#ifndef __LowSpeedObserver_H_
#define __LowSpeedObserver_H_

#define LowSpeedObserver_K1              _Q12(3.5)                                       //反电动势与速度比例系数
#define LowSpeedObserver_K2              _Q15(0.05)                                      //转速更新系数
#define LowSpeedObserver_K3              _Q8(40.0/16.0)                                  //反电动势估算角度信任系数
#define LowSpeedObserverDeltaTheta_K     _Q15(TPWM_VALUE * MAX_OMEG_RAD_SEC / 3.1415926) //角度积分系数
#define BEMFFliter_Count                 (3000)                                          //滤波计数值
#define Init_gain                        (200)                                           //速度初始增益
#define Angle_Com                        (1000)                                          //角度补偿
#define ForcedSpeed_1                    (250)                                           //强制速度1
#define ForcedSpeed_2                    (270)                                           //强制速度2
#define ForcedSpeedLoop_Period           (100)                                           //强制速度补偿值更新周期
#define ForcedSpeed_NegativeValue1       (-500)                                          //强制速度补偿值最小值1

typedef union
{
    int32 s32;
    int16 s16[2];
}s32tos16;
typedef struct
{

    int16 EMF;
    int16 EMF_OMEGA;
    int16 EMF_OMEGA_LPF;
    int16 EMF_OMEGA_LPF_LSB;
    int16 Observer_Count;
    int16 Switch_Count;
    uint8 windstate;
    int16 EALP;
    int16 EBET;
    
    int16 MulHigh;
    int16 MulLow;
    
    int16 AO_THTEA;
    
    int16 OMEGALPF;
    int16 OMEGALPF_LSB;
    s32tos16 LowSpeedObserverOmega_32;
    int16 omega;
    int16 ABSOMEGA;
    int16 ABSOMEGA1;
    int16 AO_DELTAOMEGA;
    
    int16 LowSpeedObserverOmegaErr;

    s32tos16 LowSpeedObserverOmegaDelta_32;

    s32tos16 LowSpeedObserverDeltaTheta_32;
    int16 Lowspeed_adjust;
    int16 count1;
    int16 count2;
    s32tos16 LowSpeedObserverTheta_32;
    int16 ThetaDelta;
    uint16 Smoothswith_Count;
    uint8 state;   
    
}MCLowSpeedObserver;

typedef struct
{
    int16 LowSpeedObserverK1;
    int16 LowSpeedObserverK2;
    int16 LowSpeedObserverK3;
    int16 LowSpeedObserverDeltaThetaK;
    int16 AngleCom;
    int16 Initgain;
    int16 ForcedSpeed1;
    int16 ForcedSpeed2;
    int16 ForcedSpeedLoopPeriod;
    int16 ForcedSpeedNegativeValue1;
    int16 BEMFLPFK;
    int16 BEMFFliterCount;
}StartupVar;




extern StartupVar xdata Startup;
extern MCLowSpeedObserver data LowSpeedObserver;
extern void LowSpeedObserverInit(void);
extern void StartupState(void);
extern void MCLowSpeedObserverFunction(void);


#endif