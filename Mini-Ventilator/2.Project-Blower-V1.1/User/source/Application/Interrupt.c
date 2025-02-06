/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : Interrupt.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-04
    Description    : This file contains interrupt.c function used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <FU68xx_4.h>
#include <Myproject.h>
#include <UART.h>
uint16 xdata spidebug[4] = { 0 };
uint8 g_1mTick=0;
/* -------------------------------------------------------------------------------------------------
    Function Name  : LVW_TSD_INT
    Description    : 低压预警中断（LVW）、温度侦测中断（TSD）
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void LVW_TSD_INT(void) interrupt 0
{
    if (ReadBit(LVSR, LVWIF))
    {
        if (ReadBit(LVSR, LVWF))
        {
            #if 0                                    // 关闭输出
            mcFaultSource = FaultLVW;                // 低压预警保护
            #endif
            ClrBit(LVSR, LVWF);
        }
        
        ClrBit(LVSR, LVWIF);
    }
    
    if (TSDIF)
    {
        TSDIF = 0;
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : FO_INT
    Description    : FO硬件过流中断
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
#if 0
void FO_INT(void)    interrupt 1                // 外部中断0用来做硬件FO过流中断，关闭输出
{
    if (IF0)
    {
        mcFaultSource = FaultHardOVCurrent;     // 硬件过流保护
        IF0 = 0;                                // 清零中断标志
    }
}
#endif

/* -------------------------------------------------------------------------------------------------
    Function Name  : DRV_ISR
    Description    : FOC中断(Drv中断),每个载波周期执行一次，用于处理响应较高的程序，中断优先级第二。DCEN开了就会产生中断。
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void DRV_ISR(void)     interrupt 3
{
    if (ReadBit(DRV_SR, FGIF))
    {
        ClrBit(DRV_SR, FGIF);
    }
    
    if (ReadBit(DRV_SR, DCIF))
    {
  
        #if (Open_Start_Mode == PLL_Start)
        {            
            PLLStateFunction();            
        }
        #endif                       

        DRV_SR = (DRV_SR | SYSTIF) & (~DCIF);
    }   
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : TIM2_INT
    Description    : TIMER2溢出中断
    Date           : 2021-12-05
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void TIM2_INT(void) interrupt 4
{
    if (ReadBit(TIM2_CR1, T2IF))
    {
        ClrBit(TIM2_CR1, T2IF);
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : CMP_ISR
    Description    : CMP0/1/2：顺逆风判断
    Date           : 2021-12-30
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void CMP_ISR(void) interrupt 7
{
    if ((ReadBit(CMP_SR, CMP2IF)) || (ReadBit(CMP_SR, CMP0IF)) || (ReadBit(CMP_SR, CMP1IF)))
    {
        BEMFDetectFunc();
        ClrBit(CMP_SR, CMP2IF);
        ClrBit(CMP_SR, CMP1IF);
        ClrBit(CMP_SR, CMP0IF);
    }
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : TIM3_INT
    Description    : Capture PWM，用于PWM调速
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void TIM3_INT(void) interrupt 9
{    
    if (ReadBit(TIM3_CR1, T3IP))//周期中断
    {
        if (mcPwmInput.PWMUpdateFlag != 1)                        //若正在计算占空比则不更新
        {
            mcPwmInput.PWMCompare    = TIM3__DR;
            mcPwmInput.PWMARR        = TIM3__ARR;
            mcPwmInput.PWMUpdateFlag = 1;
        }
        
        ClrBit(TIM3_CR1, T3IP);
    }
    
    if (ReadBit(TIM3_CR1, T3IF))
    {
        if (mcPwmInput.PWMUpdateFlag != 1)
        {
            if (ReadBit(P0, PIN1))                                 //PWM 100%输出
            {
                mcPwmInput.PWMCompare = 3000;
                mcPwmInput.PWMARR     = 3000;
            }
            else                                                    //PWM 0%输出
            {
                mcPwmInput.PWMCompare = 0;
                mcPwmInput.PWMARR     = 28000;
            }
            
            mcPwmInput.PWMUpdateFlag = 1;
        }
        
        ClrBit(TIM3_CR1, T3IF);
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : SYStick_INT
    Description    : 1ms定时器中断（SYS TICK中断），用于处理附加功能，如控制环路响应、各种保护等。中断优先级低于FO中断和FOC中断。
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void SYStick_INT(void) interrupt 10
{
    if (ReadBit(DRV_SR, SYSTIF))                                                              // SYS TICK中断
    {
        g_1mTick=1;
        DRV_SR = (DRV_SR | DCIF) & (~SYSTIF);               // 清零标志位
    }
}


/* -------------------------------------------------------------------------------------------------
    Function Name  : CMP3_INT
    Description    : CMP3：硬件比较器过流保护，关断输出，中断优先级最高
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void CMP3_INT(void)  interrupt 12
{
    if (ReadBit(CMP_SR, CMP3IF))
    {
        /* -----保护处理关输出----- */
        mcFaultSource = FaultHardOVCurrent;
        ClrBit(CMP_SR, CMP3IF);
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : TIM4_INT
    Description    : Timer4用作FG转速反馈
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void TIM4_INT(void) interrupt 11
{
    if (ReadBit(TIM4_CR1, T4IF))
    {
        FG.mcSpeedFlt_temp = (uint32)(mcFocCtrl.SpeedFlt * MOTOR_SPEED_BASE) >> 15;           //实际转速
        
        if ((FG.mcSpeedFlt_temp > 500))
        {
            FG.TIM3_ARR_TEMP = FG.TIMcalc_temp / FG.mcSpeedFlt_temp;
            TIM4__ARR = FG.TIM3_ARR_TEMP;
            TIM4__DR  = FG.TIM3_ARR_TEMP >> 1;
        }
        ClrBit(TIM4_CR1, T4IF);
    }
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : UART2_INT
    Description    : UART2 is for communicate with man chip.
    Date           : 2024-11-28
    Parameter      : None
------------------------------------------------------------------------------------------------- */
uint16 uart2_frameend = 0x0000;
void UART2_INT(void) interrupt 14
{
	if(ReadBit(UT2_CR,UT2RI))	
	{
		uart2_data_t.uart2_recdata[uart2_data_t.rec_incount] = (unsigned char)UT2_DR;
		uart2_frameend = (uint16)((uart2_frameend<<8) | uart2_data_t.uart2_recdata[uart2_data_t.rec_incount]);
		if(uart2_frameend == 0x0123)
			UART2_ReciveData();
		
		uart2_data_t.rec_incount++;
		if(uart2_data_t.rec_incount >= UART_MAX)
			uart2_data_t.rec_incount = 0;
		ClrBit(UT2_CR,UT2RI);
	}
	if(ReadBit(UT2_CR,UT2TI)){
		if(uart2_data_t.send_outcount != uart2_data_t.send_incount){
			UT2_DR = uart2_data_t.uart2_senddata[uart2_data_t.send_outcount++];
			if(uart2_data_t.send_outcount >= UART_MAX)
				uart2_data_t.send_outcount = 0;
		}
		ClrBit(UT2_CR, UT2TI); 
	}
}



