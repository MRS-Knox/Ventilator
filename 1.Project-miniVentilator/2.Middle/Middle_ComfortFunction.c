#include "Middle_ComfortFunction.h"

/*!
	@brief 		 This function is about ventilator delay increase pressure function.
	@param[in]	 flag_delay:The flag for this function. 
				 real_time:The real-time.
				 delay_min:Delay time and uint is minute.
				 start_press:Single mode start pressure.
				 point_press:Single mode therapy pressure.
	@param[out]	 none
	@retval		 Return running pressure. 
*/
uint16_t Mid_DelayIncreasePRESS(FlagStatus flag_delay,uint16_t real_time,uint8_t delay_min,uint16_t start_press,uint16_t point_press){
	uint16_t return_value = 0;
	uint16_t delaytime_s = delay_min * 60;
	uint16_t press_step = 50;	//0.5cmH2O
	uint16_t step_count = 0; 
	uint16_t keep_steptime = 0;
	uint16_t now_step = 0;
	/* ERROR RUN. */
	if(flag_delay == RESET)
		return point_press;
	/* --------------------------------------------------------------------- */
	/* First stage. */
	if(real_time < (delaytime_s * 0.1)){
		return_value = start_press;
	}
	/* Second stage. */
	else if(real_time < (delaytime_s * 0.2)){
		return_value = start_press + press_step;
	}
	/* Third stage. */
	else if(real_time < delaytime_s){
		step_count = (point_press - (start_press + press_step * 2)) / press_step;
		keep_steptime = (uint16_t)((delaytime_s * 0.8) / step_count);
		now_step = (uint16_t)((real_time - (delaytime_s * 0.2)) / keep_steptime);
		return_value = (start_press + press_step * 2) + (now_step * press_step);
	}
	/* Fourth stage -- Quit. */
	else{
		Machine_State.flag_delaypress = RESET;
		Machine_Time.delaypresstime_1s = 0;
		return_value = point_press;
	}
	return return_value;
}

/*!
	@brief 		 Release pressure when the breathing stage is expiration.
	@param[in]	 *prun_press:.....
				 set_press:Current pressure.
				 now_stage:Breathe stage.
	@param[out]	 *prun_press:.....
	@retval		 return_press:Running pressure.
*/
void Mid_EPR(uint16_t *prun_press,uint16_t set_press,eBreathe_Stage now_stage){
	uint16_t epr_press = set_press - (Set_Param.epr*50);
	uint16_t return_press = 0;
			
	if(now_stage == None || now_stage == Error){
		return_press = *prun_press;
	}
	else if(now_stage == Ins_Start || now_stage == Ins_End){
		if(*prun_press < set_press)
			return_press = *prun_press + 3;
		else
			return_press = set_press;
	}
	else if(now_stage == Ex_Start || now_stage == Ex_End){
		return_press = epr_press;
		return_press = return_press<MACHINE_MINPRESS ? MACHINE_MINPRESS : return_press;
	}
	*prun_press = return_press;
}

/*!
	@brief 		 Auto open machine and auto close machine.
	@param[in]	 flow_data:....
	@param[out]	 none
	@retval		 none
*/
#define AUTOON_MAXFLOW		500	
#define AUTOON_MINFLOW		-500
#define AUTOOFF_FLOW		10000	
void Mid_AutoOn_AutoOff(int flow_data,EventBits_t event_bit){
	static uint8_t autoon_count = 0;
	static uint8_t autooff_count = 0;

	if((event_bit&TestMask_Start_Event) == TestMask_Start_Event || (event_bit&CalibrateStartBlower_Event) == CalibrateStartBlower_Event ||
		(Set_Param.flag_auto_on == RESET && Set_Param.flag_auto_off == RESET)){
		autoon_count = 0;
		autooff_count = 0;
		return;
	}

	if((event_bit&Machine_On_Event) == Machine_On_Event && Set_Param.flag_auto_off == SET){
		autoon_count = 0;
		if(flow_data >= AUTOOFF_FLOW){
			if(autooff_count++ >= 50){	//1s
				autooff_count = 0;
				xEventGroupClearBits(MachineStateEvent_Handle,Machine_On_Event);
				xEventGroupSetBits(MachineStateEvent_Handle,Machine_Off_Event);
				Machine_State.flag_machine_onoff = RESET;
			}
		}
		else 
			autooff_count = 0;
	}
	else if((event_bit&Machine_Off_Event) == Machine_Off_Event && Set_Param.flag_auto_on == SET){
		autooff_count = 0;
		if(flow_data > AUTOON_MAXFLOW){
			
		}
	}
	else{
		autoon_count = 0;
		autooff_count = 0;
	}
	
	// if(Machine_State.flag_machine_onoff == RESET && Set_Param.flag_auto_on == SET){
	// 	if(flow_data >= AUTOON_FLOW){
	// 		if(autoon_count++ >= 3){
	// 			autoon_count = 0;
	// 			xEventGroupSetBits(MachineStateEvent_Handle,Machine_On_Event);
	// 			xEventGroupClearBits(MachineStateEvent_Handle,Machine_Off_Event);
	// 		}
	// 	}
	// 	else if(flow_data < AUTOON_FLOW)
	// 		autoon_count = 0;
	// }
	// if(Machine_State.flag_machine_onoff == SET && Set_Param.flag_auto_off == SET){
	// 	if(flow_data >= AUTOOFF_FLOW){
	// 		if(autooff_count++ >= 3){
	// 			autooff_count = 0;
	// 			xEventGroupSetBits(MachineStateEvent_Handle,Machine_Off_Event);
	// 		}
	// 	}
	// 	else if(flow_data < AUTOOFF_FLOW)
	// 		autooff_count = 0;
	// }
}



