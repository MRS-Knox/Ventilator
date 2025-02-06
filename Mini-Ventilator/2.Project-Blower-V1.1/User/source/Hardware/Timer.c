/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : TIMER.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-04
    Description    : This file contains Timer function used for Motor Control.
----------------------------------------------------------------------------------------------------
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

#include <FU68xx_4_MCU.h>
#include <Myproject.h>
/* -------------------------------------------------------------------------------------------------
    Function Name  : Timer2_Init
    Description    : Timer2初始化
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Timer2_Init(void)
{
    ClrBit(TIM2_CR1, T2CEN);
    SetReg(TIM2_CR0, T2PSC0 | T2PSC1 | T2PSC2, T2PSC0 | T2PSC1 | T2PSC2);    //000-->24M    001-->12M   010-->6M    011-->3M 100-->1.5M 101-->750K  110-->375K  111-->187.5K
    SetReg(TIM2_CR0, T2MOD0 | T2MOD1, T2MOD0);
    ClrBit(TIM2_CR0, T2CES);
    ClrBit(TIM2_CR1, T2IR | T2IF | T2IP);
    SetBit(TIM2_CR1, T2IFE);
    TIM2__ARR  = (TailWind_Time * TIM2_Fre / 1000);
    TIM2__DR   = TIM2__ARR;
    TIM2__CNTR = 0;
    SetBit(TIM2_CR1, T2CEN);
}

/* -------------------------------------------------------------------------------------------------
    Function Name  : Timer3_Init
    Description    : Timer3初始化
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Timer3_Init(void)
{
    SetBit(PH_SEL, T3SEL);                        //Timer3端口使能
    SetBit(PH_SEL1, T3CT);                        //默认端口为P11,功能转移后为P01,需TIMER4转移到P00
    ClrBit(TIM3_CR0, T3PSC2);                     //计数器时钟分频选择
    ClrBit(TIM3_CR0, T3PSC1);                     //000-->24M   001-->12M   010-->6M    011-->3M
    ClrBit(TIM3_CR0, T3PSC0);                     //100-->1.5M  101-->750K  110-->375K  111-->187.5K（注意：Timer3在输入捕获模式下 111对应的是48MHZ，其他模式对应187.5k）
    ClrBit(TIM3_CR0, T3OCM);
    ClrBit(TIM3_CR0, T3IRE);                        //比较匹配中断/脉宽检测中断0-->Disable  1-->Enable
    ClrBit(TIM3_CR0, T3OPM);                        //0-->计数器不停止 1-->单次模式
    SetBit(TIM3_CR1, T3IPE);                        //输入Timer PWM周期检测中断使能 0-->Disable  1-->Enable
    SetBit(TIM3_CR1, T3IFE);                        //计数器上溢中断使能 0-->Disable  1-->Enable
    SetBit(TIM3_CR1, T3NM1);                      //输入噪声脉宽选择
    SetBit(TIM3_CR1, T3NM0);                        //00-->不滤波    01-->4cycles 10-->8cycles  11-->16cycles
    TIM3__ARR = 0;
    TIM3__DR = 0;
    TIM3__CNTR = 1;
    PTIM31 = 0; 
    PTIM30 = 0;
    ClrBit(TIM3_CR0, T3MOD);                      //0-->Timer模式  1-->输出模式
    SetBit(TIM3_CR1, T3EN);                       //TIM3使能    0-->Disable  1-->Enable
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : Timer4_Init
    Description    : Timer4初始化
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Timer4_Init(void)
{
    SetBit(PH_SEL, T4SEL);                        //Timer4端口使能
    SetBit(PH_SEL1, T4CT);                        //默认端口为P01,功能转移后为P00
    SetReg(TIM4_CR0, T4PSC2 | T4PSC1 | T4PSC0, T4PSC2);   //000-->24M      001-->12M       010-->6M    011-->3M
    //100-->1.5M   101-->750K      110-->375K  111-->187.5K
    SetBit(TIM4_CR0, T4OCM);
    ClrBit(TIM4_CR0, T4IRE);                        //比较匹配中断/脉宽检测中断0-->Disable  1-->Enable
    ClrBit(TIM4_CR0, T4OPM);                        //0-->计数器不停止 1-->单次模式
    ClrBit(TIM4_CR1, T4IPE);                        //输入Timer PWM周期检测中断使能 0-->Disable  1-->Enable
    SetBit(TIM4_CR1, T4IFE);                        //计数器上溢中断使能 0-->Disable  1-->Enable
    ClrBit(TIM4_CR1, T4NM1);                        //输入噪声脉宽选择
    ClrBit(TIM4_CR1, T4NM0);                        //00-->不滤波    01-->4cycles  10-->8cycles  11-->16cycles
    TIM4__DR = 5000;
    TIM4__ARR = 10000;
    TIM4__CNTR = 0;
    SetBit(TIM4_CR0, T4MOD);                        //0-->Timer模式  1-->输出模式
    ClrBit(TIM4_CR1, T4EN);                         //TIM4使能    0-->Disable  1-->Enable
}
