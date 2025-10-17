#include "Middle_BreatheStage.h"

#define STD_LENGENTH 			5

/*!
	@brief 		 Judge the breathe stage.
	@param[in]	 pflow_buff:Point to the flow.
				 pexend_flow:Point to the flow about the end of expiration.
				 pnow_stage:.....
	@param[out]	 pexend_flow:.....
				 pstd_buff:Point to the standard deviation.
				 pnow_stage:.....
	@retval		 none
*/
void Mid_Judge_BreatheStage(int *pflow_buff,int *pexend_flow,int *pstd_buff,eBreathe_Stage *pnow_stage,int *std_mean_1){
	static uint8_t func_count[3] = {0,0,0};
	static uint8_t err_count = 0;
	int now_std,std_mean;
	eBreathe_Stage last_stage = *pnow_stage;
	int now_flow = pflow_buff[MAXFLOWBUFF_COUNT-1];
	int last_flow = pflow_buff[MAXFLOWBUFF_COUNT-2];
	long int sum = 0;
	int threshold_uplimit 	= *pexend_flow + 300;
//	int threshold_downlimit = *pexend_flow - 200;
	int flow_derivative = now_flow-last_flow;

	now_std = Calculate_STD(&pflow_buff[MAXFLOWBUFF_COUNT-STD_LENGENTH],STD_LENGENTH);
	MoveRight_Range(pstd_buff,MAXFLOWBUFF_COUNT-STD_LENGENTH,now_std);
	for(uint16_t i = 0;i < MAXFLOWBUFF_COUNT-STD_LENGENTH;i++)
		sum += pstd_buff[i];
	std_mean = sum / (MAXFLOWBUFF_COUNT-STD_LENGENTH);
	*std_mean_1 = std_mean;

	if((now_flow > threshold_uplimit) && (flow_derivative > 150) && (last_stage == None || last_stage == Ex_End))
		*pnow_stage = Ins_Start;

	if((now_flow > threshold_uplimit) && (last_stage == Ins_Start) && (now_std < std_mean || flow_derivative < 100)){
		if(func_count[0]++ >= 5){
			func_count[0] = 0;
			*pnow_stage = Ins_End;
		}
	}
	else
		func_count[0] = 0;

	if((last_stage == Ins_End) && (now_std > std_mean) && (flow_derivative < -50)){
		if(func_count[1]++ >= 3){
			func_count[1] = 0;
			*pnow_stage = Ex_Start;
		}
	}
	else
		func_count[1] = 0;

	if((last_stage == Ex_Start) && (now_std < std_mean)){
		if(func_count[2]++ >= 10){
			func_count[2] = 0;
			*pnow_stage = Ex_End;
		}
	}
	else
		func_count[2] = 0;

	if(last_stage == Ex_End && *pnow_stage == Ins_Start)
		*pexend_flow = pflow_buff[MAXFLOWBUFF_COUNT-6];

	if(last_stage == *pnow_stage && *pnow_stage != None){
		if(err_count++ >= 150){
			err_count = 0;
			*pnow_stage = None;
		}
	}
	else
		err_count = 0;
}

/*!
	@brief 		 Process the every stage of breathing.
	@param[in]	 set_press:Current pressure.
				 now_stage:Breathe stage.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
uint16_t Mid_Process_BreatheStage(unsigned short set_press,eBreathe_Stage now_stage){
	static uint8_t ins_count = 0;
	static uint8_t ex_count = 0;
	static uint16_t epr_press = 0;
	static FlagStatus flag_inscount = RESET;
	uint16_t return_press = 0;
	if(now_stage == None){
		return_press = set_press;
	}	
	else if(now_stage == Ins_Start || now_stage == Ins_End){
		if(flag_inscount == RESET){
			ins_count = ins_count>=100 ? 100 : ins_count+1;
			if(ins_count == 100) flag_inscount = SET;
		}
		else if(flag_inscount == SET){
			ins_count = ins_count<=1 ? 1 : ins_count-1; 	
		}
		return_press = set_press + (ins_count*2);
	}
	else if(now_stage == Ex_Start){
		epr_press = Mid_EPR_Function(set_press);
		return_press = epr_press;
	}
	else if(now_stage == Ex_End){
		return_press = epr_press + (1 * (++ex_count));
		if(return_press >= set_press){
			return_press = set_press;
			ex_count--;
		}
	}

	/* Clear the parameters. */
	if(now_stage != Ins_Start && now_stage != Ins_End){
		ins_count = 0;
		flag_inscount = RESET;
	}
	if(now_stage != Ex_End){
		ex_count = 0;
	}
	return return_press;
}


