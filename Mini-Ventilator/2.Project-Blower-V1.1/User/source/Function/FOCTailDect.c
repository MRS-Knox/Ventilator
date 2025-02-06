/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : FOCTailDect.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-27
    Description    : This file contains FOCTailDect function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include "Myproject.h"
/* -------------------------------------------------------------------------------------------------
    Function Name  : TailWindDetectInit
    Description    : 顺逆风参数初始化
    Date           : 2021-12-27
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void TailWindDetectInit(void)
{
    TailWindDetect.MotorTailWindState     = NormalTailWind;     //  初始状态为正常的顺逆风状态
    TailWindDetect.TempThailWindSpeedBase = ThailWindSpeedBase; //  顺逆风计算的speedbase
    FOC_Init();                                                 // FOC的初始化
    FOC_DQKP    = DQKP_TailWind;                                // 顺逆风的电流环KP
    FOC_DQKI    = DQKI_TailWind;                                // 顺逆风的电流环KI
    FOC_EKP     = OBSW_KP_GAIN_WIND;                            // 顺逆风速度估算的KP
    FOC_EKI     = OBSW_KI_GAIN_WIND;                            // 顺逆风速度估算的KI
    FOC_OMEKLPF = SPEED_KLPF_WIND;                              // 顺逆风下的速度滤波系数                       
    SetBit(FOC_CR1, ANGM); 
    FOC_IDREF = 0;                                              // D轴给定电流
    FOC_IQREF = 0;    
    DRV_CMR |= 0x3F;                                            // U、V、W相输出
    MOE = 1;                                                    // 打开MOE
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : TailWindSpeedDetect
    Description    : 顺逆风速度检测，根据角度的变化趋势来判断方向
    Date           : 2021-12-09
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void TailWindSpeedDetect(void)
{
    static int16 LatestTheta=0;
    if(mcState == mcTailWind)
    {
        //顺风检测过程由于FOC内部根据电流估算角度，故直接对FOC_ETHETA进行处理
        if(TailWindDetect.MotorTailWindState == NormalTailWind)
        {
            LatestTheta = FOC__ETHETA;

            //旋转方向判断在 <-170度   <10 >-10  >170度三个状态切换的时间
            if(LatestTheta < -30946)
            {
                //计数器未清零或从状态3跳到状态1时清零
                if((TailWindDetect.SpeedTimerClearStatus == 0) || (TailWindDetect.AngleState == 3))
                {
                    TailWindDetect.SpeedCountTimer       = 0;
                    TailWindDetect.SpeedTimerClearStatus = 1;
                    if(TailWindDetect.AngleState == 3) TailWindDetect.ShakeTimes ++;//来回1和3之间抖动，抖动次数加1
                }
                //<-170度  时设置状态为1，并清零SpeedCountTimer在TIM5中计时
                TailWindDetect.AngleState = 1;
            }
            else if((LatestTheta>-1820)&&(LatestTheta<1820)) //<10 >-10
            {
                //状态1或状态3切换到状态2时保存当前转速时间至TailWindDetect.SpeedCount[SpeedStoreNum]
                if((TailWindDetect.AngleState == 1) || (TailWindDetect.AngleState == 3))
                {
                    //计算当前转速，RPM
                    TailWindDetect.SpeedCountTimer += 1;//防止为0
                    DivQ_L_MDU(TailWindDetect.TempThailWindSpeedBase >> 16, TailWindDetect.TempThailWindSpeedBase, TailWindDetect.SpeedCountTimer, TailWindDetect.TailWindSpeed);
                    TailWindDetect.SpeedStoreNum ++;

                    //用于SpeedCountTimer清零
                    if(TailWindDetect.SpeedTimerClearStatus == 1) TailWindDetect.SpeedTimerClearStatus = 0;

                    //有1状态切换到2状态说明角度递增旋转方向为CW，3->2则为CCW
                    if(TailWindDetect.AngleState == 1)
                    {                        
                        TailWindDetect.MotorDir = CW;
                    }
                    else if(TailWindDetect.AngleState == 3) 
                    {
                        TailWindDetect.MotorDir = CCW;
                    }
                    TailWindDetect.ShakeTimes = 0;//清除来回抖动的计数
                }
                TailWindDetect.AngleState = 2;

            }
            //>170度时
            else if(LatestTheta > 30946)
            {
                //计数器未清零或从状态1跳到状态3时清零
                if((TailWindDetect.SpeedTimerClearStatus == 0) || (TailWindDetect.AngleState == 1))
                {
                    TailWindDetect.SpeedCountTimer       = 0;
                    TailWindDetect.SpeedTimerClearStatus = 1;
                    if(TailWindDetect.AngleState == 1) TailWindDetect.ShakeTimes ++;//来回1和3之间抖动
                }
                TailWindDetect.AngleState = 3;
            }
        }
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : FOCCloseLoopStart
    Description    : 无感顺风启动
    Date           : 2021-12-29
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void FOCCloseLoopStart(void)
{
    MOE = 0;
    FOC_Init();
	ClrBit(FOC_CR2,UQD); 
    ClrBit(FOC_CR2,UDD); 
    FOC_EFREQACC  = Motor_Omega_Ramp_ACC;
    FOC_EFREQMIN  = Motor_Omega_Ramp_Min;
    FOC_EFREQHOLD = Motor_Omega_Ramp_End;    
	ClrBit(FOC_CR2,UDD);
    ClrBit(FOC_CR2, ESEL);                                           //切换到SMO模式	
    SetBit(FOC_CR0, ESCMS);
    FOC_EKLPFMIN  = OBS_EA_KS;
    FOC_KSLIDE = OBS_KSLIDE;
	
    FOC_IDREF = ID_Start_CURRENT;                                    // D轴启动电流 
    
    FOC__THECOMP = _Q15(0.0 / 180.0);                                // SMO 估算补偿角
    FOC__THECOR  = 0x00;                                             // 误差角度补偿

    FOC_DQKP = DQKP_TailWind;
    FOC_DQKI = DQKI_TailWind; 
	  
    SetBit(FOC_CR1, EFAE);                                           // 估算器强制输出
    ClrBit(FOC_CR1, RFAE);                                           // 禁止强拉
    SetBit(FOC_CR1, ANGM);                                           // 估算模式
    
    if(TailWindDetect.TailWindSpeed<_Q15(2000/MOTOR_SPEED_BASE))
    {
        FOC_EKP = OBSW_KP_GAIN_RUN1;                                 // 估算器里的PI的KP
        FOC_EKI = OBSW_KI_GAIN_RUN1;                                 // 估算器里的PI的KI  
        mcFocCtrl.mcIqref =I_Value(1.5);
        mcFocCtrl.State_Count=700;
    }
    else if(TailWindDetect.TailWindSpeed<_Q15(3000/MOTOR_SPEED_BASE))
    {
        FOC_EKP = OBSW_KP_GAIN_RUN2;                                 // 估算器里的PI的KP
        FOC_EKI = OBSW_KI_GAIN_RUN2;                                 // 估算器里的PI的KI
        mcFocCtrl.mcIqref =I_Value(1.8);
        mcFocCtrl.State_Count=500;
    }
    else
    {
        FOC_EKP = OBSW_KP_GAIN_RUN3;                                 // 估算器里的PI的KP
        FOC_EKI = OBSW_KI_GAIN_RUN3;                                 // 估算器里的PI的KI
        mcFocCtrl.mcIqref =I_Value(2.0);
        mcFocCtrl.State_Count=300;
        
    }   
    FOC_OMEKLPF  = SPEED_KLPF;    
    FOC_IQREF =mcFocCtrl.mcIqref; 
    
    mcState  = mcRun;
    	
    McStaSet.SetFlag.StartSetFlag = 0;
    
    DRV_CMR  = 0x00;
    DRV_CMR |= 0x3F;
	MOE = 1;   
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : FOCTailWindDealwith
    Description    : 顺风逆风参数配置函数
    Date           : 2021-12-27
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void FOCTailWindDealwith(void)
{  
    #if (EstimateAlgorithm == SMO)
    {
        SMO_TailWindDealwith();
    }
    #elif (EstimateAlgorithm == PLL)
    {
        PLL_TailWindDealwith();
    }
    #endif 
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : SMO_TailWindDealwith
    Description    : 滑膜估算器情况下顺逆风处理
    Date           : 2021-12-27
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void SMO_TailWindDealwith(void)
{
    if(((TailWindDetect.MotorDir == CW) && (TailWindDetect.SpeedOverFlowStatus == 1)) || (TailWindDetect.ShakeTimes > 2))
    { 
        mcState = mcAlign;        
        mcFocCtrl.State_Count = Align_Time;      
        McStaSet.SetFlag.TailWindSetFlag = 0;
    }
    else if((TailWindDetect.MotorDir == CCW) && (TailWindDetect.SpeedStoreNum > 2))
    {
        if(TailWindDetect.TailWindSpeed < 500)
        {
            mcFocCtrl.mcIqref       = IQ_Start_CURRENT + I_Value(0.2);       // Q轴启动电流
            TailWindDetect.AntiFlag = 1;
        }
        else if(TailWindDetect.TailWindSpeed < 1000)
        {
            mcFocCtrl.mcIqref       = IQ_Start_CURRENT + I_Value(0.5);       // Q轴启动电流
            TailWindDetect.AntiFlag = 2;
        }
        else if(TailWindDetect.TailWindSpeed < 1500)
        {
            mcFocCtrl.mcIqref       = IQ_Start_CURRENT + I_Value(0.8);       // Q轴启动电流
            TailWindDetect.AntiFlag = 3;
        }
        else
        {
            mcFocCtrl.mcIqref       = IQ_Start_CURRENT + I_Value(1.0);       // Q轴启动电流
            TailWindDetect.AntiFlag = 4;
        }

        FOC_DQKP = DQKP;
        FOC_DQKI = DQKI;
        
        SetReg(FOC_CR1, EFAE | RFAE | ANGM, EFAE | ANGM);
        
        FOC_DMAX = DOUTMAX;
        FOC_DMIN = DOUTMIN;
        FOC_QMAX = QOUTMAX;
        FOC_QMIN = QOUTMIN;
        FOC_EK2  = OBS_K2T_SMO;

        FOC_IQREF  = mcFocCtrl.mcIqref;
        mcFocCtrl.State_Count = 1300;
        FOC_EKP               = OBSW_KP_GAIN;
        FOC_EKI               = OBSW_KI_GAIN;
        FOC_IDREF          = 0;
        mcState            = mcRun;
        mcFocCtrl.CtrlMode = 0;

    }
    else if(TailWindDetect.MotorDir == CW)
    {
        //超过检测次数TailWindDetect.SpeedStoreNum切转速TailWindStartMinSpeed以上时直接顺风启动
        if((TailWindDetect.TailWindSpeed > TailWindStartMinSpeed) && (TailWindDetect.SpeedStoreNum >= 2))
        {
            FOCCloseLoopStart();	
        }
    }

}
/* -------------------------------------------------------------------------------------------------
    Function Name  : FOCTailWindTimeLimit
    Description    : FOC顺逆风检测的时间处理
    Date           : 2021-12-29
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void FOCTailWindTimeLimit(void)
{
    if (mcState == mcTailWind)
    {
        if (TailWindDetect.SpeedCountTimer < (TailWind_Time-1)) // SpeedCountTimer-计算时间间隔
        {
            TailWindDetect.SpeedCountTimer++;
        }
        else
        {
            TailWindDetect.TailWindSpeed       = 0;             //速度为0
            TailWindDetect.SpeedCountTimer     = 0;
            TailWindDetect.SpeedOverFlowStatus = 1;             // 转速计数溢出标志
        }
    }
}
