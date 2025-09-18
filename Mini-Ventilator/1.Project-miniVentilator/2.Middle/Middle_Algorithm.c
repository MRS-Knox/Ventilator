#include "Middle_Algorithm.h"

#define MAXFLOWBUFF_COUNT		20
#define LIMIT_SWITCHPHASE		800		//8lpm
#define INS_START_K				50		//0.5lpm
#define EX_START_K				-40		//-0.4lpm
#define K_CONTINUOUS_DATA		5

/*!
	@brief 		 Judge inspiration and expiration.
	@param[in]	 pflow_value:Point to the flow value buff.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
void Mid_JudgeInsEx(void *pvParameter){
	int flow_buff[MAXFLOWBUFF_COUNT];
	int max_flowvalue = 0;
	int min_flowvalue = 0;
	uint8_t ins_k_count = 0;
	uint8_t ex_k_count = 0;
	eBreathe_Stage last_breathestage = None;
	uint16_t breathe_count = 0;
	while(1){
		xEventGroupWaitBits(MachineStateEvent_Handle,CaculateFlowEnd_Event,pdTRUE,pdTRUE,portMAX_DELAY);
		
		MoveRight_Range(flow_buff,MAXFLOWBUFF_COUNT,Run_Param.flow_data);	
		Find_MIN_MAX(flow_buff,MAXFLOWBUFF_COUNT,&max_flowvalue,&min_flowvalue);
		
		/* Judge breathe initial phase. */
		if(max_flowvalue - min_flowvalue > LIMIT_SWITCHPHASE){
			if(Run_Param.flow_data > Run_Param.flow_baseline){
				for(unsigned char i = 0;i < MAXFLOWBUFF_COUNT-1;i++){
					if(flow_buff[i+1] - flow_buff[i] > INS_START_K){
						ins_k_count++;
						ex_k_count = 0;
					}
					else{
						ins_k_count = 0;
						ex_k_count = 0;
					}
				}
			}
			else if(Run_Param.flow_data < Run_Param.flow_baseline){
				for(unsigned char i = 0;i < MAXFLOWBUFF_COUNT-1;i++){
					if(flow_buff[i+1] - flow_buff[i] < EX_START_K){
						ins_k_count = 0;
						ex_k_count++;
					}
					else{
						ins_k_count = 0;
						ex_k_count = 0;
					}
				}
			}

			if(ins_k_count >= K_CONTINUOUS_DATA && (Run_Param.breathe_stage == Ex_End || Run_Param.breathe_stage == None))
				Run_Param.breathe_stage = Ins_Start;
			if(ex_k_count >= K_CONTINUOUS_DATA && Run_Param.breathe_stage == Ins_End)
				Run_Param.breathe_stage = Ex_Start;
		}
		/* Judge breathe end phase. */
		else if(max_flowvalue - min_flowvalue <= LIMIT_SWITCHPHASE){
			if(Run_Param.breathe_stage == Ins_Start && Run_Param.flow_data > Run_Param.flow_baseline)
				Run_Param.breathe_stage = Ins_End;
			else if(Run_Param.breathe_stage == Ex_Start && Run_Param.flow_data < Run_Param.flow_baseline)
				Run_Param.breathe_stage = Ex_End;
		}
		
		/* !!!ERROR ---- Inspiration over time or Expiration over time. */
		if(Run_Param.breathe_stage != last_breathestage){
			breathe_count = 0;
			last_breathestage = Run_Param.breathe_stage;
		}
		else if(Run_Param.breathe_stage == last_breathestage){
			if(breathe_count++ >= 250){ 	//5s (250*20ms)
				breathe_count = 0;
				Run_Param.breathe_stage = None;
			}
		}
	}
}

/*!
	@brief 		 Inspiration stage process function.
	@param[in]	 set_press:Current pressure.
				 therapy_mode:Therapy mode.
				 ins_stage:Breathe stage.
	@param[out]	 none
	@retval		 return_press:Running pressure.
*/
uint16_t Mid_Inspiration_Stage(short set_press,eTherapy_Mode therapy_mode,eBreathe_Stage ins_stage){
    uint16_t return_press = set_press;
	if(therapy_mode == CPAP || therapy_mode == AutoCPAP){
		if(ins_stage == Ins_Start)
			return_press = set_press + (0.2 * set_press);
		else if(ins_stage == Ins_End)
			return_press = set_press;
	}

	// Run_Param.ins_flow += Run_Param.flow_data;
	// if(Run_Param.breathe_stage == Ins_Start){
	// 	Run_Param.breath_totalflow = Run_Param.ins_flow + Run_Param.ex_flow;
	// Run_Param.flow_baseline = Run_Param.breath_totalflow;
	// }
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
uint16_t Mid_Expiration_Stage(short set_press,eTherapy_Mode therapy_mode,eBreathe_Stage ex_stage){
    uint16_t return_press = set_press;
	if(therapy_mode == CPAP || therapy_mode == AutoCPAP){
		if(ex_stage == Ex_Start)
			return_press = Mid_EPR_Function(set_press);
		else if(ex_stage == Ex_End)
			return_press = set_press;
	}

	// Run_Param.ex_flow += Run_Param.flow_data;
	return return_press;
}





