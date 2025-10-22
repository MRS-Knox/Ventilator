// #include "Middle_RGB.h"

// const uint32_t display_bright[13] = {
// 	Bright_0,Bright_1,Bright_2,Bright_3,Bright_4,Bright_5,Bright_6,
// 	Bright_7,Bright_8,Bright_9,Bright_10,Bright_11,Bright_12
// };

// /*!
// 	@brief 		 This initial function is about RGB light hardware.
// 	@param[in]	 none
// 	@param[out]	 none
// 	@retval		 none
// */
// void Mid_RGB_Init(void){
// 	Dri_RGBHardware_Init();
// }

// /*!
// 	@brief 		 RGB light task.
// 	@param[in]	 none
// 	@param[out]	 none
// 	@retval		 none
// */
// void Mid_RGB_Task(void *pvParameter){
// 	EventBits_t rgb_event = 0;
// 	uint8_t rgb_depth_buff[RGB_Count][3];
// 	uint8_t rgb_bright = 1;
// 	uint32_t display_light = display_bright[0];
// 	uint32_t lastdisplay_light = display_bright[1];

// 	while(1){
// 		rgb_event = xEventGroupWaitBits(MachineStateEvent_Handle,Machine_Off_Event|Machine_On_Event,
// 										pdFALSE,pdFALSE,0);
// 		/* Display white color when machine off. */		
// 		if((rgb_event & Machine_Off_Event) == Machine_Off_Event){
// //			display_light = White_Light & display_bright[rgb_bright];
// 			display_light = White_Light & display_bright[0];
// 			if(rgb_bright < 12)
// 				rgb_bright++;
// 		}
// 		/* Display nothing color when machine on. */		
// 		if((rgb_event & Machine_On_Event) == Machine_On_Event){
// 			display_light = display_bright[0];
// 			rgb_bright = 1;
// 		}


// 		if(display_light != lastdisplay_light){
// 			lastdisplay_light = display_light;
// 			for(uint8_t count = 0;count < RGB_Count;count++){
// 				rgb_depth_buff[count][0] = (uint8_t)(display_light>>16);	//Red.
// 				rgb_depth_buff[count][1] = (uint8_t)(display_light>>8);		//Green.
// 				rgb_depth_buff[count][2] = (uint8_t)(display_light);		//Blue.
// 			}
// 			Dri_SendDataTo_WS2812B(RGB_Count,rgb_depth_buff);
// 		}

// 		// vTaskSuspend(NULL);
// 		vTaskDelay(pdMS_TO_TICKS(250));
// 	}
// }


