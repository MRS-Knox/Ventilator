#include "Middle_BreatheStage.h"

#define PHASE_INITIAL_LIMIT		280		//8lpm
#define INS_START_K				50		//0.5lpm
#define EX_START_K				-40		//-0.4lpm
#define K_CONTINUOUS_DATA		5

/*!
	@brief 		 Judge the breathing stage.
	@param[in]	 pflow_value:Point to the flow value buff.
	             count:The number of flow value.
				 now_stage:Breathing stage right now.
	@param[out]	 none
	@retval		 eBreathe_Stage:The breathing stage right now.
*/
eBreathe_Stage Mid_Judge_BreatheStage(int *pflow_value,uint16_t count,eBreathe_Stage now_stage){
	static uint16_t breathestage_tcount = 0;
	int max_value = 0;
	int min_value = 0;
	char ins_k_count = 0;
	char ex_k_count = 0;
	eBreathe_Stage return_stage = None;

	Find_MIN_MAX(pflow_value,count,&max_value,&min_value);	
	if(max_value - min_value > PHASE_INITIAL_LIMIT){
		for(unsigned char i = 0;i < count-1;i++){
			if(pflow_value[i+1] - pflow_value[i] > INS_START_K){
				ins_k_count++;
				ex_k_count = 0;
			}
			else if(pflow_value[i+1] - pflow_value[i] < EX_START_K){
				ins_k_count = 0;
				ex_k_count++;
			}
			else{
				ins_k_count = 0;
				ex_k_count = 0;
			}
		}
		if(ins_k_count >= K_CONTINUOUS_DATA && (now_stage == Ex_End || now_stage == None))
			return_stage = Ins_Start;
		if(ex_k_count >= K_CONTINUOUS_DATA && now_stage == Ins_End)
			return_stage = Ex_Start;
	}
	else{
		if(now_stage == Ins_Start)
			return_stage = Ins_End;
		else if(now_stage == Ex_Start)
			return_stage = Ex_End;
	} 
	/* ERROR!!! */
	if(now_stage == return_stage){
		if(breathestage_tcount++ >= 250){	//5s  5000/20
			breathestage_tcount = 0;
			return_stage = None;
		} 
	}
	else
		breathestage_tcount = 0;
	return return_stage;
}

/*!
	@brief 		 Inspiration stage process function.
	@param[in]	 set_press:Current pressure.
				 therapy_mode:Therapy mode.
				 ins_stage:Breathe stage.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
uint16_t Mid_Inspiration_Stage(unsigned short set_press,eBreathe_Stage ins_stage){
    uint16_t return_press = set_press;
	if(ins_stage == Ins_Start)
		return_press = set_press + (0.2 * set_press);
	else if(ins_stage == Ins_End)
		return_press = set_press;

	return return_press;
}

/*!
	@brief 		 Expiration stage process function.
	@param[in]	 set_press:Current pressure.
				 therapy_mode:Therapy mode.
				 ex_stage:Breathe stage.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
uint16_t Mid_Expiration_Stage(unsigned short set_press,eBreathe_Stage ex_stage){
    uint16_t return_press = set_press;
	if(ex_stage == Ex_Start)
		return_press = Mid_EPR_Function(set_press);
	else if(ex_stage == Ex_End)
		return_press = set_press;


	return return_press;
}

