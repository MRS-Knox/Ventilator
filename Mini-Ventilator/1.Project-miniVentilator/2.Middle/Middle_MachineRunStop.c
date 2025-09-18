#include "Middle_MachineRunStop.h"

/*!
	@brief 		 The ventilator is in an opening state.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
// uint16_t rpm11 = 15000;
// uint16_t rpm111[141];
// uint16_t in1111 = 10;
// uint16_t flag11 = 0;
// uint16_t setpress = 400;
// uint16_t setpress1 = 500;
// uint16_t setpress2 = 1000;
// uint16_t count = 0;
// void Mid_MachineRun_Task(void *pvParameter){
// 	uint16_t time_count = 0;
// 	int measure_press = 0;	
// 	eMachine_Open bloweropen_step = Stop;
// 	EventBits_t machine_event = 0;
// 	while(1){
// 		machine_event = xEventGroupWaitBits(MachineStateEvent_Handle,
// 											Machine_On_Event|Machine_Off_Event,
// 											pdFALSE,pdFALSE,0);

// 		/* Wait for the user close the ventilator. */
// 		if(bloweropen_step != Stop){
// 			if((machine_event&Machine_Off_Event) == Machine_Off_Event){
// 				machine_event = Machine_Off_Event;
// 				xEventGroupClearBits(MachineStateEvent_Handle,Machine_On_Event);
// 				Mid_SendToBlower(SEND_RPM,0);
// 				Mid_MachineOff_ClearParam();
// 				bloweropen_step = Stop;
// 			}
// 		}
// 		switch(bloweropen_step){
// 			/* Wait for the user open the ventilator. */
// 			case Stop:
// 				if((machine_event&Machine_On_Event) == Machine_On_Event){
// 					machine_event = Machine_On_Event;
// 					xEventGroupClearBits(MachineStateEvent_Handle,Machine_Off_Event);
// 					Mid_MachineOn_SetParam();
// 					bloweropen_step = Start;
// 					time_count = 0;
// 				}
// 				break;
// 			/* The ventilator is in the first open state. */
// 			case Start:
// 				Mid_SendToBlower(SEND_RPM,START_RPM);
// 				if(Blower_State.actual_rpm >= MOTOR_SPEED_MIN){
// 					if(time_count++ >= 100){
// 						time_count = 0;
// 						bloweropen_step = DelayIP;
// 					}
// 				}
// 				break;
// 			/* The ventilator is in the second open state. */
// 			case DelayIP:
// 				if(Machine_State.flag_delaypress == RESET){
// 					bloweropen_step = Run;
// 					Run_Param.now_set_p = Run_Param.delay_end_p;
// 					break;
// 				}
// 				Run_Param.now_set_p = Mid_DelayIncreasePRESS(Machine_State.flag_delaypress,
// 														  	 Machine_Time.delaypresstime_1s,
// 														   	 Run_Param.delayp_time,
// 														   	 Set_Param.start_press,
// 														   	 Run_Param.delay_end_p);
// 				break;
// 			/* The ventilator is in the third open state. */
// 			case Run:
// //				if(count++ >= 500){
// //					count = 0;
// //					Run_Param.now_run_p = Run_Param.now_run_p==setpress1 ? setpress2 : setpress1;
// //				}			
// 				break;
// 			default:
// 				break;
// 		}

// 		if(bloweropen_step >= DelayIP){
// 			/* Measure pressure. */
// 			measure_press = Mid_CalculatePRESS();
// 			/* Ajust ventilator running pressure base on breathe stage. */
// 			if(Run_Param.breathe_stage == Ins_Start || Run_Param.breathe_stage == Ins_End)
// 				Run_Param.now_run_p = Mid_Inspiration_Stage(Run_Param.now_set_p,Set_Param.mode,Run_Param.breathe_stage);
// 			else if(Run_Param.breathe_stage == Ex_Start || Run_Param.breathe_stage == Ex_End)
// 				Run_Param.now_run_p = Mid_Expiration_Stage(Run_Param.now_set_p,Set_Param.mode,Run_Param.breathe_stage);
// 			else if(Run_Param.breathe_stage == None)
// 				Run_Param.now_run_p = Run_Param.now_set_p;
// 			/* Control pressure. */	
// 			Mid_IncreasePID(Run_Param.now_run_p,(uint16_t)measure_press);
// 		}
// 		vTaskDelay(pdMS_TO_TICKS(10));
// 	}
// }
