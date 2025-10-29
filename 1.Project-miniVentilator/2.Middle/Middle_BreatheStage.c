#include "Middle_BreatheStage.h"

/*!
	@brief 		 Judge the breathe stage.
	@param[in]	 pflow_buff:Point to the flow.
				 mean:Flow mean.
				 pmean5_buff:....
				 pstage:.....
	@param[out]	 pnow_stage:.....
	@retval		 none
*/
void Mid_Judge_BreatheStage(int *pflow_buff,int mean,eBreathe_Stage *pstage){
	static eBreathe_Stage last_stage = None;
	static FlagStatus flag_node_ins = RESET;
	static FlagStatus flag_node_ex 	= RESET;
	static int last_mean = 0;
	static uint8_t ins_count1 = 0;
	static uint8_t ins_count2 = 0;
	static uint8_t ex_count1 = 0;
	static uint8_t ex_count2 = 0;
	static uint16_t err_count = 0;
	int now_flow = pflow_buff[MAXFLOWBUFF_COUNT-1];
	int last_flow = pflow_buff[MAXFLOWBUFF_COUNT-2];
	Run_Param.std = Calculate_STD(&pflow_buff[MAXFLOWBUFF_COUNT-10],10);
	int k = now_flow - last_flow;
	if(-20 < k && k < 20){	//Flow is stabilization.
		ex_count1 = 0;
		ins_count1 = 0;
	}	
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
	if(last_mean >= last_flow && mean < now_flow){
		flag_node_ins = SET;
		ex_count1 = 0;
		ex_count2 = 0;
		flag_node_ex = RESET;
	}
	else if(last_mean <= last_flow && mean > now_flow){
		flag_node_ex = SET;
		ins_count1 = 0;
		ins_count2 = 0;
		flag_node_ins = RESET;
	}
	/* Calculate the phase about breathe. */
	if(flag_node_ins == SET && mean < now_flow ){
		if(Run_Param.std > 100 && k > 20){
			if(ins_count1++ >= 5){
				ins_count1 = 0;
				flag_node_ins = RESET;
				*pstage = Ins_Start;
			}
		}
		else
			ins_count1 = ins_count1<=1 ? 0 : ins_count1-1;
	}
	if(flag_node_ex == SET && mean > now_flow){
		if(Run_Param.std > 70 && k < -20){
			if(ex_count1++ >= 3){
				ex_count1 = 0;
				flag_node_ex = RESET;
				*pstage = Ex_Start;
			}
		}
		else
			ex_count1 = ex_count1<=1 ? 0 : ex_count1-1;
	}

	if(*pstage == Ins_Start && (Run_Param.std < 70 || k < 0) && now_flow > mean){
		if(ins_count2++ >= 3){
			ins_count2 = 0;
			*pstage = Ins_End;
		}
	}
	else
		ins_count2 = 0;
	if(*pstage == Ex_Start && (Run_Param.std < 60 || k > 0) && now_flow < mean){
		if(ex_count2++ >= 3){
			ex_count2 = 0;
			*pstage = Ex_End;
		}
	}
	else
		ex_count2 = 0;
	/* Error! */
	if(flag_node_ex == SET && mean < now_flow){
		ex_count1 = 0;
		ex_count2 = 0;
		flag_node_ex = RESET;
		*pstage = Error;
	}
	if(flag_node_ins == SET && mean > now_flow){
		ins_count1 = 0;
		ins_count2 = 0;
		flag_node_ins = RESET;
		*pstage = Error;
	}
	if(*pstage == last_stage){
		if(err_count++ >= (10000/20)){	//10s
			err_count = 0;
			*pstage = Error;
		}	
	}
	else
		err_count = 0;

	last_stage = *pstage;
	last_mean = mean;
}



