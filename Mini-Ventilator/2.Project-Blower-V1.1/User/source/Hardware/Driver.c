/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : DRIVER.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-04
    Description    : This file contains Driver function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <Myproject.h>
/* -------------------------------------------------------------------------------------------------
    Function Name  : Driver_Init
    Description    : 驱动初始化
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void Driver_Init(void)
{
    DRV_ARR = PWM_VALUE_LOAD - 1;     	                      // 载波频率的周期值
    DRV_DTR = PWM_LOAD_DEADTIME - 1;	                      // 死区时间
    DRV_DR  = 0;

    DRV_CMR = 0x00;                                        
    DRV_OUT = 0x00;   

    ClrBit(DRV_SR , FGIE);                                    //FG中断使能 0-->Disable 1-->Enable

    ClrBit(DRV_SR , DCIF);                                    //清除DRV中断标志位

    SetBit(DRV_SR , DCIM1);                                   //DRV比较匹配中断模式
    ClrBit(DRV_SR , DCIM0);

    DRV_COMR = (PWM_VALUE_LOAD >> 3);                         //设置DRV计数器的比较匹配值

    PDRV1 = 1;                                                // 中断优先级设置为2，优先级低于FO硬件过流
    PDRV0 = 0;

    SetBit(DRV_SR , DCIP);	                                  //0-->1个计数周期产生中断  1-->2个计数周期产生中断

    ClrBit(DRV_CR , FOCEN);
 
    SetBit(DRV_CR , MESEL);                                   //MESEL为0，ME模块工作在BLDC模式  MESEL为1，ME模块工作在FOC/SVPWM/SPWM模式

    SetBit(DRV_CR , DRVEN);	                                  //计数器使能  0-->Disable	1-->Enable
    ClrBit(DRV_CR , DRPE);	                                  //计数器比较值预装载使能 0-->Disable		        1-->Enable
    SetBit(DRV_CR , DRVOE);	                                  //Driver输出使能0-->Disable		1-->Enable
    MOE=0;
}
