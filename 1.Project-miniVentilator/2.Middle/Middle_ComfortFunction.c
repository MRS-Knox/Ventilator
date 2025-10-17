#include "Middle_ComfortFunction.h"

#define AUTOON_FLOW			1000	//10lpm
#define AUTOOFF_FLOW		10000	//100lpm


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
	@brief 		 This function is about ventilator auto on and auto off.
	@param[in]	 flow_data:Flow value. 
	@param[out]	 none
	@retval		 none
*/
void Mid_AutoOn_AutoOff(int flow_data){
	static uint8_t autoon_count = 0;
	static uint8_t autooff_count = 0;
	
	if(Machine_State.flag_machine_onoff == RESET && Set_Param.flag_auto_on == SET){
		if(flow_data >= AUTOON_FLOW){
			if(autoon_count++ >= 3){
				autoon_count = 0;
				xEventGroupSetBits(MachineStateEvent_Handle,Machine_On_Event);
			}
		}
		else if(flow_data < AUTOON_FLOW)
			autoon_count = 0;
	}
	if(Machine_State.flag_machine_onoff == SET && Set_Param.flag_auto_off == SET){
		if(flow_data >= AUTOOFF_FLOW){
			if(autooff_count++ >= 3){
				autooff_count = 0;
				xEventGroupSetBits(MachineStateEvent_Handle,Machine_Off_Event);
			}
		}
		else if(flow_data < AUTOOFF_FLOW)
			autooff_count = 0;
	}
}

/*!
	@brief 		 This function is about ventilator release expiration pressure.
	@param[in]	 set_press:Machine current run pressure.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
uint16_t Mid_EPR_Function(uint16_t set_press){
	uint16_t return_press = set_press - (Set_Param.epr*50);

	return (return_press < MACHINE_MINPRESS ? MACHINE_MINPRESS : return_press);
}



