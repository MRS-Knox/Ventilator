#include "Middle_BreatheStage.h"

void Mid_Judge_BreatheStage_New(int *pflow_buff,int mean,int *pmean5_buff,eBreathe_Stage *pstage){
	static FlagStatus flag_node_ins = RESET;
	static FlagStatus flag_node_ex 	= RESET;
	static int last_mean = 0;
	static uint8_t ins_count1 = 0;
	static uint8_t ins_count2 = 0;
	static uint8_t ex_count1 = 0;
	static uint8_t ex_count2 = 0;
//	static eBreathe_Stage last_stage = None;
	float k = (pmean5_buff[4] - pmean5_buff[0]) / 5.0f;
	/* Machine is just starting. */
	if(*pstage == None){
		flag_node_ins = RESET;
		flag_node_ex = RESET;
		ins_count1 = 0;
		ins_count2 = 0;
		ex_count1 = 0;
		ex_count2 = 0;
		last_mean = mean;
		*pstage = Error;
		return;
	}
	/* Calculate the nodes. */
	if(last_mean >= pmean5_buff[3] && mean < pmean5_buff[4]){
		flag_node_ins = SET;
		ex_count1 = 0;
		ex_count2 = 0;
		flag_node_ex = RESET;
	}
	else if(last_mean <= pmean5_buff[3] && mean > pmean5_buff[4]){
		flag_node_ex = SET;
		ins_count1 = 0;
		ins_count2 = 0;
		flag_node_ins = RESET;
	}
	/* Calculate the phase about breathe. */
	if(flag_node_ins == SET && mean < pmean5_buff[4]){
		if(k > 18.0f){
			ins_count1++;
		}
		else
			ins_count1 = ins_count1<=1 ? 0 : ins_count1-1;
		if(ins_count1 >= 20){
			ins_count1 = 0;
			flag_node_ins = RESET;
			*pstage = Ins_Start;
		}
	}
	if(flag_node_ex == SET && mean > pmean5_buff[4]){
		if(k < -10.0f){
			if(ex_count1++ >= 5){
				ex_count1 = 0;
				flag_node_ex = RESET;
				*pstage = Ex_Start;
			}
		}
		else
			ex_count1 = 0;
	}

	if(*pstage == Ins_Start && k < 3.0f && pmean5_buff[4] > mean){
		if(ins_count2++ >= 3){
			*pstage = Ins_End;
		}
	}
	else
		ins_count2 = 0;
	if(*pstage == Ex_Start && k > 0 && pmean5_buff[4] < mean){
		if(ex_count2++ >= 3){
			*pstage = Ex_End;
		}
	}
	else
		ex_count2 = 0;
	last_mean = mean;
	/* Error! */
	if(flag_node_ex == SET && mean < pmean5_buff[4]){
		ex_count1 = 0;
		ex_count2 = 0;
		flag_node_ex = RESET;
	}
	if(flag_node_ins == SET && mean > pmean5_buff[4]){
		ins_count1 = 0;
		ins_count2 = 0;
		flag_node_ins = RESET;
	}
}




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
void Mid_Judge_BreatheStage(int *pflow_buff,int *pexend_flow,int *pstd_buff,eBreathe_Stage *pnow_stage){
	static uint8_t func_count[4] = {0,0,0,0};
	static uint8_t err_count = 0;
	int now_std,std_mean;
	eBreathe_Stage last_stage = *pnow_stage;
	int now_flow = pflow_buff[MAXFLOWBUFF_COUNT-1];
	int last_flow = pflow_buff[MAXFLOWBUFF_COUNT-2];
	long int sum = 0;
	int threshold_uplimit 	= *pexend_flow + 200;
//	int threshold_downlimit = *pexend_flow - 200;
	int flow_derivative = now_flow-last_flow;

	now_std = Calculate_STD(&pflow_buff[MAXFLOWBUFF_COUNT-STD_LENGENTH],STD_LENGENTH);
	MoveRight_Range(pstd_buff,MAXFLOWBUFF_COUNT-STD_LENGENTH,now_std);
	for(uint16_t i = 0;i < MAXFLOWBUFF_COUNT-STD_LENGENTH;i++)
		sum += pstd_buff[i];
	std_mean = sum / (MAXFLOWBUFF_COUNT-STD_LENGENTH);


	if((now_flow > threshold_uplimit) && ((flow_derivative > 300) || (now_std > std_mean && flow_derivative > 50)) && 
	(last_stage == None || last_stage == Ex_End)){
		if(func_count[0]++ >= 3){
			func_count[0] = 0;
			*pnow_stage = Ins_Start;
		}
	}
	else
		func_count[0] = func_count[0] <= 1 ? 0 : func_count[0]-1;

	if((now_flow > threshold_uplimit) && (last_stage == Ins_Start) && (now_std < std_mean || flow_derivative < 100)){
		if(func_count[1]++ >= 5){
			func_count[1] = 0;
			*pnow_stage = Ins_End;
		}
	}
	else
		func_count[1] = func_count[1] <= 1 ? 0 : func_count[1]-1;

	if((last_stage == Ins_End || last_stage == None) && (now_std > std_mean) && (flow_derivative < -50)){
		if(func_count[2]++ >= 3){
			func_count[2] = 0;
			*pnow_stage = Ex_Start;
		}
	}
	else
		func_count[2] = func_count[2] <= 1 ? 0 : func_count[2]-1;

	if((last_stage == Ex_Start) && (now_std < std_mean)){
		if(func_count[3]++ >= 5){ //10
			func_count[3] = 0;
			*pnow_stage = Ex_End;
		}
	}
	else
		func_count[3] = func_count[3] <= 1 ? 0 : func_count[3]-1;

	if(last_stage == Ex_End && *pnow_stage == Ins_Start){
		if(pflow_buff[MAXFLOWBUFF_COUNT-11]-*pexend_flow < 3000)
			*pexend_flow = pflow_buff[MAXFLOWBUFF_COUNT-11];
	}

	if(last_stage == *pnow_stage && *pnow_stage != None){
		if(err_count++ >= 150){
			err_count = 0;
			*pnow_stage = None;
		}
	}
	else
		err_count = 0;
}


