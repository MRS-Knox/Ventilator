/* --------------------------- (C) COPYRIGHT 2022 Fortiortech ShenZhen -----------------------------
    File Name      : MotorProtect.c
    Author         : Fortiortech  Appliction Team
    Version        : V1.0
    Date           : 2022-12-09
    Description    : This file contains XX-XX-XX function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

#include <FU68xx_4.h>
#include <Myproject.h>

FaultVarible         xdata   mcFaultDect;
CurrentVarible       xdata   mcCurVarible;
ProtectVarible       xdata   mcProtectTime;

/* -------------------------------------------------------------------------------------------------
    Function Name  : Fault_OverUnderVoltage
    Description    : 欠过压保护
    Date           : 2021-12-28
    Parameter      : h_Fault: [输入/出]
------------------------------------------------------------------------------------------------- */
void Fault_OverUnderVoltage(void)
{
    if (mcFaultSource == FaultNoSource)
    {
        //过压保护
        if (mcFocCtrl.mcDcbusFlt > OVER_PROTECT_VALUE)
        {
            mcFaultDect.OverVoltDetecCnt++;
            
            if (mcFaultDect.OverVoltDetecCnt > 30)
            {
                mcFaultDect.OverVoltDetecCnt = 0;
                mcFaultSource             = FaultOverVoltage;
            }
        }
        else if (mcFaultDect.OverVoltDetecCnt > 0)
        {
            mcFaultDect.OverVoltDetecCnt--;
        }
        
        //欠压保护
        if (mcFocCtrl.mcDcbusFlt < UNDER_PROTECT_VALUE)
        {
            mcFaultDect.UnderVoltDetecCnt++;
            
            if (mcFaultDect.UnderVoltDetecCnt > 30)
            {
                mcFaultDect.UnderVoltDetecCnt = 0;
                mcFaultSource              = FaultUnderVoltage;
            }
        }
        else if (mcFaultDect.UnderVoltDetecCnt > 0)
        {
            mcFaultDect.UnderVoltDetecCnt--;
        }
    }   
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Fault_Overcurrent
    Description    : 软件过流保护
    Date           : 2021-12-28
    Parameter      : h_Cur: [输入/出]
------------------------------------------------------------------------------------------------- */
void Fault_Overcurrent(void)
{
    if ((mcState == mcRun) || (mcState == mcStart))
    {
        mcCurVarible.Max_ia = FOC__IAMAX;
        mcCurVarible.Max_ib = FOC__IBMAX;
        mcCurVarible.Max_ic = FOC__ICMAX;
        
        if ((mcCurVarible.Max_ia >= OverSoftCurrentValue) || (mcCurVarible.Max_ib >= OverSoftCurrentValue) || (mcCurVarible.Max_ic >= OverSoftCurrentValue))
        {
            mcCurVarible.OverCurCnt++;
            
            if (mcCurVarible.OverCurCnt >= OverSoftCurrentTimes)
            {
                mcFaultSource     = FaultSoftOVCurrent;
                mcCurVarible.Max_ia     = 0;
                mcCurVarible.Max_ib     = 0;
                mcCurVarible.Max_ic     = 0;
                mcCurVarible.OverCurCnt = 0;
            }
        }
        else if (mcCurVarible.OverCurCnt > 0)
        {
            mcCurVarible.OverCurCnt--;
        }
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Fault_Start
    Description    : 启动保护函数
    Date           : 2021-12-28
    Parameter      : h_Fault: [输入/出]
------------------------------------------------------------------------------------------------- */
void Fault_Start(void)
{
    if (mcState == mcRun)
    {
        if (mcFaultDect.StartSpeedCnt <= 5000)         //5秒内速度大于最大速度，同时反电动势值低于一定值
        {
            mcFaultDect.StartSpeedCnt++;
            
            if ((mcFocCtrl.SpeedFlt > Motor_Stall_Max_Speed) && (mcFocCtrl.EsValue < 500))
            {
                mcFaultDect.StartSpeedCnt = 0;
                mcFaultSource          = FaultStart;
                mcProtectTime.StartFlag = 1;
            }
        }
        
        //方法二
        if (mcFaultDect.StartEsCnt <= 6000)            //前6s，等待2s后，开始判断ES，如果超过一定次数，则失败
        {
            mcFaultDect.StartEsCnt++;
            mcFaultDect.StartDelay++;
            
            if (mcFaultDect.StartDelay >= 2000)         // 2s
            {
                mcFaultDect.StartDelay = 2000;
                
                if ((mcFocCtrl.EsValue < 1000))
                {
                    mcFaultDect.StartESCount++;
                    
                    if (mcFaultDect.StartESCount >= 100)//ms
                    {
                        mcFaultSource = FaultStart;
                        mcFaultDect.StartDelay     = 0;
                        mcFaultDect.StartESCount   = 0;
                        mcProtectTime.StartFlag = 2;
                    }
                }
                else
                {
                    if (mcFaultDect.StartESCount > 0)
                    {
                        mcFaultDect.StartESCount--;
                    }
                }
            }
        }
        else
        {
            mcFaultDect.StartESCount = 0;
        }
        
        //方法三，长时间在CtrlMode=0状态
        if (mcFocCtrl.CtrlMode == 0)                //mcFocCtrl.CtrlMode =0在开环状态，mcFocCtrl.CtrlMode=1在闭环状态
        {
            mcFaultDect.StartFocmode++;
            
            if (mcFaultDect.StartFocmode >= 2000)       //2秒后
            {
                mcFaultDect.StartFocmode = 0;
                mcFaultSource         = FaultStart;
                mcProtectTime.StartFlag = 3;
            }
        }
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Fault_Stall
    Description    : 堵转保护
    Date           : 2021-12-28
    Parameter      : h_Fault: [输入/出]
------------------------------------------------------------------------------------------------- */
void Fault_Stall(void)
{
    if (mcState == mcRun)
    {
        if (mcFaultDect.StallDelayCnt <= 2000) //2s  
        {
            mcFaultDect.StallDelayCnt++;
        }
        else
        {
            if ((mcFocCtrl.EsValue < 1000) || ((FOC__EOME > _Q15(3000.0 / MOTOR_SPEED_BASE)) && (mcFocCtrl.EsValue < 1000)))
            {
                mcFaultDect.StallDectEs++;
                
                if (mcFaultDect.StallDectEs >= 500)//ms
                {
                    mcFaultDect.StallDectEs = 0;
                    mcFaultSource        = FaultStall;
                    mcProtectTime.StallFlag = 1;
                }
            }
            else
            {
                if (mcFaultDect.StallDectEs > 0)
                {
                    mcFaultDect.StallDectEs--;
                }
            }
            
            if (mcFocCtrl.SpeedFlt > (uint32)mcFocCtrl.EsValue * 7)
            {
                mcFaultDect.StallTimes++;
                
                if (mcFaultDect.StallTimes >= 500)//ms
                {
                    mcFaultDect.StallTimes = 0;
                    mcFaultSource        = FaultStall;
                    FaultProcess();
                    mcProtectTime.StallFlag = 2;
                }
            }
            else
            {
                if (mcFaultDect.StallTimes > 0)
                {
                    mcFaultDect.StallTimes--;
                }
            }
            
            //判断速度低于堵转最小值或者超过堵转最大值
            if ((mcFocCtrl.SpeedFlt < Motor_Stall_Min_Speed) || (mcFocCtrl.SpeedFlt > Motor_Stall_Max_Speed))
            {
                mcFaultDect.StallDectSpeed++;
                
                if (mcFaultDect.StallDectSpeed >= 500)//ms
                {
                    mcFaultDect.StallDectSpeed = 0;
                    mcFaultSource           = FaultStall;
                    mcProtectTime.StallFlag = 3;
                }
            }
            else
            {
                if (mcFaultDect.StallDectSpeed > 0)
                {
                    mcFaultDect.StallDectSpeed--;
                }
            }
        }
    }   
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Fault_phaseloss
    Description    : 缺相保护
    Date           : 2021-12-28
    Parameter      : h_Fault: [输入/出]
------------------------------------------------------------------------------------------------- */
void Fault_phaseloss(void)
{
    if (mcState == mcRun)
    {
        mcFaultDect.Lphasecnt++;
        
        if (mcFaultDect.Lphasecnt > 500) 
        {
            mcFaultDect.Lphasecnt = 0;
            
            
            if (((mcCurVarible.Max_ia > (mcCurVarible.Max_ib * PhaseLossTimes)) || (mcCurVarible.Max_ia > (mcCurVarible.Max_ic * PhaseLossTimes)))
                && (mcCurVarible.Max_ia > PhaseLossCurrentValue))
            {
                mcFaultDect.AOpencnt++;
            }
            else
            {
                if (mcFaultDect.AOpencnt > 0)
                {
                    mcFaultDect.AOpencnt --;
                }
            }
            
            if (((mcCurVarible.Max_ib > (mcCurVarible.Max_ia * PhaseLossTimes)) || (mcCurVarible.Max_ib > (mcCurVarible.Max_ic * PhaseLossTimes)))
                && (mcCurVarible.Max_ib > PhaseLossCurrentValue))
            {
                mcFaultDect.BOpencnt++;
            }
            else
            {
                if (mcFaultDect.BOpencnt > 0)
                {
                    mcFaultDect.BOpencnt --;
                }
            }
            
            if (((mcCurVarible.Max_ic > (mcCurVarible.Max_ia * PhaseLossTimes)) || (mcCurVarible.Max_ic > (mcCurVarible.Max_ib * PhaseLossTimes)))
                && (mcCurVarible.Max_ic > PhaseLossCurrentValue))
            {
                mcFaultDect.COpencnt++;
            }
            else
            {
                if (mcFaultDect.COpencnt > 0)
                {
                    mcFaultDect.COpencnt --;
                }
            }  

            if ((mcCurVarible.Max_ic < PhaseLossCurrentValue) && (mcCurVarible.Max_ib < PhaseLossCurrentValue) && (mcCurVarible.Max_ia < PhaseLossCurrentValue))
            {
                mcFaultDect.ABCOpencnt++;
                
                if(mcFaultDect.ABCOpencnt>=2)
                {
                   mcFaultDect.ABCOpencnt = 0;
                   mcFaultSource = FaultLossPhase;                                    
                }

            }
            else
            {
                mcFaultDect.ABCOpencnt = 0;
            }            
            
            mcCurVarible.Max_ia = 0;
            mcCurVarible.Max_ib = 0;
            mcCurVarible.Max_ic = 0;
            
            SetBit(FOC_CR2, ICLR);
            
            if ((mcFaultDect.AOpencnt > 1) || (mcFaultDect.BOpencnt > 1) || (mcFaultDect.COpencnt > 1))
            {           
                mcFaultSource = FaultLossPhase;
            }                                                                                                                                               
            
        }
    }
}

void Fault_Recovery(void)
{
    #if (!StartONOFF_Enable)
    {
        /*******过压欠压保护恢复*********/
        if ((mcState == mcFault) && ((mcFaultSource == FaultUnderVoltage) || (mcFaultSource == FaultOverVoltage)))
        {
            if ((mcFocCtrl.mcDcbusFlt < OVER_RECOVER_VALUE) && (mcFocCtrl.mcDcbusFlt > UNDER_RECOVER_VALUE))
            {
                mcFaultDect.VoltRecoverCnt++;
                
                if (mcFaultDect.VoltRecoverCnt > VOU_Recover_Time)
                {
                    mcFaultSource           = FaultNoSource;
                    mcFaultDect.VoltRecoverCnt = 0;
                }
            }
            else
            {
                mcFaultDect.VoltRecoverCnt = 0;
            }
        }
        /*******启动保护恢复*********/ 
        if ((mcFaultSource == FaultStart) && (mcState == mcFault) && (mcProtectTime.SecondStartTimes < StartProtectRestartTimes))
        {
            mcFaultDect.StartReCount++;
            
            if (mcFaultDect.StartReCount >= StartRecoverTime)
            {
                mcFaultDect.StartReCount = 0;
                mcProtectTime.SecondStartTimes++;
                mcFaultSource         = FaultNoSource;
            }
        }
        else
        {
            mcFaultDect.StartReCount = 0;
        } 

        /*******堵转保护恢复*********/
        if ((mcFaultSource == FaultStall) && (mcState == mcFault) && (mcProtectTime.StallTimes < StallProtectRestartTimes))
        {
            mcFaultDect.StallReCount++;
            
            if (mcFaultDect.StallReCount >= StallRecoverTime)
            {
                mcFaultDect.StallReCount = 0;
                mcProtectTime.StallTimes++;
                mcFaultSource         = FaultNoSource;
            }
        }
        else
        {
            mcFaultDect.StallReCount = 0;
        }
    
        /*******缺相保护恢复*********/
        if ((mcFaultSource == FaultLossPhase) && (mcState == mcFault) && (mcProtectTime.LossPHTimes < PhaseLossRestartTimes)) //可重启5次
        {
            mcFaultDect.mcLossPHRecCount++;
            
            if (mcFaultDect.mcLossPHRecCount >= PhaseLossRecoverTime)
            {
                mcProtectTime.LossPHTimes++;
                mcFaultDect.AOpencnt = 0;
                mcFaultDect.BOpencnt = 0;
                mcFaultDect.COpencnt = 0;
                mcFaultSource     = FaultNoSource;
            }
        }
        else
        {
            mcFaultDect.mcLossPHRecCount = 0;
        }
        
         /*******过流保护恢复*********/ 
        if ((mcState == mcFault) && ((mcFaultSource == FaultSoftOVCurrent) || (mcFaultSource == FaultHardOVCurrent)) && (mcProtectTime.CurrentPretectTimes < OverCurrentRestartTimes))
        {
            mcFaultDect.CurrentRecoverCnt++;
            
            if (mcFaultDect.CurrentRecoverCnt >= OverCurrentRecoverTime)
            {
                mcFaultDect.CurrentRecoverCnt = 0;
                mcProtectTime.CurrentPretectTimes++;
                mcFaultSource = FaultNoSource;
            }
        }

        
    }
    #endif       
}