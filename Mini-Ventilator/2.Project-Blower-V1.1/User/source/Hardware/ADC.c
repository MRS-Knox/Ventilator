/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : ADC.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-04
    Description    : This file contains ADC function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */
#include <FU68xx_4_MCU.h>
#include <Myproject.h>
/* -------------------------------------------------------------------------------------------------
    Function Name  : ADC_Init
    Description    : ADC初始化配置
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void ADC_Init(void)
{
    SetBit(P2_AN , PIN0);	                                  //AD0 P20 CH open--IU--固定
    SetBit(P2_AN , PIN3);	                                  //AD1 P23 CH open--IV--固定 
    SetBit(P2_AN , PIN4);                                     //AD2 P24 CH open--DCBUS--固定
    SetBit(P2_AN , PIN5);	                                  //AD3 P25 CH open
    SetBit(P2_AN , PIN7);	                                  //AD4 P27 CH open--IBUS/IW--固定
    
    #if (SPEED_MODE == SREFMODE)
    {
        SetBit(P3_AN , PIN4);	                              //AD7 P34 CH open
        SetBit(ADC_MASK , CH7EN);
    }
    #endif
    
    SetBit(ADC_MASK , CH0EN | CH1EN | CH2EN | CH3EN | CH4EN  | CH14EN);   //通道使能
    
    #if (VOLTAGEMode ==Inter_VOLTAGE)
    {
        #if (VOLTAGE_SCALE ==Ratio_6_5)
        {
            SetBit(ADC_CR, ADCRATIO);                         //AD14采用1/6.5分压比
        }
        #else
        {
            ClrBit(ADC_CR, ADCRATIO);                         //AD14采用1/12分压比
        }
        #endif
    }
    #endif
    
    SetBit(ADC_CR , ADCALIGN); 		                          //ADC数据次高位对齐使能0-->Disable	1-->Enable
    ClrBit(ADC_CR , ADCIE); 		                          //ADC中断使能
    SetBit(ADC_CR , ADCEN);                                   //Enable ADC0
}

