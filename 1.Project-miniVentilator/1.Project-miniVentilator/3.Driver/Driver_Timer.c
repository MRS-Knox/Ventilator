#include "Driver_Timer.h"

/*!
	@brief 		 This initial function is about timer hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_Timer6Hardware_Init(uint16_t timer6_prescaler,uint32_t timer6_count){
	TIM_TimeBaseInitTypeDef  Timer6_Init;
	NVIC_InitTypeDef 		 Timer6_NVIC_Init;  
	
	Timer6_Init.TIM_Prescaler		  = timer6_prescaler-1;
	Timer6_Init.TIM_CounterMode		  = TIM_CounterMode_Up;
	Timer6_Init.TIM_Period			  = timer6_count-1;
	Timer6_Init.TIM_ClockDivision	  = TIM_CKD_DIV1;
//	Timer6_Init.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM6,&Timer6_Init);
	
	Timer6_NVIC_Init.NVIC_IRQChannel 			= TIM6_DAC_IRQn;
	Timer6_NVIC_Init.NVIC_IRQChannelPreemptionPriority = 6; 	//RTOS can shield it.
	Timer6_NVIC_Init.NVIC_IRQChannelSubPriority = 0; 			
	Timer6_NVIC_Init.NVIC_IRQChannelCmd 		= ENABLE;
	NVIC_Init(&Timer6_NVIC_Init);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
}

