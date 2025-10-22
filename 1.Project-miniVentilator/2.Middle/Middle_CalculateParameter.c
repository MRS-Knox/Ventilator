#include "Middle_CalculateParameter.h"


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
// int Mid_Update_FlowBaseLine(int *pflowbuff,uint16_t buffcount){
//     int return_baseline = 0;
//     int flow_sum = 0;
//     for(uint16_t i = 0;i < buffcount;i++)
//         flow_sum += pflowbuff[i];
//     return_baseline = (int)(flow_sum/300);
//     return return_baseline;
// }


