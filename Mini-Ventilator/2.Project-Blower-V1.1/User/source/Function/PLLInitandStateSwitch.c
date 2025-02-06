/* --------------------------- (C) COPYRIGHT 2022 Fortiortech ShenZhen -----------------------------
    File Name      : PLLInitandStateSwitch.c
    Author         : Fortiortech  Appliction Team
    Version        : V1.0
    Date           : 2022-7-23
    Description    : This file contains PLLInitandStateSwitch.c function used for Motor Startup.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include "FU68xx_4.h"
#include <Myproject.h>
/*  -------------------------------------------------------------------------------------------------
    Function Name  : PLLFunctionInit
    Description    : PLL参数初始化
    Date           : 2022-07-23
    Parameter      : None
    ------------------------------------------------------------------------------------------------- */
void PLLFunctionInit(void)
{
    memset(&PLLfunction, 0, sizeof(PLLVar));
    PI_Init_PLL();
    PLLfunction.Theta_pre = 0;
    FOC__THETA = 0;
    FOC_EFREQACC  = 0;
    FOC_EFREQMIN  = 0;
    FOC_EFREQHOLD = 0;
    ClrBit(FOC_CR1, EFAE);
    ClrBit(FOC_CR1, RFAE);
    ClrBit(FOC_CR1, ANGM);
    PLLfunction.BEMFBase = V_factor1;
    PLLfunction.Vfactor = V_factor;
    PLLfunction.PLLFunctionFlag = 1; 
}

/*  -------------------------------------------------------------------------------------------------
    Function Name  : StartupState
    Description    : 启动状态机控制
    Date           : 2022-07-23
    Parameter      : None
    ------------------------------------------------------------------------------------------------- */

void PLLStateFunction(void)
{
    switch (PLLfunction.PLLFunctionFlag)
    {
        case 1:
            PLLfunction.test = PLLfunction.Theta_pre + 80;
            if (PLLfunction.count < 30000)
            {
                PLLfunction.count++;                
            }
            
            PLLSoftwareFunction();

            if ((PLLfunction.count > 1500) && (PLLfunction.Speed_PLL1 > CLOSE_SPEED))
            {
                PLLfunction.Switch_Count++;
                
                if  (PLLfunction.Switch_Count > 500)
                {
                    PLLfunction.count = 0;
                    PLLfunction.PLLFunctionFlag = 2;
                    PLLfunction.Theta_err = FOC__THETA - FOC__ETHETA;
                    FOC_DQKP = DQKP_observer;
                    FOC_DQKI = DQKI_observer;
                    ClrBit(DRV_SR , DCIP);
                    SetBit(FOC_CR1, ANGM);
                }           
            }
            else
            {
                if (PLLfunction.Switch_Count > 0)
                {
                    PLLfunction.Switch_Count--;
                }
            }
            break;
        case 2:
            if (PLLfunction.count < 256)
            {
                PLLfunction.count++;
                FOC__THETA = FOC__ETHETA + ((PLLfunction.Theta_err >> 8)*(256  - PLLfunction.count));              
            }
            else
            {
                PLLfunction.PLLFunctionFlag = 3;
            }            
            break;
        default:
            break;
    }   
}

/*  -------------------------------------------------------------------------------------------------
    Function Name  : RampFucntion
    Description    : PLL启动时参数爬坡函数
    Date           : 2022-07-23
    Parameter      : None
    ------------------------------------------------------------------------------------------------- */

void RampFucntion(void)
{    
    if (PLLfunction.BEMFBase_Update_Count > 0)
    {
        if (PLLfunction.BEMFBase < V_factor1_Max)
        {
            PLLfunction.BEMFBase = PLLfunction.BEMFBase + 50;
        }
        PLLfunction.BEMFBase_Update_Count = 0;
    }
    
    PLLfunction.BEMFBase_Update_Count++;
}