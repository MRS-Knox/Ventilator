/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : FocControlFunction.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-27
    Description    : This file contains XX-XX-XX function used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <FU68xx_4.h>
#include <Myproject.h>
int8 Step_count=0;
/* -------------------------------------------------------------------------------------------------
    Function Name  : FOC_Init
    Description    : FOC初始化配置
    Date           : 2021-12-27
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void FOC_Init(void)
{
    /*使能FOC*/
    ClrBit(DRV_CR ,DRVEN);	    
    ClrBit(DRV_CR, FOCEN);
    SetBit(DRV_CR, FOCEN);
    SetBit(FOC_CR0, ESCMS);
    #if (VOLTAGEMode ==Inter_VOLTAGE)
    {
        SetBit(FOC_CR0, UCSEL);
    }
    #else
    {
        ClrBit(FOC_CR0, UCSEL);
    }
    #endif
    SetBit(FOC_CR0, MERRS1);
    SetBit(FOC_CR0, MERRS0);
    FOC_EOMEKLPF    = 0xff;
    FOC_CR1         = 0;                                    // 清零 FOC_CR1
    FOC_CR2         = 0;                                    // 清零 FOC_CR2
    FOC_IDREF       = 0;                                    // 清零 Id
    FOC_IQREF       = 0;                                    // 清零 Iq
    FOC__THETA      = 0;                                    // 清零 角度
    FOC_RTHEACC     = 0;                                    // 清零 爬坡函数的初始加速度
    FOC__RTHESTEP   = 0;                                    // 清零 爬坡速度
    FOC__RTHECNT    = 0;                                    // 清零 爬坡次数
    FOC__THECOMP    = _Q15(0.0 / 180.0);                    // SMO 估算补偿角
    FOC__THECOR     = 0x04;                                 // 误差角度补偿
    /*电流环参数配置*/
    FOC_DMAX        = DOUTMAX;
    FOC_DMIN        = DOUTMIN;
    FOC_QMAX        = QOUTMAX;
    FOC_QMIN        = QOUTMIN;
    /*位置估算参数配置*/
    FOC_EK1         = OBS_K1T;
    FOC_EK2         = OBS_K2T ;
    FOC_EK3         = OBS_K3T ;
    FOC_EK4         = OBS_K4T;
    FOC_FBASE       = OBS_FBASE;
    FOC_OMEKLPF     = SPEED_KLPF;
    FOC_EBMFK       = OBS_KLPF;
    FOC_TGLI        = PWM_TGLI_LOAD;
    /*********PLL或SMO**********/
    #if (EstimateAlgorithm == SMO)
    {
        ClrBit(FOC_CR2, ESEL);
        FOC_KSLIDE    = OBS_KSLIDE;
        FOC_EKLPFMIN  = OBS_EA_KS;
    }
    #elif (EstimateAlgorithm == PLL)
    {
        SetBit(FOC_CR2, ESEL);
        FOC_KSLIDE    = OBSE_PLLKP_GAIN;
        FOC_EKLPFMIN  = OBSE_PLLKI_GAIN;
    }
    #endif //end SVPMW_Mode
    SetBit(FOC_CR1, SVPWMEN);                            // SVPWM模式
    #if (SetDirection == 1)
    {
        SetBit(DRV_CR, DDIR);                            // 反转标志位
    }
    #else
    {
        ClrBit(DRV_CR, DDIR);                            // 反转标志位
    }
    #endif //end IRMODE
    /**过调制**/
    #if (OverModulation == 1)
    {
        SetBit(FOC_CR1, OVMDL);                            // 过调制
    }
    #endif //end OverModulation
    /*单电阻采样；需要最小采样窗,FOC_TRGDLY为0，七段式SVPWM方式*/
    #if (Shunt_Resistor_Mode == Single_Resistor)
    {
        SetReg(FOC_CR1, CSM0 | CSM1, 0x00);
        FOC_TSMIN  = PWM_TS_LOAD;                  // 最小采样窗口
        FOC_TRGDLY = 0x09;                         // 采样时刻在中点，一般考虑开关噪声影响，会设置延迟；
        // 0x0c表示延迟12个clock，提前用反码形式，如0x84表示提前12个clock。
        ClrBit(FOC_CR2, F5SEG);                    // 7段式
        SetReg(CMP_CR1, CMP3MOD0 | CMP3MOD1, 0x00);
    }
    /*双电阻采样，可设置死区补偿值，在下降沿结束前开始采样Ia，配置81*/
    #elif (Shunt_Resistor_Mode == Double_Resistor) // double resistor sample
    {
        SetReg(FOC_CR1, CSM0 | CSM1, CSM0);
        FOC_TSMIN = PWM_DT_LOAD;                   // 死区补偿值
        FOC_TRGDLY = 0x83;                         // ADC采样的时刻，采样时刻在计数器零点附近，83为下降沿结束前3个clock采样Ia，与单电阻不同
        // 01为上升沿开始后第一个clock开始采样。根据实际情况调整。
        FOC_TBLO = PWM_DLOWL_TIME;                 //下桥臂最小脉冲，保证采样
        SetReg(CMP_CR1, CMP3MOD0 | CMP3MOD1, 0x00);
        /*五段式或七段式选择*/
        #if (SVPMW_Mode == SVPWM_7_Segment)
        {
            ClrBit(FOC_CR2, F5SEG);                  // 7段式
        }
        #elif (SVPMW_Mode == SVPWM_5_Segment)
        {
            SetBit(FOC_CR2, F5SEG);                  // 5段式
        }
        #endif
        #if (DouRes_Sample_Mode == DouRes_1_Cycle)
        {
            ClrBit(FOC_CR2, DSS);                    // 7段式
        }
        #elif (DouRes_Sample_Mode == DouRes_2_Cycle)
        {
            SetBit(FOC_CR2, DSS);                    // 5段式
        }
        #endif //end DouRes_Sample_Mode
    }
    /*三电阻采样*/
    #elif (Shunt_Resistor_Mode == Three_Resistor)   // signel resistor sample
    {
        SetReg(FOC_CR1, CSM0 | CSM1, CSM0 | CSM1); // 三电阻
        FOC_TSMIN  = PWM_DT_LOAD; // 死区补偿值
        FOC_TRGDLY = 0x06;        // ADC采样的时刻，采样时刻在计数器零点附近，83为下降沿结束前3个clock采样Ia，与单电阻不同。
        // 01为上升沿开始后第一个clock开始采样。根据实际情况调整。
        SetReg(CMP_CR1, CMP3MOD0 | CMP3MOD1, CMP3MOD0 | CMP3MOD1);
        FOC_TBLO = PWM_OVERMODULE_TIME;             // 过调制电流采样处理的TB脉宽
        /*五段式或七段式选择*/
        #if (SVPMW_Mode == SVPWM_7_Segment)
        {
            ClrBit(FOC_CR2, F5SEG);                             // 7段式
        }
        #elif (SVPMW_Mode == SVPWM_5_Segment)
        {
            SetBit(FOC_CR2, F5SEG);                             // 5段式
        }
        #endif //end SVPMW_Mode
        #if (DouRes_Sample_Mode == DouRes_1_Cycle)
        {
            ClrBit(FOC_CR2, DSS);                               // 7段式
        }
        #elif (DouRes_Sample_Mode == DouRes_2_Cycle)
        {
            SetBit(FOC_CR2, DSS);                               // 5段式
        }
        #endif //end DouRes_Sample_Mode
    }
    #endif  //end Shunt_Resistor_Mode
    /* 使能电流基准校正 */
    #if (CalibENDIS == Enable)
    {
        if (mcCurOffset.OffsetFlag == 1)
        {
            #if (Shunt_Resistor_Mode == Single_Resistor)          // 单电阻校正
            {
                /*set ibus current sample offset*/
                SetReg(FOC_CR2, CSOC0 | CSOC1, 0x00);
                FOC_CSO = mcCurOffset.Iw_busOffset;               // 写入Ibus的偏置
            }
            #elif (Shunt_Resistor_Mode == Double_Resistor)        // 双电阻校正
            {
                /*set ia, ib current sample offset*/
                SetReg(FOC_CR2, CSOC0 | CSOC1, CSOC0);
                FOC_CSO  = mcCurOffset.IuOffset;                  // 写入IA的偏置
            
                SetReg(FOC_CR2, CSOC0 | CSOC1, CSOC1);
                FOC_CSO  = mcCurOffset.IvOffset;                  // 写入IB的偏置
            }
            #elif (Shunt_Resistor_Mode == Three_Resistor)         // 三电阻校正
            {
                /*set ibus current sample offset*/
                SetReg(FOC_CR2, CSOC0 | CSOC1, CSOC0);
                FOC_CSO = mcCurOffset.IuOffset;                   // 写入IA的偏置
            
                SetReg(FOC_CR2, CSOC0 | CSOC1, CSOC1);
                FOC_CSO = mcCurOffset.IvOffset;                   // 写入IB的偏置
            
                SetReg(FOC_CR2, CSOC0 | CSOC1, 0x00);
                FOC_CSO = mcCurOffset.Iw_busOffset;               // 写入IC的偏置
            }
            #endif  //end Shunt_Resistor_Mode
        }
    }
    #endif  //end CalibENDIS
    /*-------------------------------------------------------------------------------------------------
    DRV_CTL：PWM来源选择
    OCS = 0, DRV_COMR
    OCS = 1, FOC/SVPWM/SPWM
    -------------------------------------------------------------------------------------------------*/
    /*计数器比较值来源FOC*/
    SetBit(DRV_CR, OCS);
    SetBit(DRV_CR ,DRVEN);	
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Motor_Align
    Description    : 预定位函数，当无逆风判断时，采用预定位固定初始位置;当有逆风判断时，采用预定位刹车
    Date           : 2021-12-27
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Motor_Align(void)
{
    if (McStaSet.SetFlag.AlignSetFlag == 0)
    {
        McStaSet.SetFlag.AlignSetFlag = 1;
        DRV_CMR = 0x00;                      
        MOE = 0;
        /* -----FOC初始化----- */
        FOC_Init();
        /*配置预定位的电流、KP、KI*/
        FOC_IDREF = ID_Align_CURRENT;
        FOC_IQREF = IQ_Align_CURRENT;
        FOC_DQKP  = DQKP_Alignment;
        FOC_DQKI  = DQKI_Alignment;
        FOC_EKP   = OBSW_KP_GAIN;
        FOC_EKI   = OBSW_KI_GAIN;
        /*配置预定位角度*/
        #if (AlignTestMode == 1)
        FOC__THETA      = Align_Theta;
        #else
        FOC__THETA      = 0;
        #endif
        /*********PLL或SMO**********/
        #if (EstimateAlgorithm == SMO)
        FOC__ETHETA   = FOC__THETA - 4096;
        #elif (EstimateAlgorithm == PLL)
        FOC__ETHETA   = FOC__THETA;
        #endif //end    EstimateAlgorithm
        /*使能输出*/
        DRV_CMR |= 0x3F;                         // U、V、W相输出
        MOE = 1;
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Motor_NoHall_TailWind
    Description    : 无感顺逆风处理
    Date           : 2021-12-30
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Motor_TailWind(void)
{
    if (mcFocCtrl.State_Count == 0)
    {
        if (McStaSet.SetFlag.TailWindSetFlag == 0)
        {
            McStaSet.SetFlag.TailWindSetFlag = 1;
            /* -----BEMF顺逆风检测初始化配置----- */
            BEMFDetectInit();
        }
        else
            /* -----BEMF顺逆风处理----- */
        { BEMFDealwith(); }
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : Motor_No_Hall_Start
    Description    : 无感启动相关参数配置
    Date           : 2021-12-07
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Motor_Open(void)
{  
    if (McStaSet.SetFlag.StartSetFlag == 0)
    {
        MOE = 0;
        DRV_CMR = 0x00;
        DRV_CMR |= 0x3F;                                         // U、V、W相输出
        MOE = 1;
        McStaSet.SetFlag.StartSetFlag = 1;
        FOC_Init();
        ClrBit(FOC_CR2, UDD);
        ClrBit(FOC_CR2, UQD);
        FOC__THETA  = Align_Theta;                    //启动角度
        FOC_IDREF = ID_Start_CURRENT;                            // D轴启动电流
        mcFocCtrl.mcIqref = IQ_Start_CURRENT >> 1;                    // Q轴启动电流
        FOC_DQKP = DQKPStart;
        FOC_DQKI = DQKIStart;
        FOC_EKP  = OBSW_KP_GAIN;
        FOC_EKI  = OBSW_KI_GAIN;
        #if (Open_Start_Mode == Omega_Start)                     // 启动方式选择:Omega 启动
        {
            FOC_EFREQACC  = Motor_Omega_Ramp_ACC;
            FOC_EFREQMIN  = Motor_Omega_Ramp_Min;
            FOC_EFREQHOLD = Motor_Omega_Ramp_End;
            SetReg(FOC_CR1, EFAE | RFAE | ANGM, EFAE | ANGM);
        }
        #elif (Open_Start_Mode == Open_Start)
        {
            FOC_RTHEACC      = Motor_Open_Ramp_ACC;              // 爬坡函数的初始加速度
            FOC__RTHESTEP    = Motor_Open_Ramp_Min;              // 0.62 degree acce speed
            FOC_RTHECNT      = MOTOR_OPEN_ACC_CNT;               // acce time
            SetReg(FOC_CR1, EFAE | RFAE | ANGM, RFAE);
        }
        #elif (Open_Start_Mode == Open_Omega_Start)
        {
            FOC_RTHEACC   = Motor_Open_Ramp_ACC;                 // 爬坡函数的初始加速度
            FOC__RTHESTEP = Motor_Open_Ramp_Min;                 // 0.62 degree acce speed
            FOC_RTHECNT  = MOTOR_OPEN_ACC_CNT;                   // acce time
            FOC_EFREQACC  = Motor_Omega_Ramp_ACC;
            FOC_EFREQMIN  = Motor_Omega_Ramp_Min;
            FOC_EFREQHOLD = Motor_Omega_Ramp_End;
            SetReg(FOC_CR1, EFAE | RFAE | ANGM, EFAE | RFAE | ANGM);
        }
        #endif
    }
    
    /*不同启动方式下，切换到MCRUN状态*/
    #if (Open_Start_Mode == Open_Start)                           //OPEN状态启动时拖动多次
    {
        if (OpenRampCycles < (MOTOR_OPEN_ACC_CYCLE - 1))
        {
            if (!ReadBit(FOC_CR1, RFAE))
            {
                SetBit(FOC_CR1, RFAE);
                OpenRampCycles++;
                mcFocCtrl.FGDelayOutputTimer ++;
            }
        }
        else
        {
            mcFocCtrl.State_Count = 2;
            mcState = mcRun;
            SetBit(FOC_CR1, ANGM);
        }
        
        FOC_EKP = OBSW_KP_GAIN_RUN4;                              // 估算器里的PI的KP
        FOC_EKI = OBSW_KI_GAIN_RUN4;                              // 估算器里的PI的KI
    }
    #elif (Open_Start_Mode == Open_Omega_Start)
    {
        mcFocCtrl.State_Count = 2600;
        mcState = mcRun;
    }
    #elif (Open_Start_Mode == Omega_Start)
    {
        /*********PLL或SMO**********/
        #if (EstimateAlgorithm == SMO)
        {
            mcFocCtrl.State_Count = 1200;
        }
        #elif (EstimateAlgorithm == PLL)
        {
            mcFocCtrl.State_Count = 1680;
            FOC_EKP = OBSW_KP_GAIN_RUN4;                          // 估算器里的PI的KP
            FOC_EKI = OBSW_KI_GAIN_RUN4;                          // 估算器里的PI的KI
        }
        #endif
        mcState   = mcRun;
    }
    #endif
    
    #if (Open_Start_Mode == PLL_Start)
    {
        PLLFunctionInit();         
    }
    #endif 
    
    FOC_IQREF = mcFocCtrl.mcIqref;                                // Q轴启动电流
    mcState = mcRun;
}

void Motor_Charge(void)
{
    if (McStaSet.SetFlag.ChargeSetFlag == 0)
    {
        McStaSet.SetFlag.ChargeSetFlag = 1;
        DRV_DR = 0.15 * DRV_ARR;              //下桥臂10% duty
        /*-------------------------------------------------------------------------------------------------
        DRV_CTL：PWM来源选择
        OCS = 0, DRV_COMR
        OCS = 1, FOC/SVPWM/SPWM
        -------------------------------------------------------------------------------------------------*/
        ClrBit(DRV_CR, OCS);
        DRV_CMR &= 0xFFc0;
    }
    
    if ((mcFocCtrl.State_Count < 100) && (Step_count == 0))
    {
        Step_count = 1;
        DRV_CMR |= 0x03;                         // U相输出
        MOE = 1;
    }
    
    if (( mcFocCtrl.State_Count <= (100 << 1) / 3) && (Step_count == 1))
    {
        Step_count = 2;
        DRV_CMR |= 0x0F;                         // U、V相输出

    }
    
    if ((mcFocCtrl.State_Count <= 100 / 3) && (Step_count == 2))
    {
        Step_count = 3;
        DRV_CMR |= 0x3F;                         // U、V、W相输出

    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : MotorcontrolInit
    Description    : 控制变量初始化清零,包括保护参数的初始化、电机状态初始化
    Date           : 2021-12-27
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void MotorcontrolInit(void)
{
    memset(&mcFaultDect, 0, sizeof(FaultVarible));                              // FaultVarible保护变量清零
    memset(&mcProtectTime, 0, sizeof(ProtectVarible));                          // ProtectVarible保护次数清零
    memset(&mcCurVarible, 0, sizeof(CurrentVarible));                           // 电流保护的变量清零
    memset(&Vari, 0, sizeof(Variable));
    #if (FRDetectMethod == BEMFMethod)
    {
        memset(&BEMFDetect, 0, sizeof(BEMFDetect_TypeDef));                     //  BEMFDetect所有变量清零
    }
    #endif
    memset(&ONOFFTest, 0, sizeof(ONVarible));                                   // 启停测试参数清零
    memset(&mcFocCtrl, 0, sizeof(FOCCTRL));                                     // mcFocCtrl变量清零
    memset(&AdcSampleValue, 0, sizeof(ADCSample));                              // ADCSample变量清零
    memset(&mcCurOffset, 0, sizeof(CurrentOffset));                             // mcCurOffset变量清零
    mcCurOffset.IuOffsetSum            = 16383;
    mcCurOffset.IvOffsetSum            = 16383;
    mcCurOffset.Iw_busOffsetSum        = 16383;
    memset(&mcSpeedRamp, 0, sizeof(MCRAMP));                                    // mcSpeedRamp变量清零
    memset(&mcPwmInput, 0, sizeof(PWMINPUTCAL));                                // mcPwmInput变量清零
    
    mcSpeedRamp.DelayPeriod = 1;
    
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : VariablesPreInit
    Description    : 初始化电机相关变量
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void VariablesPreInit(void)
{
    memset(&mcFaultDect, 0, sizeof(FaultVarible));                                                                 // FaultVarible变量清零
    memset(&McStaSet, 0, sizeof(MotStaM));
    memset(&mcFocCtrl, 0, sizeof(FOCCTRL)-4);                                                                        // mcFocCtrl变量清零
    memset(&mcPwmInput, 0, sizeof(PWMINPUTCAL));                                                                   // mcPwmInput变量清零
    #if (FRDetectMethod == BEMFMethod)
    {
        memset(&BEMFDetect, 0, sizeof(BEMFDetect_TypeDef));                                                        //  BEMFDetect所有变量清零
    }
    #endif
    mcFRState.TargetFR = SetDirection;
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : GetCurrentOffset
    Description    : 上电时，先对硬件电路的电流进行采集，写入对应的校准寄存器中。
                     调试时，需观察mcCurOffset结构体中对应变量是否在范围内。采集结束后，OffsetFlag置1。
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void GetCurrentOffset(void)
{
    if (!mcCurOffset.OffsetFlag)
    {
        SetBit(ADC_CR, ADCBSY);                                        //使能ADC
        
        while (ReadBit(ADC_CR, ADCBSY));                               //等待AD采样结束
        
        #if (Shunt_Resistor_Mode == Single_Resistor)                    //单电阻模式
        {
            mcCurOffset.Iw_busOffsetSum += ((ADC4_DR & 0x7ff8));
            mcCurOffset.Iw_busOffset     = mcCurOffset.Iw_busOffsetSum >> 4;
            mcCurOffset.Iw_busOffsetSum -= mcCurOffset.Iw_busOffset;
        }
        #elif (Shunt_Resistor_Mode == Double_Resistor)                  //双电阻模式
        {
            mcCurOffset.IuOffsetSum     += ((ADC0_DR & 0x7ff8));
            mcCurOffset.IuOffset         = mcCurOffset.IuOffsetSum >> 4;
            mcCurOffset.IuOffsetSum     -= mcCurOffset.IuOffset;
            mcCurOffset.IvOffsetSum     += ((ADC1_DR & 0x7ff8));
            mcCurOffset.IvOffset         = mcCurOffset.IvOffsetSum >> 4;
            mcCurOffset.IvOffsetSum     -= mcCurOffset.IvOffset;
        }
        #elif (Shunt_Resistor_Mode == Three_Resistor)                   //三电阻模式
        {
            mcCurOffset.IuOffsetSum     += ((ADC0_DR & 0x7ff8));
            mcCurOffset.IuOffset         = mcCurOffset.IuOffsetSum >> 4;
            mcCurOffset.IuOffsetSum     -= mcCurOffset.IuOffset;
            mcCurOffset.IvOffsetSum     += ((ADC1_DR & 0x7ff8));
            mcCurOffset.IvOffset         = mcCurOffset.IvOffsetSum >> 4;
            mcCurOffset.IvOffsetSum     -= mcCurOffset.IvOffset;
            mcCurOffset.Iw_busOffsetSum += ((ADC4_DR & 0x7ff8));
            mcCurOffset.Iw_busOffset     = mcCurOffset.Iw_busOffsetSum >> 4;
            mcCurOffset.Iw_busOffsetSum -= mcCurOffset.Iw_busOffset;
        }
        #endif
        mcCurOffset.OffsetCount++;                                      //校正次数
        
        if (mcCurOffset.OffsetCount > Calib_Time)
        {
            mcCurOffset.OffsetFlag = 1;
            mcCurOffset.OffsetCount = 0;
            
            #if (GetCurrentOffsetEnable)
            {
              Fault_GetCurrentOffset();     //偏置电压保护
            }
            #endif
        }
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : Motor_Ready
    Description    : 准备阶段关输出
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Motor_Ready(void)
{
    if (McStaSet.SetFlag.CalibFlag == 0)
    {
        McStaSet.SetFlag.CalibFlag = 1;
        ClrBit(DRV_CR, FOCEN);
        MOE = 0;
        SetBit(ADC_MASK, CH4EN | CH2EN | CH1EN | CH0EN);
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : Motor_Init
    Description    : 对电机相关变量、PI进行初始化设置
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Motor_Init(void)
{
    ClrBit(ADC_MASK, CH4EN | CH1EN | CH0EN);                           // 关闭软件电流采样的ADC
    VariablesPreInit();                                                // 电机相关变量初始化 
    
}