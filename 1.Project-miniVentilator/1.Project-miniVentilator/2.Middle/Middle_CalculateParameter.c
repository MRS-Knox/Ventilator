#include "Middle_CalculateParameter.h"

/*!
	@brief 		 Update the flow value about the end of expiration when machine is just starting.
	@param[in]	 pflow_value:....
	@param[out]	 pstd_buff:Standard deviation.
	@retval		 return_value:.....
*/
int Mid_Update_EXEnd_Flow(int *pflow_value,int *pstd_buff){
	/* Calculate some variables. */
	FlagStatus flag_exend = RESET;
	int exend_buff[LITTLERANGE_LENGENTH];
	long int sum[2] = {0,0};
	int std_mean,flow_mean,max_flow,min_flow,up_limit,down_limit,return_value;
	uint8_t fun_count1 = 0;
	uint8_t fun_count2 = 0;
	uint16_t last_stoppoint  = 0;
	for(uint16_t i = 0;i < MAXFLOWBUFF_COUNT-LITTLERANGE_LENGENTH;i++){
		pstd_buff[i] = Calculate_STD(&pflow_value[i],LITTLERANGE_LENGENTH);
		sum[0] += pstd_buff[i];
		sum[1] += pflow_value[i];
	}
	for(uint16_t i = MAXFLOWBUFF_COUNT-LITTLERANGE_LENGENTH;i < MAXFLOWBUFF_COUNT;i++)
		sum[1] += pflow_value[i];			
	std_mean 	= sum[0] / (MAXFLOWBUFF_COUNT-LITTLERANGE_LENGENTH);
	flow_mean 	= sum[1] / MAXFLOWBUFF_COUNT;
	Find_MIN_MAX(pflow_value,MAXFLOWBUFF_COUNT,&max_flow,&min_flow);
	up_limit 	= max_flow - ((max_flow - flow_mean) / 2);
	down_limit 	= min_flow + ((flow_mean - min_flow) / 5);
	/* Start to find out the flow value about the end of expiration. */	
	for(uint16_t i = 0;i < MAXFLOWBUFF_COUNT-LITTLERANGE_LENGENTH;i++){
		if(flag_exend == RESET){
			if(pstd_buff[i] < std_mean && down_limit < pflow_value[i] && pflow_value[i] < up_limit)
				fun_count1++;
			else
				fun_count1 = 0;
			if(fun_count1 >= CONTINUES_LENGENTH){
				fun_count1 = 0;
				flag_exend = SET;
				if(last_stoppoint != 0 && (i-last_stoppoint) < 50)
					fun_count2--;
			}
		}
		else if(flag_exend == SET && pstd_buff[i] > std_mean){
			flag_exend = RESET;
			last_stoppoint = i;
			if(i >= 5)
				exend_buff[fun_count2++] = pflow_value[i-5];
			else
				exend_buff[fun_count2++] = pflow_value[0];
			fun_count2 = fun_count2>=LITTLERANGE_LENGENTH ? LITTLERANGE_LENGENTH-1 : fun_count2; 
		}
	}
	if(fun_count2 == 0)
		return_value = flow_mean;
	else{
		for(uint8_t i = 0;i < fun_count2;i++)
			return_value += exend_buff[i];
		return_value /= fun_count2;
	}
	return return_value;
}

/*!
	@brief 		 Calculate the running pressure.
	@param[in]	 set_press:Current pressure.
				 now_stage:Breathe stage.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
void Mid_CalculateRunPRESS(uint16_t *prun_press,uint16_t set_press,eBreathe_Stage now_stage){
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
	@brief 		 Update some parameters about BPM when machine on.
	@param[in]	 now_stage:....
                 
	@param[out]	 Run_Param:ins_time,ex_time,ins_ex_scale[2],bpm.
	@retval		 none
	@note		 Running in the timer interrupt.
*/
void Mid_Update_PARAM_AboutBPM(eBreathe_Stage now_stage){
	static eBreathe_Stage last_stage = None;
	uint8_t base_time = 10;	//10ms
	switch(now_stage){
		case None:
			Run_Param.ins_time = 0;
			Run_Param.ex_time = 0;
			break;
		case Ins_Start:
			if(last_stage == Ex_End){
				/* Calculate BPM. */
				Run_Param.bpm = (uint16_t)((60000.0f / (float)(Run_Param.ins_time+Run_Param.ex_time))*10.0f);
				/* Calculate I:E. */
				if(Run_Param.ins_time <= Run_Param.ex_time){
					Run_Param.ins_ex_scale[0] = 10;
					Run_Param.ins_ex_scale[1] = (uint8_t)(((float)Run_Param.ex_time/Run_Param.ins_time)*10);
				}
				else{
					Run_Param.ins_ex_scale[0] = (uint8_t)(((float)Run_Param.ins_time/Run_Param.ex_time)*10);
					Run_Param.ins_ex_scale[1] = 10;
				}
				Run_Param.ins_time = 0;
				Run_Param.ex_time = 0;
			}
			Run_Param.ins_time += base_time;
			break;
		case Ins_End:
			Run_Param.ins_time += base_time;
			break;
		case Ex_Start:
			Run_Param.ex_time += base_time;
			break;
		case Ex_End:
			Run_Param.ex_time += base_time;
			break;
		default:break;
	}
	
	last_stage = now_stage;
}

/*!
	@brief 		 Update the baseline of the one cycle of breathe flow.
	@param[in]	 pflowbuff:....
                 buffcount:....
	@param[out]	 none
	@retval		 return_baseline:.....
*/
int Mid_Update_FlowBaseLine(int *pflowbuff,uint16_t buffcount){
    int return_baseline = 0;
    int flow_sum = 0;
    for(uint16_t i = 0;i < buffcount;i++)
        flow_sum += pflowbuff[i];
    return_baseline = (int)(flow_sum/300);
    return return_baseline;
}


