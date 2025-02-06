#ifndef Driver_Blower_H
#define Driver_Blower_H

#include "stm32f4xx_conf.h"
#include "Add_Function.h"

#define POWER_Pin			GPIO_Pin_11
#define RESET_Pin			GPIO_Pin_10
#define P_R_Group			GPIOC
#define USART2_TX_Pin		GPIO_Pin_2
#define USART2_RX_Pin		GPIO_Pin_3
#define USART2_TX_RX_Group	GPIOA

/* This initial function is about blower hardware. */
void Dri_BlowerHardware_Init(void);
/* This control function is about USART2 interrupt. */
void Dri_BlowerControlSend(FunctionalState tx_state);

#endif
