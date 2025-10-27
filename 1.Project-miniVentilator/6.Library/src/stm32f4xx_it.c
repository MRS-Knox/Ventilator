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
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "FreeRTOS_Define.h"
#include "Middle_CalculateParameter.h"
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
  * @brief   This function handles about open or close machine.
  * @param   None
  * @retval  None
  */
void EXTI1_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	if(EXTI_GetITStatus(EXTI_Line1) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line1);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == RESET && Machine_State.flag_machine_switch == RESET){
			Machine_State.flag_machine_switch = SET;
		}
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == SET && Machine_State.flag_machine_switch == SET){
			Machine_State.flag_machine_switch = RESET;
			if(Machine_State.flag_machine_onoff == RESET){
				Machine_State.flag_machine_onoff = SET;
                xEventGroupClearBitsFromISR(MachineStateEvent_Handle,Machine_Off_Event);
				xEventGroupSetBitsFromISR(MachineStateEvent_Handle,Machine_On_Event,&xHigherPriorityTaskWoken);
			}
			else if(Machine_State.flag_machine_onoff == SET){
				Machine_State.flag_machine_onoff = RESET;
                xEventGroupClearBitsFromISR(MachineStateEvent_Handle,Machine_On_Event);
				xEventGroupSetBitsFromISR(MachineStateEvent_Handle,Machine_Off_Event,&xHigherPriorityTaskWoken);
			}
		}
	}
}

/**
  * @brief   This function handles about open or close bluetooth chip or close alarm voice.
  * @param   None
  * @retval  None
  */
void EXTI9_5_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line9);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == RESET && Machine_State.flag_bluetooth_switch == RESET){
			Machine_State.flag_bluetooth_switch = SET;
		}
		else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == SET && Machine_State.flag_bluetooth_switch == SET){
			Machine_State.flag_bluetooth_switch = RESET;

            if(Machine_Time.push_btkey_time_10ms >= 500){   //5s
                Machine_Time.push_btkey_time_10ms = 0;
                if(Machine_State.flag_bluetooth_onoff == RESET){
                    Machine_State.flag_bluetooth_onoff = SET;
                    xEventGroupSetBitsFromISR(MachineStateEvent_Handle,BlueTooth_On_Event,&xHigherPriorityTaskWoken);
                }
                else if(Machine_State.flag_bluetooth_onoff == SET){
                    Machine_State.flag_bluetooth_onoff = RESET;
                    xEventGroupSetBitsFromISR(MachineStateEvent_Handle,BlueTooth_Off_Event,&xHigherPriorityTaskWoken);
                }
            }
            else{
                Machine_Time.push_btkey_time_10ms = 0;
                // if(Machine_State.flag_alarmvoice_state == SET){
                //     Machine_State.flag_alarmvoice_state == RESET;
                //     xEventGroupSetBitsFromISR(MachineStateEvent_Handle,AlarmState_Off_Event,&xHigherPriorityTaskWoken);
                // }
            } 
		}
	}
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
UARTQueue_t sendqueue;
FlagStatus flag_send = RESET;
void USART2_IRQHandler(void){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	/* Receive interrupt. */
	if(USART_GetITStatus(USART2,USART_IT_RXNE)){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		BlowerReceiveQueue.buff[BlowerReceiveQueue.buff_count++] = (uint8_t)USART_ReceiveData(USART2);
		if(BlowerReceiveQueue.buff[0] != BLOWER_FRAMEHEADER)
			BlowerReceiveQueue.buff_count = 0;
		if(BlowerReceiveQueue.buff_count >= UART_MAXBUFF){
			BlowerReceiveQueue.buff_count = 0;
			xQueueSendToFrontFromISR(BlowerRECQueue_Handle,&BlowerReceiveQueue,&xHigherPriorityTaskWoken);
		}
	}
	/* Transmit interrupt. */
	if(USART_GetITStatus(USART2,USART_IT_TXE)){
		if(flag_send == RESET){
			if(pdTRUE == xQueueReceiveFromISR(BlowerSendQueue_Handle,&sendqueue,&xHigherPriorityTaskWoken))
				flag_send = SET;
			else	//Transmit data finish.
				USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
		}
		if(flag_send == SET){
			USART_SendData(USART2,(uint16_t)sendqueue.buff[sendqueue.buff_count++]);	
			if(sendqueue.buff_count >= UART_MAXBUFF){
				sendqueue.buff_count = 0;
				flag_send = RESET;
			} 
		}
	}
	/* Over run error. */
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)){
		USART_ReceiveData(USART2);
	}
}

/**
  * @brief   This function handles is Timer6 about machine base time.
  * @param   None
  * @retval  None
  */
#define MAX_COUNT_10MS      30000000
void TIM6_DAC_IRQHandler(){
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);	
		
		Machine_Time.basetime_10ms = Machine_Time.basetime_10ms>=MAX_COUNT_10MS ? 0 : Machine_Time.basetime_10ms+1;
        
        if(Machine_State.flag_machine_onoff == SET)
		    Machine_Time.machine_runtime_10ms = Machine_Time.machine_runtime_10ms>=MAX_COUNT_10MS ? 0 : Machine_Time.machine_runtime_10ms+1;
		
        /* Record the time for the bluetooth key is pushing. */
        if(Machine_State.flag_bluetooth_switch == SET)
            Machine_Time.push_btkey_time_10ms = Machine_Time.push_btkey_time_10ms>=2000 ? 2000 : Machine_Time.push_btkey_time_10ms+1;
        /* ----------------- 1s ------------------- */
		if(Machine_Time.machine_runtime_10ms % 100 == 0 && Machine_State.flag_machine_onoff == SET){
            /* Record the running tims of the machine. */
            if(Machine_Time.machine_runtime_s++ >= 60){
                Machine_Time.machine_runtime_s = 0;
                if(Machine_Time.machine_runtime_min++ >= 60){
                    Machine_Time.machine_runtime_min = 0;
                    Machine_Time.machine_runtime_hour = Machine_Time.machine_runtime_hour>=SINGLEMACHINERUN_MAXTIME ? 
                                                        SINGLEMACHINERUN_MAXTIME : Machine_Time.machine_runtime_hour+1;
                    Machine_Time.machine_totalruntime_hour = Machine_Time.machine_totalruntime_hour>=TOTALMACHINERUN_MAXTIME ? 
                                                        TOTALMACHINERUN_MAXTIME : Machine_Time.machine_totalruntime_hour+1;
                }
            }
            /* Delay increase pressure function. */
            if(Machine_State.flag_delaypress == SET){
                Machine_Time.delaypresstime_1s++;
                if(Set_Param.delaypress_min == 0)
                    Run_Param.delayp_remaintime = 0;
                else if(Set_Param.delaypress_min > 0)
                    Run_Param.delayp_remaintime = Set_Param.delaypress_min - (Machine_Time.delaypresstime_1s/60);
                /* ERROR! 80 minutes! */
                if(Machine_Time.delaypresstime_1s >= 4800){
                    Machine_Time.delaypresstime_1s = 0;
                    Machine_State.flag_delaypress = RESET;
                }
            }
        }
        /* ----------------- Update the monitor parameters. ------------------- */
		if(Machine_State.flag_machine_onoff == SET)
			Mid_Update_MonitorPARAM(Run_Param.breathe_stage);
	}	
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


