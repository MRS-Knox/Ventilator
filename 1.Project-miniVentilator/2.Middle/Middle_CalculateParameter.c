#include "Middle_CalculateParameter.h"

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


