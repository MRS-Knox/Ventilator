#include "Driver_Key.h"

/*!
	@brief 		 This initial function is about key hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_KeyHardware_Init(void){
	GPIO_InitTypeDef 	Key_GPIO_Init;
	EXTI_InitTypeDef   	Key_EXTI_Init;
	NVIC_InitTypeDef   	Key_NVIC_Init;
	
	Key_GPIO_Init.GPIO_Pin 		= OpenKey_Pin; 
	Key_GPIO_Init.GPIO_Mode 	= GPIO_Mode_IN;
	Key_GPIO_Init.GPIO_Speed 	= GPIO_Speed_50MHz;
	Key_GPIO_Init.GPIO_OType 	= GPIO_OType_OD;
	Key_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(OpenKey_Group,&Key_GPIO_Init);
	Key_GPIO_Init.GPIO_Pin 		= BlueKey_Pin; 
	GPIO_Init(BlueKey_Group,&Key_GPIO_Init);
	/* Connect EXTI line1 to PA1 pin,connect EXTI line9 to PB9 pin. */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);
	/* Configure EXTI line1 and line9. */
	Key_EXTI_Init.EXTI_Line 	= EXTI_Line1;
	Key_EXTI_Init.EXTI_Mode 	= EXTI_Mode_Interrupt;
	Key_EXTI_Init.EXTI_Trigger 	= EXTI_Trigger_Rising_Falling;  
	Key_EXTI_Init.EXTI_LineCmd 	= ENABLE;
	EXTI_Init(&Key_EXTI_Init);
	Key_EXTI_Init.EXTI_Line 	= EXTI_Line9;
	Key_EXTI_Init.EXTI_Mode 	= EXTI_Mode_Interrupt;
	Key_EXTI_Init.EXTI_Trigger 	= EXTI_Trigger_Rising_Falling;  
	Key_EXTI_Init.EXTI_LineCmd 	= ENABLE;
	EXTI_Init(&Key_EXTI_Init);

	/* Enable and set EXTI line1 and line9 interrupt. */
	Key_NVIC_Init.NVIC_IRQChannel 					= EXTI1_IRQn;
	Key_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 10;		//RTOS can shield it.
	Key_NVIC_Init.NVIC_IRQChannelSubPriority 		= 0;
	Key_NVIC_Init.NVIC_IRQChannelCmd 				= ENABLE;
	NVIC_Init(&Key_NVIC_Init);	
	Key_NVIC_Init.NVIC_IRQChannel 					= EXTI9_5_IRQn;
	Key_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 10;		//RTOS can shield it.
	Key_NVIC_Init.NVIC_IRQChannelSubPriority 		= 0;
	Key_NVIC_Init.NVIC_IRQChannelCmd 				= ENABLE;
	NVIC_Init(&Key_NVIC_Init);	
}

