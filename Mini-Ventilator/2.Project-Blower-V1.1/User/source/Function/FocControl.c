/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : FocControl.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-04
    Description    : This file contains FocControl function used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

#include <FU68xx_4.h>
#include <Myproject.h>
/**
 * @brief     电机控制状态机
 * @warning   电机的状态只能在电机状态控制中切换，禁止在其他地方切换电机状态
 */
void MC_Control(void)
{
    switch (mcState)
    {
        case mcReady:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else
            {
                /* -----Motor_Ready----- */
                Motor_Ready();
                
                if ((mcCurOffset.OffsetFlag == 1) && (mcSpeedRamp.FlagONOFF == 1) && (mcFocCtrl.mcDcbusFlt > UNDER_RECOVER_VALUE))     //等待电路校准结束，并且收到启动信号跳转Init状态
                {
                    mcState = mcInit;
                    mcCurOffset.OffsetFlag = 0;
                    mcCurOffset.OffsetCount = 0;
                }
            }
            
            break;
            
        case mcInit:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else if (mcSpeedRamp.FlagONOFF == 0)
            {
                mcState = mcStop;
            }
            else
            {
                if (mcCurOffset.OffsetFlag == 1)
                {
                    /* -----Motor_Init----- */
                    Motor_Init();
                    #if (IPMState == IPMtest)
                    {
                        mcState               = mcCharge;
                        mcFocCtrl.State_Count = 100;
                    }
                    #else
                    {
                        mcState              = mcTailWind;
                    }
                    #endif
                }
            }
                    
            break;
                    
        case mcCharge:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else if (mcSpeedRamp.FlagONOFF == 0)
            {
                mcState = mcStop;
            }
            else
            {
                Motor_Charge();
            }
                    
            break;
                    
        case mcTailWind:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else if (mcSpeedRamp.FlagONOFF == 0)
            {
                mcState = mcStop;
            }
            else
            {
                #if (TailWind_Mode == NoTailWind)
                {
                    mcState = mcAlign;
                    
                    mcFocCtrl.State_Count = Align_Time;
                }
                #else
                {
                    /* -----无感顺逆风----- */
                    Motor_TailWind();
                    if (mcFocCtrl.TailWindStatus == Staticwind)
                    {
                    	mcState = mcAlign;
                    	mcFocCtrl.State_Count = Align_Time;
                    }
                    else if (mcFocCtrl.TailWindStatus == Downwind)
                    {
                    	mcState = mcRun;
                    }
                }
                #endif
            }
                    
            break;
                    
        case mcAlign:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else if (mcSpeedRamp.FlagONOFF == 0)
            {
                mcState = mcStop;
            }
            else
            {
                /* -----预定位----- */
                Motor_Align();
                #if (AlignTestMode == 1)
                {
                    _nop_();
                }
                #else
                {
                    if (mcFocCtrl.State_Count == 0)
                    {
                        mcState = mcStart;                                                    //定位结束后启动
                    }
                }
                #endif
            }
                    
            break;
                    
        case mcStart:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else if (mcSpeedRamp.FlagONOFF == 0)
            {
                mcState = mcStop;
            }
            else
            {
                /* -----无感启动----- */
                Motor_Open();
            }
                    
            break;
                    
        case mcRun:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else
            {
                if ((mcSpeedRamp.TargetValue == 0) || (mcSpeedRamp.FlagONOFF == 0))          //延迟关断输出
                {
                     mcState = mcStop;
                }
            }
                    
            break;
                    
        case mcStop:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else
            {
                #if (StopBrakeFlag == 0)
                {
                    mcState = mcReady;
                    FOC_CR1 = 0x00;
                    ClrBit(DRV_CR, FOCEN);
                    ClrBit(DRV_CR, DRVEN);
                    MOE = 0;
                }
                #else
                {
                    if (mcFocCtrl.SpeedFlt < Motor_Stop_Speed)
                    {
                        MOE = 0;
                        FOC_CR1 = 0x00;
                        ClrBit(DRV_CR, FOCEN);
                        DRV_DR   = DRV_ARR + 1;
                        DRV_CMR &= 0xFFC0;;
                        DRV_CMR |= 0x015;
                        ClrBit(DRV_CR, OCS);
                        MOE = 1;
                        mcState  = mcBrake;
                        mcFocCtrl.State_Count = StopWaitTime;
                    }
                }
                #endif
            }
                    
            break;
                    
        case mcBrake:
            if (mcFaultSource != FaultNoSource)
            {
                mcState = mcFault;
            }
            else
            {
                if (mcFocCtrl.State_Count == 0)
                {
                    mcState = mcReady;
                    MOE = 0;
                    ClrBit(DRV_CR, FOCEN);
                }
            }
                    
            break;
                    
        case mcFault:
            FaultProcess();
                    
            if (mcFaultSource == FaultNoSource)
            {
                mcState = mcReady;
            }
                    
            break;
                    
        default:
            break;
    }
}                    