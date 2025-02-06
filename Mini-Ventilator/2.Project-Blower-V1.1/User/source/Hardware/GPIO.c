/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : GPIO.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-04
    Description    : This file contains GPIO function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <FU68xx_4_MCU.h>
#include <Myproject.h>
/* -------------------------------------------------------------------------------------------------
    Function Name  : GPIO_DefaultInit
    Description    : 没用的IO口配置上拉，提高芯片抗干扰能力
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void GPIO_DefaultInit(void)
{
    P3_OE = 0;
    P3_PU = P33 | P34 ; // 需确认这些端口能接受上拉   
}
/* -------------------------------------------------------------------------------------------------
    Function Name  : GPIO_Init
    Description    : 相关GPIO口初始化配置
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void GPIO_Init(void)
{  
  SetBit(P0_OE , P00);                       //FG开漏输出
//  SetBit(P1_AN , ODE0);	
  SetBit(P0_PU , P00);                       //FG内部上拉
  GP00 = 0;     
 
  ClrBit(P0_OE , P01);                       //PWM输入
  ClrBit(P0_PU , P01);
    
}
