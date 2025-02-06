/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : AddFunction.c
    Author         : Fortiortech  Appliction Team
    Version        : V1.0
    Date           : 2021-12-28
    Description    : This file contains  function used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <FU68xx_4.h>
#include <Myproject.h>

FaultStateType       xdata   mcFaultSource;
PWMINPUTCAL          xdata   mcPwmInput;
FGout                xdata   FG;
FOCCTRL              xdata   mcFocCtrl;
ADCSample            xdata   AdcSampleValue;
ONVarible            idata   ONOFFTest;
MCRAMP               xdata   mcSpeedRamp;
MotorFRTypeDef       xdata   mcFRState;
int16                xdata   VSP;
int16                xdata   VSP_LSB;
Variable             xdata   Vari;
CurrentOffset        xdata   mcCurOffset;
MotStaType           xdata   mcState;
MotStaM              xdata   McStaSet;
UART2_Struct         xdata   uart2_data_t;
Blower_State		 xdata   blower_state_t;
/* -------------------------------------------------------------------------------------------------
    Function Name  : FaultProcess
    Description    : 保护处理函数，关闭FOC输出，同时将状态变为mcFault
    Date           : 2021-12-28
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void FaultProcess(void)
{
    ClrBit(DRV_CR, FOCEN);
    ClrBit(DRV_CR, DRVEN);
    MOE     = 0;
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : Fault_Detection
    Description    : 保护函数
    Date           : 2021-12-28
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Fault_Detection(void)
{
    /* -----软件过流保护----- */
    Fault_Overcurrent();
    #if (VoltageProtectEnable)          //过压保护
    {
        Fault_OverUnderVoltage();
    }
    #endif
    #if (StartProtectEnable)            //启动保护使能
    {
        Fault_Start();
    }
    #endif
    #if (StallProtectEnable)            //堵转保护使能
    {
        Fault_Stall();
    }
    #endif
    #if (PhaseLossProtectEnable)        //缺相保护使能
    {
        Fault_phaseloss();
    }
    #endif
    /* -----保护恢复----- */
    Fault_Recovery();
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : ONOFF_Starttest
    Description    : 启停测试
    Date           : 2021-12-05
    Parameter      : h_test: [输入/出]
------------------------------------------------------------------------------------------------- */
void ONOFF_Starttest(void)
{
    if (ONOFFTest.ONOFF_Flag == 1)
    {
        ONOFFTest.ON_Count++;
        
        if (ONOFFTest.ON_Count > StartON_Time)
        {
            ONOFFTest.ON_Count        = 0;
            ONOFFTest.ONOFF_Times++;
            ONOFFTest.ONOFF_Flag      = 0;
            mcSpeedRamp.FlagONOFF   = 0;
            mcSpeedRamp.TargetValue = 0;
        }
    }
    else
    {
        if (mcState != mcFault)
        {
            ONOFFTest.OFF_Count++;
            
            if (ONOFFTest.OFF_Count >= StartOFF_Time)
            {
                ONOFFTest.OFF_Count       = 0;
                ONOFFTest.ONOFF_Flag      = 1;
                mcSpeedRamp.FlagONOFF   = 1;
                mcSpeedRamp.TargetValue = MOTOR_ONOFF_Speed;
            }
        }
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Speed_response
    Description    : 速度响应函数，可根据需求加入控制环，如恒转矩控制、恒转速控制、恒功率控制
    Date           : 2021-12-05
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Speed_response(void)
{
    if ((mcState == mcRun) || (mcState == mcStop))
    {
        switch (mcFocCtrl.CtrlMode)
        {
            case 0:  //开环
            {
                #if (Open_Start_Mode == PLL_Start)
            
                if ((FOC__EOME > Motor_Loop_Speed) && (PLLfunction.PLLFunctionFlag > 1))
                #else
                if (FOC__EOME > Motor_Loop_Speed)
                #endif
                {
                    mcFocCtrl.CtrlMode = 1;                                 //切闭环
                    FOC_DQKP = DQKP;
                    FOC_DQKI = DQKI;
                    FOC_IDREF = 0;
                    FOC__THECOMP    = _Q15(2.0 / 180.0);                    // SMO 估算补偿角
                    mcFocCtrl.TorqueLoopTime = SPEED_LOOP_TIME;
                    PI3_Init();
                    #if (Motor_Speed_Control_Mode == SPEED_LOOP_CONTROL)
                    {
                        mcSpeedRamp.ActualValue = Motor_Loop_Speed;
                    }
                    #elif (Motor_Speed_Control_Mode == POWER_LOOP_CONTROL)
                    {
                        mcSpeedRamp.ActualValue = mcFocCtrl.Powerlpf;
                    }
                    #endif
                    PI2_UKH = mcFocCtrl.mcIqref;
                    PI3_UKH = mcFocCtrl.mcIqref;
                }
            }
            break;
            
            case 1:  //闭环
            {
                mcFocCtrl.TorqueLoopTime++;
                
                if (mcFocCtrl.TorqueLoopTime > SPEED_LOOP_TIME)
                {
                    mcFocCtrl.TorqueLoopTime = 0;
                    #if (OUTLoop_Mode == OUTLoop_Disable)
                    {
                        mcFocCtrl.mcIqref = FOC_IQREF;
                        
                        if (mcFocCtrl.mcIqref < QOUTMAXVALUE)
                        {
                            mcFocCtrl.mcIqref += QOUTINC;
                            
                            if (mcFocCtrl.mcIqref > QOUTMAXVALUE)
                            {
                                mcFocCtrl.mcIqref = QOUTMAXVALUE;
                            }
                            
                            FOC_IQREF = mcFocCtrl.mcIqref;
                        }
                        else if (mcFocCtrl.mcIqref > QOUTMAXVALUE)
                        {
                            mcFocCtrl.mcIqref -= QOUTINC;
                            
                            if (mcFocCtrl.mcIqref < QOUTMAXVALUE)
                            {
                                mcFocCtrl.mcIqref = QOUTMAXVALUE;
                            }
                            
                            FOC_IQREF = mcFocCtrl.mcIqref;
                        }
                        else
                        { FOC_IQREF = mcFocCtrl.mcIqref; }
                    }
                    #elif (OUTLoop_Mode == OUTLoop_Enable)
                    {
                        mc_ramp();
                        #if (Motor_Speed_Control_Mode == SPEED_LOOP_CONTROL) //速度环
                        {
                            mcFocCtrl.mcIqref = HW_One_PI3(mcSpeedRamp.ActualValue - mcFocCtrl.SpeedFlt);
                            FOC_IQREF = mcFocCtrl.mcIqref;
                        }
                        #elif (Motor_Speed_Control_Mode == POWER_LOOP_CONTROL) //功率环
                        {
                            mcFocCtrl.mcIqref = HW_One_PI3(mcSpeedRamp.ActualValue - mcFocCtrl.Powerlpf);
                            FOC_IQREF = mcFocCtrl.mcIqref;
                        }
                        #endif
                    }
                    #endif
                    
                    if (FOC_EK2 < OBS_K2T_Actual - 10)
                    {
                        mcFocCtrl.Smo_EK2  = FOC_EK2;
                        mcFocCtrl.Smo_EK2 += 10;
                        FOC_EK2 = mcFocCtrl.Smo_EK2;
                    }
                    else if (FOC_EK2 > OBS_K2T_Actual + 10)
                    {
                        mcFocCtrl.Smo_EK2  = FOC_EK2;
                        mcFocCtrl.Smo_EK2 -= 10;
                        FOC_EK2 = mcFocCtrl.Smo_EK2;
                    }
                    else
                    {
                        FOC_EK2 = OBS_K2T_Actual;
                    }
                }
            }
            break;
        }
    }
}


/* -------------------------------------------------------------------------------------------------
    Function Name  : mc_ramp
    Description    : 爬坡函数
    Date           : 2021-12-05
    Parameter      : hSpeedramp: [输入/出]
------------------------------------------------------------------------------------------------- */
void mc_ramp(void)
{
    if ( --mcSpeedRamp.DelayCount < 0)
    {
        mcSpeedRamp.DelayCount = mcSpeedRamp.DelayPeriod;
        
        if (mcSpeedRamp.ActualValue < mcSpeedRamp.TargetValue)
        {
            if (mcSpeedRamp.ActualValue + Motor_Speed_Inc < mcSpeedRamp.TargetValue)
            {
                mcSpeedRamp.ActualValue += Motor_Speed_Inc;
            }
            else
            {
                mcSpeedRamp.ActualValue = mcSpeedRamp.TargetValue;
            }
        }
        else
        {
            if (mcSpeedRamp.ActualValue - Motor_Speed_Dec > mcSpeedRamp.TargetValue)
            {
                mcSpeedRamp.ActualValue -= Motor_Speed_Dec;
            }
            else
            {
                mcSpeedRamp.ActualValue = mcSpeedRamp.TargetValue;
            }
        }
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : HW_One_PI3
    Description    : PI3控制
    Date           : 2021-12-05
    Parameter      : Xn1: [输入/出]
------------------------------------------------------------------------------------------------- */
int16 HW_One_PI3(int16 Xn1)
{
    PI3_EK = Xn1;
    SetBit(PI_CR, PI3STA);
    
    while (ReadBit(PI_CR, PIBSY));
    
    return PI3_UKH;
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : HW_one_PI2
    Description    : PI2控制
    Date           : 2021-12-05
    Parameter      : Xn1: [输入/出]
------------------------------------------------------------------------------------------------- */
int16 HW_one_PI2(int16 Xn1)
{
    PI2_EK = Xn1;
    SetBit(PI_CR, PI2STA);
    
    while (ReadBit(PI_CR, PIBSY));
    
    return PI2_UKH;
}

/*---------------------------------------------------------------------------*/
/* Name     :   void VSPSample(void)
/* Input    :   NO
/* Output   :   NO
/* Description: VSP采样
/*---------------------------------------------------------------------------*/
void VSPSample(void)
{
    /*****VREF的采样获取值并滤波******/
    AdcSampleValue.ADCSpeed = (ADC7_DR);        //采集对应端口
    LPF_MDU(AdcSampleValue.ADCSpeed, 30, VSP, VSP_LSB);   //低通滤波
    
    if (VSP > ONPWMDuty)
    {
        mcSpeedRamp.VSP_Off_count = 0;
        
        if (++mcSpeedRamp.VSP_On_count > 100)
        {
            mcSpeedRamp.VSP_On_count = 100;
            mcSpeedRamp.FlagONOFF = 1;
        }
    }
    else if (VSP < OFFPWMDuty)
    {
        mcSpeedRamp.VSP_On_count = 0;
        
        if (++mcSpeedRamp.VSP_Off_count > 100)
        {
            mcSpeedRamp.VSP_Off_count = 100;
            mcSpeedRamp.FlagONOFF = 0;
        }
    }
    
    //转速曲线计算
    if (mcSpeedRamp.FlagONOFF == 1) //
    {
        #if (Motor_Speed_Control_Mode == SPEED_LOOP_CONTROL)
        {
            if (VSP <= MINPWMDuty)   //最小转速运行
            {
                mcSpeedRamp.TargetValue = Motor_Min_Speed;
            }
            else if (VSP < MAXPWMDuty) //调速
            {
                mcSpeedRamp.TargetValue = Motor_Min_Speed + SPEED_K * (VSP - MINPWMDuty);
            }
            else                    //最大转速运行
            {
                mcSpeedRamp.TargetValue = Motor_Max_Speed;
            }
        }
        #elif (Motor_Speed_Control_Mode == POWER_LOOP_CONTROL)
        {
        
            if (VSP <= MINPWMDuty)   //最小转速运行
            {
                mcSpeedRamp.TargetValue = MOTOR_POWER_MIN_RPM;
            }
            else if (VSP < MAXPWMDuty) //调速
            {
                mcSpeedRamp.TargetValue = MOTOR_POWER_MIN_RPM + POWER_K * (VSP - MINPWMDuty);
            }
            else             //最大转速运行
            {
                mcSpeedRamp.TargetValue = MOTOR_POWER_MAX_RPM;
            }
        }
        #endif
    }
    else
    {
        mcSpeedRamp.TargetValue = 0;
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : StarRampDealwith
    Description    : 电机启动爬坡函数处理
    Date           : 2021-12-05
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void StarRampDealwith(void)
{
    if (mcFocCtrl.CtrlMode == 0)
    {
        #if (Open_Start_Mode == PLL_Start)
        {
            RampFucntion();
        }
        #endif
        
        if ( mcFocCtrl.mcIqref < IQ_Start_CURRENT)
        {
            mcFocCtrl.mcIqref  += 10;
            FOC_IQREF = mcFocCtrl.mcIqref;
        }
    }
    
    if ((mcState == mcRun))
    {
        if (mcFocCtrl.State_Count == 1000)
        {
            FOC_EKP = OBSW_KP_GAIN_RUN;              // 估算器里的PI的KP
            FOC_EKI = OBSW_KI_GAIN_RUN;              // 估算器里的PI的KI
        }
        else if (mcFocCtrl.State_Count == 800)
        {
            FOC_EKP = OBSW_KP_GAIN_RUN1;             // 估算器里的PI的KP
            FOC_EKI = OBSW_KI_GAIN_RUN1;             // 估算器里的PI的KI
        }
        else if (mcFocCtrl.State_Count == 600)
        {
            FOC_EKP = OBSW_KP_GAIN_RUN2;             // 估算器里的PI的KP
            FOC_EKI = OBSW_KI_GAIN_RUN2;             // 估算器里的PI的KI
        }
        else if (mcFocCtrl.State_Count == 400)
        {
            FOC_EKP = OBSW_KP_GAIN_RUN3;             // 估算器里的PI的KP
            FOC_EKI = OBSW_KI_GAIN_RUN3;             // 估算器里的PI的KI
        }
        else if (mcFocCtrl.State_Count == 200)
        {
            FOC_EKP = OBSW_KP_GAIN_RUN4;             // 估算器里的PI的KP
            FOC_EKI = OBSW_KI_GAIN_RUN4;             // 估算器里的PI的KI
        }
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : PWMInputCapture
    Description    : 输入PWM处理
    Date           : 2021-12-05
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void PWMInputCapture(void)
{
    if (mcPwmInput.PWMUpdateFlag == 1)
    {
        DivQ_L_MDU(mcPwmInput.PWMCompare >> 1, 0x0000, mcPwmInput.PWMARR, mcPwmInput.PWMDuty);
        
        if ((mcPwmInput.PWMDuty > ONPWMDuty))                                                         //大于开机占空比开机
        {
            if (++mcPwmInput.OnCount >= 30)
            {
                mcPwmInput.OnCount = 30;                                                              //开机延时
                mcSpeedRamp.FlagONOFF = 1;
            }
        }
        else
        {
            mcPwmInput.OnCount = 0;
        }
        
        if (mcPwmInput.PWMDuty < OFFPWMDuty)                                                          //小于关机占空比关机
        {
            if (++mcPwmInput.OffCount >= 20)
            {
                mcPwmInput.OffCount = 20;                                                             //关机延时
                mcSpeedRamp.FlagONOFF = 0;
            }
        }
        else
        {
            mcPwmInput.OffCount = 0;
        }
        
        if (mcSpeedRamp.FlagONOFF == 1)
        {
            #if (Motor_Speed_Control_Mode == SPEED_LOOP_CONTROL) //速度环
            {
                if (mcPwmInput.PWMDuty < MINPWMDuty)
                {
                    mcSpeedRamp.TargetValue = Motor_Min_Speed;
                }
                else if (mcPwmInput.PWMDuty <= MAXPWMDuty)
                {
                    mcSpeedRamp.TargetValue = Motor_Min_Speed + SPEED_K * (mcPwmInput.PWMDuty - MINPWMDuty) ;
                }
                else
                { mcSpeedRamp.TargetValue = Motor_Max_Speed; }
                
            }
            #elif (Motor_Speed_Control_Mode == POWER_LOOP_CONTROL) //功率环
            {
                if (mcPwmInput.PWMDuty < MINPWMDuty)
                {
                    mcSpeedRamp.TargetValue = MOTOR_POWER_MIN_RPM;
                }
                else if (mcPwmInput.PWMDuty <= MAXPWMDuty)
                {
                    mcSpeedRamp.TargetValue = MOTOR_POWER_MIN_RPM + POWER_K * (mcPwmInput.PWMDuty - MINPWMDuty) ;
                }
                else
                { mcSpeedRamp.TargetValue = MOTOR_POWER_MAX_RPM; }
            }
            #endif
        }
        else
        {
            mcSpeedRamp.TargetValue = 0;
        }
    }
    
    mcPwmInput.PWMUpdateFlag = 0;
}


void Fault_GetCurrentOffset(void)
{
    if (mcCurOffset.OffsetFlag == 1)
    {
        #if (Shunt_Resistor_Mode == Single_Resistor)                   //单电阻模式
        {
            if ((mcCurOffset.Iw_busOffset < GetCurrentOffsetValueLow) || (mcCurOffset.Iw_busOffset > GetCurrentOffsetValueHigh))
            {
                mcFaultSource = FaultGetOffset;
            }
        }
        #elif (Shunt_Resistor_Mode == Double_Resistor)                 //双电阻模式
        {
            if ((mcCurOffset.IuOffset < GetCurrentOffsetValueLow) || (mcCurOffset.IuOffset > GetCurrentOffsetValueHigh)
                || (mcCurOffset.IvOffset < GetCurrentOffsetValueLow) || (mcCurOffset.IvOffset > GetCurrentOffsetValueHigh))
            {
                mcFaultSource = FaultGetOffset;
            }
        }
        #elif (Shunt_Resistor_Mode == Three_Resistor)                 //三电阻模式
        {
            if ((mcCurOffset.IuOffset < GetCurrentOffsetValueLow) || (mcCurOffset.IuOffset > GetCurrentOffsetValueHigh)
                || (mcCurOffset.IvOffset < GetCurrentOffsetValueLow) || (mcCurOffset.IvOffset > GetCurrentOffsetValueHigh)
                || (mcCurOffset.Iw_busOffset < GetCurrentOffsetValueLow) || (mcCurOffset.Iw_busOffset > GetCurrentOffsetValueHigh))
            {
                mcFaultSource = FaultGetOffset;
            }
        }
        #endif
    }
}
/**
 * @function     Function_1_MS_
*  @brief        1ms中断服务函数
 * @param[in]    None
 * @return       None
 * @date         2024-08-26
*/
void Function_1_MS_(uint32 speed)
{
    SetBit(ADC_CR, ADCBSY);                                                               //使能ADC的DCBUS采样
    Vari.UQ     = FOC__UQ;
    Vari.QMAX   = FOC_QMAX;
    Vari.UD     = FOC__UD;
    Vari.DMAX   = FOC_DMAX;
    Vari.IQ     = FOC__IQ;
    Vari.THETA  = FOC__THETA;
    Vari.ETHETA = FOC__ETHETA;
    Vari.IQREF  = FOC_IQREF;
    
    if (mcState == mcRun)
    {
        mcFocCtrl.CurrentPower = FOC__POW << 3;                                           //左移提高功率值精度
        LPF_MDU(mcFocCtrl.CurrentPower, 1, mcFocCtrl.Powerlpf, mcFocCtrl.Powerlpf_LSB);   //注意低通滤波器系数范围为0---127
        LPF_MDU(FOC__UQ, 1, mcFocCtrl.UQ_lpf, mcFocCtrl.UQ_LSB);
    }
    
    if ((mcState != mcInit) && (mcState != mcReady))
    {
        LPF_MDU(FOC__EOME, 10, mcFocCtrl.SpeedFlt, mcFocCtrl.SpeedFlt_LSB);
        Atan_LPF_MDU(FOC__EALP, FOC__EBET, 10, mcFocCtrl.EsValue, mcFocCtrl.EsValue_LSB);
    }
    else
    {
        mcFocCtrl.SpeedFlt = 0;
    }
    
    Atan_Us_MDU(FOC__EALP, FOC__EBET, mcFocCtrl.EsValue);
    /* -----环路响应----- */
    Speed_response();                                                                      //环路响应，如速度环、转矩环、功率环等
    #if (!StartONOFF_Enable)
    {
        #if (SPEED_MODE == PWMMODE)
        {
            PWMInputCapture();
        }
        #elif (SPEED_MODE == NONEMODE)
        {
            mcSpeedRamp.FlagONOFF   = 1;
            mcSpeedRamp.TargetValue = Motor_Speed(speed);
        }
        #elif ( SPEED_MODE == SREFMODE)
        {
            VSPSample();
        }
        #endif
    }
    #elif (StartONOFF_Enable)
    /* -----启停测试----- */
    ONOFF_Starttest();
    #endif
    #if (VOLTAGEMode ==Inter_VOLTAGE)
    AdcSampleValue.ADCDcbus = ADC14_DR;
    #else
    AdcSampleValue.ADCDcbus = ADC2_DR;
    #endif
    LPF_MDU((AdcSampleValue.ADCDcbus), 50, mcFocCtrl.mcDcbusFlt, mcFocCtrl.mcDcbusFlt_LSB);  //DCbus采样滤波值
    /* -----故障保护函数----- */
    Fault_Detection();
    
    if (mcFocCtrl.State_Count > 0)                                                           //电机状态机时序处理
    {
        mcFocCtrl.State_Count--;
    }
    
    if (BEMFDetect.BEMFTimeCount > 0)
    {
        BEMFDetect.BEMFTimeCount--;
    }
    
    #if (Open_Start_Mode == PLL_Start)
    {
        PLLfunction.UBASE_Factor = (float)(mcFocCtrl.mcDcbusFlt) * 16383 / ((float)(U_BASE));
        
        if (PLLfunction.UBASE_Factor < 10000)
        {
            PLLfunction.UBASE_Factor = 10000;
        }
    }
    #endif
    /* -----电机启动爬坡函数处理----- */
    StarRampDealwith();
    
    if (mcState == mcRun)
    {
        SetBit(TIM4_CR1, T4EN);
    }
    else
    {
        ClrBit(TIM4_CR1, T4EN);
        TIM4__DR = 5000;
        TIM4__ARR = 10000;
        TIM4__CNTR = 0;
    }
}
