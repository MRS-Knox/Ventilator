#include "Driver_RGB.h"

/*!
	@brief 		 Initialize RGB light hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_RGBHardware_Init(){	
	GPIO_InitTypeDef RGB_GPIO_Init;
	
	RGB_GPIO_Init.GPIO_Pin   = StartR_Pin;
	RGB_GPIO_Init.GPIO_Mode  = GPIO_Mode_OUT;
	RGB_GPIO_Init.GPIO_Speed = GPIO_Fast_Speed;
	RGB_GPIO_Init.GPIO_OType = GPIO_OType_PP;
	RGB_GPIO_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(StartRGB_Group,&RGB_GPIO_Init);
	RGB_GPIO_Init.GPIO_Pin   = StartG_Pin;
	GPIO_Init(StartRGB_Group,&RGB_GPIO_Init);
	RGB_GPIO_Init.GPIO_Pin   = StartB_Pin;
	GPIO_Init(StartRGB_Group,&RGB_GPIO_Init);
	
	RGB_GPIO_Init.GPIO_Pin   = BlueR_Pin;
	GPIO_Init(BlueRGB_Group,&RGB_GPIO_Init);
	RGB_GPIO_Init.GPIO_Pin   = BlueG_Pin;
	GPIO_Init(BlueRGB_Group,&RGB_GPIO_Init);
	RGB_GPIO_Init.GPIO_Pin   = BlueB_Pin;
	GPIO_Init(BlueRGB_Group,&RGB_GPIO_Init);	
	
	GPIO_WriteBit(StartRGB_Group,StartR_Pin|StartG_Pin|StartB_Pin,Bit_RESET);
	GPIO_WriteBit(BlueRGB_Group,BlueR_Pin|BlueG_Pin|BlueB_Pin,Bit_RESET);
}

/*!
	@brief 		 Control RGB light.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_Control_StartRGB(uint8_t light_color,FlagStatus gpio_level){
	uint16_t rgb_pin = 0;
	
	GPIO_WriteBit(StartRGB_Group,StartR_Pin|StartG_Pin|StartB_Pin,Bit_RESET);
	
	if((light_color&R_Light) == R_Light)
		rgb_pin |= StartR_Pin;
	if((light_color&G_Light) == G_Light)
		rgb_pin |= StartG_Pin;
	if((light_color&B_Light) == B_Light)
		rgb_pin |= StartB_Pin;
	GPIO_WriteBit(StartRGB_Group,rgb_pin,(BitAction)gpio_level);
}

/*!
	@brief 		 Control RGB light.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_Control_BlueRGB(uint8_t light_color,FlagStatus gpio_level){	
	uint16_t rgb_pin = 0;
	
	GPIO_WriteBit(BlueRGB_Group,BlueR_Pin|BlueG_Pin|BlueB_Pin,Bit_RESET);
	
	if((light_color&R_Light) == R_Light)
		rgb_pin |= BlueR_Pin;
	if((light_color&G_Light) == G_Light)
		rgb_pin |= BlueG_Pin;
	if((light_color&B_Light) == B_Light)
		rgb_pin |= BlueB_Pin;
	GPIO_WriteBit(BlueRGB_Group,rgb_pin,(BitAction)gpio_level);
}



