#include "Middle_Parameter.h"

/*!
	@brief 		 This function is about set parameters when the machine is on.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_MachineOn_SetParam(void){
	Run_Param.now_run_p = Set_Param.start_press;
	if(Set_Param.mode == CPAP)
		Run_Param.delay_end_p = Set_Param.therapy_press;
	else if(Set_Param.mode == AutoCPAP)
		Run_Param.delay_end_p = Set_Param.mintherapy_press;

	Machine_Time.machine_runtime_10ms = 0;
	
	/* About delay increase pressure. */
	Machine_State.flag_delaypress = SET;
	if(Set_Param.delaypress_min > 0)
		Run_Param.delayp_time = Set_Param.delaypress_min;
	else if(Set_Param.delaypress_min == 0)
		Run_Param.delayp_time = 1;
}

/*!
	@brief 		 This function is about clear parameters when the machine is off.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_MachineOff_ClearParam(void){
	/* ------ IncreasePID ------- */
	Mid_IncreasePID_Init();
	
	/* ------ Machine_State ------- */
	Machine_State.flag_delaypress = RESET;
	
	/* ------ Machine_Time ------- */
	Machine_Time.machine_runtime_10ms 	= 0;
	Machine_Time.delaypresstime_1s 		= 0;
	Machine_Time.machine_runtime_s 		= 0;
	Machine_Time.machine_runtime_min 	= 0;
	Machine_Time.machine_runtime_hour	= 0;
	Machine_Time.push_btkey_time_10ms	= 0;

	/* ------ Run_Param ------- */
	Run_Param.measure_p 		= 0;
	Run_Param.ins_flow 			= 0;
	Run_Param.ex_flow  			= 0;
	Run_Param.breath_totalflow 	= 0;
	Run_Param.now_set_p 		= 0;
	Run_Param.now_run_p 		= 0;
	Run_Param.delay_end_p 		= 0;
	Run_Param.delayp_time 		= 0;
	Run_Param.delayp_remaintime = 0;
	Run_Param.breathe_stage 	= None;
}


