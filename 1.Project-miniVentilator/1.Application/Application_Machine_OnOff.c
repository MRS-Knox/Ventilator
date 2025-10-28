#include "Application_Machine_OnOff.h"

/*!
	@brief 		 Machine is on or off.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
unsigned char machine_onoff_time = 0;
//int flow_buff1[3000];
//eBreathe_Stage stage1[3000];
//uint16_t flow_buff1_count = 0;
void App_Machine_OnOff_Task(void *pvParameter){
	FlagStatus flag_clearparam = SET;
	FlagStatus flag_machine_start = RESET;
	int flow_buff[MAXFLOWBUFF_COUNT];
	uint16_t flow_buff_count = 0;
	int flow_mean5 = 0;
	EventBits_t machine_event = 0x00;
	eMachine_RunStage run_stage = Machine_Stop; 
	while(1){
		machine_onoff_time = machine_onoff_time>200 ? 0 : machine_onoff_time+1;
		machine_event = xEventGroupWaitBits(MachineStateEvent_Handle,Machine_On_Event,pdFALSE,pdFALSE,0);
		/* ERROR!!! */
		if((machine_event&Machine_On_Event) == Machine_On_Event && (machine_event&Machine_Off_Event) == Machine_Off_Event){
			xEventGroupClearBits(MachineStateEvent_Handle,Machine_On_Event);
			Machine_State.flag_machine_onoff = RESET;
			machine_event &= (~Machine_On_Event); 
		}

		Run_Param.flow_data = Mid_CalculateFlow(Run_Param.flow_data);
		
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
					App_MachineOff_ClearParam();
					for(uint16_t i = 0;i < MAXFLOWBUFF_COUNT;i++)
						flow_buff[i] = 0;
				}
				break;
			case Machine_Start:
				flag_clearparam = SET;
				App_MachineOn_SetParam();
				if((machine_event&MachineOn_PID_Event) == MachineOn_PID_Event){
					if(Machine_State.flag_delaypress == SET)
						run_stage = Machine_DelayIP;
					else
						run_stage = Machine_NoDelayIP;
				}	
				break;				
			case Machine_DelayIP:
				Run_Param.now_set_p = Mid_DelayIncreasePRESS(Machine_State.flag_delaypress,
															Machine_Time.delaypresstime_1s,
									   						Run_Param.delayp_time,
															Set_Param.start_press,
															Run_Param.delay_end_p);
				if(Machine_State.flag_delaypress == RESET || Run_Param.now_set_p >= Run_Param.delay_end_p){
					Machine_State.flag_delaypress = RESET;
					Run_Param.now_set_p = Run_Param.delay_end_p;
					run_stage = Machine_Run;
				}
				break;
			case Machine_NoDelayIP:
				if(flag_machine_start == RESET){
					if(Run_Param.now_set_p++ >= Run_Param.delay_end_p){
						Run_Param.now_set_p = Run_Param.delay_end_p;
						run_stage = Machine_Run;
					}					
				}
				break;
			case Machine_Run:
				//AHI....
				break;
			default:break;
		}
		/* Judge the breathing stage and update the control pressure. */
		if(Machine_DelayIP <= run_stage && run_stage <= Machine_Run){
			/* Machine is just starting. */
			if(flag_machine_start == SET){
				flow_buff[flow_buff_count++] = Run_Param.flow_data;
				Run_Param.flow_sum += Run_Param.flow_data;
				if(flow_buff_count >= MAXFLOWBUFF_COUNT-4){
					flow_mean5 = FlowAverage_Filter(&flow_buff[flow_buff_count-5],5);
					MoveRight_Range(Run_Param.flow_mean_5,5,flow_mean5);
				}
				if(flow_buff_count >= MAXFLOWBUFF_COUNT){
					flow_buff_count = 0;
					flag_machine_start = RESET;
				}
			}
			/* Machine is running. */
			else{
				Run_Param.flow_sum = (Run_Param.flow_sum-flow_buff[0])+Run_Param.flow_data;
				Run_Param.flow_mean = Run_Param.flow_sum / MAXFLOWBUFF_COUNT;
				MoveRight_Range(flow_buff,MAXFLOWBUFF_COUNT,Run_Param.flow_data);
				flow_mean5 = FlowAverage_Filter(&flow_buff[MAXFLOWBUFF_COUNT-5],5);
				MoveRight_Range(Run_Param.flow_mean_5,5,flow_mean5);
				Mid_Judge_BreatheStage(flow_buff,Run_Param.flow_mean,Run_Param.flow_mean_5,&Run_Param.breathe_stage);
				Mid_EPR(&Run_Param.now_run_p,Run_Param.now_set_p,Run_Param.breathe_stage);
			}
		}
		else{
			flow_buff_count = 0;
			flag_machine_start = SET;
		}		
		Mid_AutoOn_AutoOff(Run_Param.flow_data,machine_event);
		
//		stage1[flow_buff1_count] = Run_Param.breathe_stage;
//		flow_buff1[flow_buff1_count++] = Run_Param.flow_data;
//		if(flow_buff1_count >= 3000)
//			flow_buff1_count = 0;
		
		xQueueOverwrite(RunParamQueue_Handle,&Run_Param);
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}

/*!
	@brief 		 This function is about set parameters when the machine is on.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void App_MachineOn_SetParam(void){
	/* Delete!!! */
	Set_Param.mode = CPAP;
	Set_Param.delaypress_min = 0;
	Set_Param.start_press    = 400;
	Set_Param.therapy_press  = 1000;
	Set_Param.epr = 0;

	/*--------------------------------------------*/
	Run_Param.now_run_p = Set_Param.start_press;
	Run_Param.now_set_p = Set_Param.start_press;
	if(Set_Param.mode == CPAP)
		Run_Param.delay_end_p = Set_Param.therapy_press;
	else if(Set_Param.mode == AutoCPAP)
		Run_Param.delay_end_p = Set_Param.mintherapy_press;

	Machine_Time.machine_runtime_10ms = 0;
	
	/* About delay increase pressure. */
	if(Set_Param.delaypress_min > 0){
		Machine_State.flag_delaypress = SET;
		Run_Param.delayp_time = Set_Param.delaypress_min;
		Run_Param.delayp_remaintime = Run_Param.delayp_time;
	}

}

/*!
	@brief 		 This function is about clear parameters when the machine is off.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void App_MachineOff_ClearParam(void){
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
	Run_Param.now_set_p 		= 0;
	Run_Param.now_run_p 		= 0;
	Run_Param.delay_end_p 		= 0;
	Run_Param.delayp_time 		= 0;
	Run_Param.delayp_remaintime = 0;
	Run_Param.breathe_stage 	= None;
	Run_Param.flow_mean			= 0;
	Run_Param.flow_sum			= 0;
	Run_Param.max_flow			= 0;
	Run_Param.min_flow			= 0;
	Run_Param.leak_mean			= 0;
	Run_Param.ins_vt			= 0;
}



