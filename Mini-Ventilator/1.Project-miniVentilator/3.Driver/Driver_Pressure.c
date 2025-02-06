#include "Driver_Pressure.h"

/*!
	@brief 		 This initial function is about pressure hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_PHardware_Init(void){
	GPIO_InitTypeDef 	PRESS_GPIO_Init;	
	ADC_InitTypeDef 	PRESS_ADC_Init; 
	/* Initialize GPIO. */
	PRESS_GPIO_Init.GPIO_Pin 	= PRESS_Pin;	
	PRESS_GPIO_Init.GPIO_Mode 	= GPIO_Mode_AN; 
	PRESS_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_NOPULL; 
	GPIO_Init(PRESS_Group,&PRESS_GPIO_Init);
	/* Initialize ADC. */
	PRESS_ADC_Init.ADC_Resolution 			= ADC_Resolution_12b; //Maximum resolution.
	PRESS_ADC_Init.ADC_ScanConvMode 		= DISABLE;	
	PRESS_ADC_Init.ADC_ContinuousConvMode 	= ENABLE;	
	PRESS_ADC_Init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	PRESS_ADC_Init.ADC_ExternalTrigConv 	= ADC_ExternalTrigConv_T1_CC1;
	PRESS_ADC_Init.ADC_DataAlign 			= ADC_DataAlign_Right; 
	PRESS_ADC_Init.ADC_NbrOfConversion 		= 1; 
	ADC_Init(ADC1,&PRESS_ADC_Init);	
	/* Enable ADC1. */
	ADC_Cmd(ADC1,ENABLE); 
}

/*!
	@brief 		 This function is about read pressure sensor output voltage.
	@param[in]	 none
	@param[out]	 none
	@retval		 out_v:Output voltage.
*/
uint16_t Dri_ReadPRESS_V(void){
	uint16_t adc_buff[10];
	uint16_t out_v = 0;
	ADC_ContinuousModeCmd(ADC1,ENABLE); 
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_56Cycles);
	
	ADC_SoftwareStartConv(ADC1);	
	for(uint8_t count = 0;count < 10;){
		if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)){ 	
			adc_buff[count] = ADC_GetConversionValue(ADC1);
			ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
			count++;
		}
	}
	ADC_ContinuousModeCmd(ADC1,DISABLE); 
	out_v = Average_Filter_Int(adc_buff,10);
	return out_v;
}



