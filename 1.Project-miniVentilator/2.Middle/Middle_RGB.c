#include "Middle_RGB.h"

/*!
	@brief 		 Initialize RGB light.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_RGB_Init(){	
	Dri_RGBHardware_Init();
}

/*!
	@brief 		 Close RGB lights.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_RGB_Close(){	
	Dri_Control_StartRGB(R_Light|G_Light|B_Light,RESET);
	Dri_Control_BlueRGB(R_Light|G_Light|B_Light,RESET);
}

/*!
	@brief 		 RGB lights are flash.
	@param[in]	 light_color:.....
				 flash_frequency:....
				 base_time:Base time.
				 flag_clear:....
	@param[out]	 none
	@retval		 none
*/
void Mid_RGB_Flash(eRGB_Light light_color,uint8_t flash_frequency,uint8_t base_time,FlagStatus flag_clear){	
	uint8_t bright_time = (uint8_t)((1000/flash_frequency)/2); //ms
	static FlagStatus flag_lightstate = RESET;
	static uint8_t run_count = 0;
	if(flag_clear == SET){
		flag_lightstate = RESET;
		run_count = 0;
	}
	if((run_count*base_time) >= bright_time){
		run_count = 0;
		flag_lightstate = flag_lightstate==RESET ? SET :RESET;
		Dri_Control_StartRGB(light_color,flag_lightstate);
		Dri_Control_BlueRGB(light_color,flag_lightstate);
	}
}

/*!
	@brief 		 RGB light task.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_RGB_Task(void *pvParameter){
//	uint8_t light = 0x01;
	while(1){
//		Dri_Control_StartRGB((eRGB_Light)light,SET);
//		Dri_Control_BlueRGB((eRGB_Light)light,SET);
		vTaskDelay(pdMS_TO_TICKS(1000));
//		light = light==0x04 ? 0x01 : light<<1;
	}
}








