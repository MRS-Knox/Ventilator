#include "Application_Machine_OnOff.h"

/*!
	@brief 		 Machine on task.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void App_Machine_OnOff_Task(void *pvParameter){
	FlagStatus flag_clearparam = SET;
	int measure_flow = 0;
	EventBits_t machine_event = 0x00;
	eMachine_RunStage run_stage = Machine_Stop; 
	while(1){
		machine_event = xEventGroupWaitBits(MachineStateEvent_Handle,Machine_On_Event,pdFALSE,pdFALSE,0);
		/* ERROR!!! */
		if((machine_event&Machine_On_Event) == Machine_On_Event && (machine_event&Machine_Off_Event) == Machine_Off_Event){
			xEventGroupClearBits(MachineStateEvent_Handle,Machine_On_Event);
			machine_event &= (~Machine_On_Event); 
		}

		measure_flow = Mid_CalculateFlow();
		
		if(run_stage != Machine_Stop){
			if((machine_event&Machine_Off_Event) == Machine_Off_Event)
				run_stage = Machine_Stop;
		}
		else if(run_stage == Machine_Stop){
			if((machine_event&Machine_On_Event) == Machine_On_Event)
				run_stage = Machine_Start;
		}
		
		switch(run_stage){
			case Machine_Stop:
				if(flag_clearparam == SET){
					flag_clearparam = RESET;
					Mid_MachineOff_ClearParam();
				}
				break;
			case Machine_Start:
				flag_clearparam = SET;
				Mid_MachineOn_SetParam();
				if((machine_event&MachineOn_PID_Event) == MachineOn_PID_Event)
					run_stage = Machine_DelayIP;
				break;				
			case Machine_DelayIP:
				Run_Param.now_run_p = Mid_DelayIncreasePRESS(Machine_State.flag_delaypress,
															Machine_Time.delaypresstime_1s,
									   						Run_Param.delayp_time,
															Set_Param.start_press,
															Run_Param.delay_end_p);
															
				break;
			case Machine_Run:
					
				break;
			default:break;
		}
		
		
		
		
		
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

