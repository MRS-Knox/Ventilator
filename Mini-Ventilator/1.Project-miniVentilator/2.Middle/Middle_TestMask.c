#include "Middle_TestMask.h"

#define TESTMASK_HIGHFLOW		10000	//100lpm
#define TESTMASK_LOWFLOW		8000	//80lpm
#define TESTMASK_COUNT			10	


/*!
	@brief 		 Test the mask gas tightness.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
//void Mid_TestMask(void *pvParameter){
//	uint16_t measure_press = 0;
//	uint16_t time_count = 0;
//	uint8_t mask_count[2] = {0,0};
//	eTestMask_Step test_step = Test_Start;
//	EventBits_t testmask_event = 0;
//	while(1){
//		if(test_step != Test_Stop){
//			testmask_event = xEventGroupWaitBits(MachineStateEvent_Handle,TestMask_Off_Event,pdFALSE,pdFALSE,0);
//			if(testmask_event&TestMask_Off_Event == TestMask_Off_Event){
//				test_step = Test_Stop;
//				xEventGroupClearBits(MachineStateEvent_Handle,TestMask_On_Event);
//			}
//		}
//		switch(test_step){
//			case Test_Stop:
//				Mid_SendToBlower(SEND_RPM,0);
//				if(time_count++ >= 5){
//					time_count = 0;
//					test_step = Test_Start;
//					xEventGroupWaitBits(MachineStateEvent_Handle,TestMask_On_Event,pdFALSE,pdFALSE,portMAX_DELAY);
//					xEventGroupClearBits(MachineStateEvent_Handle,TestMask_Off_Event);
//				}
//				break;
//			case Test_Start:
//				Mid_SendToBlower(SEND_RPM,START_RPM);
//				if(Blower_State.actual_rpm >= MOTOR_SPEED_MIN){
//					if(time_count++ >= 50){
//						time_count = 0;
//						test_step = Test_Run;
//						mask_count[0] = 0;
//						mask_count[1] = 0;
//						Run_Param.testmask_set_p = 400;		
//						Machine_State.flag_ajust_mask = RESET;
//					}
//				}
//				break;				
//			case Test_Run:
//				/* Increase pressure. */
//				if(time_count++ >= 50){
//					time_count = 0;
//					Run_Param.testmask_set_p = Run_Param.testmask_set_p>=1000 ? 1000 : Run_Param.testmask_set_p+200;
//				}
//				/* Judge mask leakage base on flow value. */
//				if(Run_Param.flow_data >= TESTMASK_HIGHFLOW){
//					mask_count[1] = 0;
//					if(mask_count[0]++ >= TESTMASK_COUNT){
//						mask_count[0] = 0;
//						Machine_State.flag_ajust_mask = SET;
//					}
//				}
//				else if(Run_Param.flow_data < TESTMASK_LOWFLOW){
//					mask_count[0] = 0;
//					if(mask_count[1]++ >= TESTMASK_COUNT){
//						mask_count[1] = 0;
//						Machine_State.flag_ajust_mask = RESET;
//					}
//				}
//				else{
//					mask_count[0] = 0;
//					mask_count[1] = 0;
//				}
//				/* Control pressure. */
//				measure_press = (uint16_t)Mid_CalculatePRESS();
//				Mid_IncreasePID(Run_Param.testmask_set_p,measure_press);
//				break;
//			default: 
//				break;
//		}
//		vTaskDelay(pdMS_TO_TICKS(10));
//	}		
//}

