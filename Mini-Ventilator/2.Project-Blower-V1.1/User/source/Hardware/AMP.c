/* --------------------------- (C) COPYRIGHT 2021 Fortiortech ShenZhen -----------------------------
    File Name      : AMP.c
    Author         : Lewis.wang
    Version        : V1.0
    Date           : 2021-12-04
    Description    : This file contains AMP function used for Motor Control.
----------------------------------------------------------------------------------------------------  
                                       All Rights Reserved
------------------------------------------------------------------------------------------------- */

#include <FU68xx_4_MCU.h>
#include <Myproject.h>

/* -------------------------------------------------------------------------------------------------
    Function Name  : AMP_Init
    Description    : 运放初始化配置
    Date           : 2021-12-04
    Parameter      : None
------------------------------------------------------------------------------------------------- */
void AMP_Init(void)
{
    #if (Shunt_Resistor_Mode == Single_Resistor)
    {     
        SetBit(P3_AN, P31);                                       //AMP0 Pin设置为模拟模式  +
        SetBit(P3_AN, P30);                                       //AMP0 Pin设置为模拟模式  -
        SetBit(P2_AN, P27);                                       //AMP0 Pin设置为模拟模式  O
        
        #if (AMP0Mode == Inter_AMP0)    // 内部运放
        {
            SetReg(AMP0_GAIN, AMP0_GAIN2 | AMP0_GAIN1 | AMP0_GAIN0, MCU_AMP0GAIN);
        }
        #elif (AMP0Mode == Exter_AMP0)
        {
            SetReg(AMP0_GAIN, AMP0_GAIN2 | AMP0_GAIN1 | AMP0_GAIN0, 0x000);
        }
        #endif
        
        SetBit(AMP_CR, AMP0EN);      
    }
    #elif (Shunt_Resistor_Mode == Double_Resistor)
    {
        SetBit(P3_AN, P31);                                       //AMP0 Pin设置为模拟模式  +
        SetBit(P3_AN, P30);                                       //AMP0 Pin设置为模拟模式  -
        SetBit(P2_AN, P27);                                       //AMP0 Pin设置为模拟模式  O
    
        SetBit(P1_AN, P16);                                       //AMP1 Pin设置为模拟模式  +
        SetBit(P1_AN, P17);                                       //AMP1 Pin设置为模拟模式  -
        SetBit(P2_AN, P20);                                       //AMP1 Pin设置为模拟模式  O
    
        SetBit(P2_AN, P21);                                       //AMP2 Pin设置为模拟模式  +
        SetBit(P2_AN, P22);                                       //AMP2 Pin设置为模拟模式  -
        SetBit(P2_AN, P23);                                       //AMP2 Pin设置为模拟模式  O
        ClrBit(P2_OE, P23);                                       //P23_OE需要强制为0，禁止DA1输出至PAD
    
        #if (AMP0Mode == Inter_AMP0)
        {
            SetReg(AMP0_GAIN, AMP0_GAIN2 | AMP0_GAIN1 | AMP0_GAIN0, MCU_AMP0GAIN);
        }
        #elif (AMP0Mode == Exter_AMP0)
        {
            SetReg(AMP0_GAIN, AMP0_GAIN2 | AMP0_GAIN1 | AMP0_GAIN0, 0x000);
        }
        #endif
    
        SetBit(AMP_CR, AMP0EN);
        SetBit(AMP_CR, AMP1EN);                                    //AMP1 Enable
        SetBit(AMP_CR, AMP2EN);                                    //AMP2 Enable
    }
    #endif
}
