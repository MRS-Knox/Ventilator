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
void Mid_Judge_BreatheStage(int *pflow_buff,int mean,int *pmean5_buff,eBreathe_Stage *pstage){
	static eBreathe_Stage last_stage = None;
	static FlagStatus flag_node_ins = RESET;
	static FlagStatus flag_node_ex 	= RESET;
	static int last_mean = 0;
	static uint8_t ins_count1 = 0;
	static uint8_t ins_count2 = 0;
	static uint8_t ex_count1 = 0;
	static uint8_t ex_count2 = 0;
	static uint8_t err_count = 0;
//	static eBreathe_Stage last_stage = None;
	float k = (pmean5_buff[4] - pmean5_buff[2]) / 5.0f; //Decrease k.
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
		if(k > 30.0f){
			ins_count1++;
		}
		else
			ins_count1 = ins_count1<=1 ? 0 : ins_count1-1;
		if(ins_count1 >= 5){
			ins_count1 = 0;
			flag_node_ins = RESET;
			*pstage = Ins_Start;
		}
	}
	if(flag_node_ex == SET && mean > pmean5_buff[4]){
		if(k < -10.0f){
			if(ex_count1++ >= 3){
				ex_count1 = 0;
				flag_node_ex = RESET;
				*pstage = Ex_Start;
			}
		}
		else
			ex_count1 = ex_count1<=1 ? 0 : ex_count1-1;
	}

	if(*pstage == Ins_Start && k < 3.0f && pmean5_buff[4] > mean){
		if(ins_count2++ >= 3){
			ins_count2 = 0;
			*pstage = Ins_End;
		}
	}
	else
		ins_count2 = 0;
	if(*pstage == Ex_Start && k > 0 && pmean5_buff[4] < mean){
		if(ex_count2++ >= 3){
			ex_count2 = 0;
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
		*pstage = Error;
	}
	if(flag_node_ins == SET && mean > pmean5_buff[4]){
		ins_count1 = 0;
		ins_count2 = 0;
		flag_node_ins = RESET;
		*pstage = Error;
	}
	if(*pstage == last_stage){
		if(err_count++ >= (5000/20)){	//5s
			err_count = 0;
			*pstage = Error;
		}	
	}
	else
		err_count = 0;

	last_stage = *pstage;
}



