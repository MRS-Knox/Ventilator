#include "Driver_Blower.h"

/*!
	@brief 		 This initial function is about blower hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_BlowerHardware_Init(void){
	GPIO_InitTypeDef 	Blower_GPIO_Init;
	GPIO_InitTypeDef 	Blower_UARTGPIO_Init;
	USART_InitTypeDef 	Blower_UART_Init;
	NVIC_InitTypeDef 	Blower_NVIC_Init;  
	/* Initialize GPIO. */
	Blower_GPIO_Init.GPIO_Pin 	= POWER_Pin; 
	Blower_GPIO_Init.GPIO_Mode 	= GPIO_Mode_OUT;
	Blower_GPIO_Init.GPIO_Speed = GPIO_Medium_Speed;
	Blower_GPIO_Init.GPIO_OType = GPIO_OType_PP;
	Blower_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(P_R_Group,&Blower_GPIO_Init);
	Blower_GPIO_Init.GPIO_Pin 	= RESET_Pin;
	Blower_GPIO_Init.GPIO_OType = GPIO_OType_OD; 
	GPIO_Init(P_R_Group,&Blower_GPIO_Init);
	/* Initialize UART. */
	GPIO_PinAFConfig(USART2_TX_RX_Group,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(USART2_TX_RX_Group,GPIO_PinSource3,GPIO_AF_USART2);
	Blower_UARTGPIO_Init.GPIO_Pin 	= USART2_TX_Pin | USART2_RX_Pin; 
	Blower_UARTGPIO_Init.GPIO_Mode 	= GPIO_Mode_AF;
	Blower_UARTGPIO_Init.GPIO_Speed = GPIO_High_Speed;
	Blower_UARTGPIO_Init.GPIO_OType = GPIO_OType_PP;
	Blower_UARTGPIO_Init.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(USART2_TX_RX_Group,&Blower_UARTGPIO_Init);
	Blower_UART_Init.USART_BaudRate 			= 115200; 
	Blower_UART_Init.USART_WordLength 			= USART_WordLength_8b;
	Blower_UART_Init.USART_StopBits 			= USART_StopBits_1;
	Blower_UART_Init.USART_Parity 				= USART_Parity_No;
	Blower_UART_Init.USART_Mode 				= USART_Mode_Tx|USART_Mode_Rx;
	Blower_UART_Init.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_Init(USART2,&Blower_UART_Init);
	/* Initialize NVIC. */
	Blower_NVIC_Init.NVIC_IRQChannel 			= USART2_IRQn;
	Blower_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 4; 	//RTOS can't shield it.
	Blower_NVIC_Init.NVIC_IRQChannelSubPriority = 0; 			
	Blower_NVIC_Init.NVIC_IRQChannelCmd 		= ENABLE;
	NVIC_Init(&Blower_NVIC_Init);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		
	USART_ITConfig(USART2,USART_IT_TXE,DISABLE);		
	/* Blower power on. */
	GPIO_WriteBit(P_R_Group,POWER_Pin,Bit_SET); 
	/* Reset the blower chip. */
	GPIO_WriteBit(P_R_Group,RESET_Pin,Bit_RESET);
	udelay_1ms(2);
	GPIO_WriteBit(P_R_Group,RESET_Pin,Bit_SET);
	udelay_1ms(2);
	/* Enable USART2. */
	USART_Cmd(USART2,ENABLE); 
}
	
/*!
	@brief 		 This control function is about USART2 interrupt.
	@param[in]	 tx_state:ENABLE or DISABLE.
	@param[out]	 none
	@retval		 none
*/
void Dri_BlowerControlSend(FunctionalState tx_state){
	USART_ITConfig(USART2,USART_IT_TXE,tx_state); 
}

