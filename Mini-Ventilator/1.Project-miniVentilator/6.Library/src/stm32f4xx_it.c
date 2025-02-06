/**
  ******************************************************************************
  * @file    ADC/ADC_DMA/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 0 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "Use_Struct.h"
#include "Middle_Blower.h"
#include "systick.h"
#include "FreeRTOS.h"
#include "task.h"
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_DMA
  * @{
  */  

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles about .
  * @param   None
  * @retval  None
  */
void EXTI15_10_IRQHandler(void){
}

/**
  * @brief   This function handles is UART7 about .
  * @param   None
  * @retval  None
  */
void UART7_IRQHandler(void){

}

/**
  * @brief   This function handles is USART2 about blower.
  * @param   None
  * @retval  None
  */
void USART2_IRQHandler(void){
	/* Receive interrupt. */
	if(USART_GetITStatus(USART2,USART_IT_RXNE)){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Blower_UartQueue.rx_buff[Blower_UartQueue.rx_in_count++] = (uint8_t)USART_ReceiveData(USART2); 
		/* Judge frame end. */
		if(((Blower_UartQueue.rx_buff[Blower_UartQueue.rx_in_count-2]<<8)|Blower_UartQueue.rx_buff[Blower_UartQueue.rx_in_count-1]) == BLOWER_FRAMEEND)	
			Mid_ReciveMotor();	
		if(Blower_UartQueue.rx_in_count >= UART_MAXBUFF) 
			Blower_UartQueue.rx_in_count = 0;
	}
	/* Transmit interrupt. */
	if(USART_GetITStatus(USART2,USART_IT_TXE)){
		USART_SendData(USART2,(uint16_t)Blower_UartQueue.tx_buff[Blower_UartQueue.tx_out_count++]);
		if(Blower_UartQueue.tx_out_count >= UART_MAXBUFF) 
			Blower_UartQueue.tx_out_count = 0;
		/* Judge whether the transmission is finish. */
		if(Blower_UartQueue.tx_out_count == Blower_UartQueue.tx_in_count){ 
			USART_ITConfig(USART2,USART_IT_TXE,DISABLE);		
		}
	}
	/* Over run error. */
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)){
		USART_ReceiveData(USART2);
	}
}

/**
  * @brief   This function handles is Timer6 about .
  * @param   None
  * @retval  None
  */
void TIM6_DAC_IRQHandler(){
}

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
void SysTick_Handler(void){	
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED){	
		xPortSysTickHandler();
	}
}




/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s/startup_stm32f429x.s).  */
/******************************************************************************/

/**
  * @}
  */ 
  
/**
  * @}
  */   


