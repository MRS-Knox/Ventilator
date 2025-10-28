#include "Middle_CalculateParameter.h"

/*!
	@brief 		 Update some monitor parameters.
	@param[in]	 now_stage:....
	@param[out]	 Run_Param:ins_time,ex_time,ins_ex_scale[2],bpm,leak,vt,mv.
	@retval		 none
	@note		 Running in the timer interrupt.
*/
void Mid_Update_MonitorPARAM(eBreathe_Stage now_stage){
	static eBreathe_Stage last_stage = None;
	static long int leak_sum = 0;
	static long int ins_vt_sum = 0;
	static long int ex_vt_sum = 0;
	float f_data = 0.0f;
	uint8_t base_time = 10;	//10ms

	switch(now_stage){
		case None:
			Run_Param.ins_time = 0;
			Run_Param.ex_time = 0;
			leak_sum = 0;
			ins_vt_sum = 0;
			ex_vt_sum = 0;
			break;
		case Error:
			Run_Param.ins_time = 0;
			Run_Param.ex_time = 0;
			leak_sum = 0;
			ins_vt_sum = 0;
			ex_vt_sum = 0;
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
				/* Calculate leakage. */
				Run_Param.leak_mean = leak_sum / ((Run_Param.ins_time+Run_Param.ex_time)/base_time);
				/* Calculate the tital volume about inspiration. */
				ins_vt_sum = ins_vt_sum - (Run_Param.leak_mean*(Run_Param.ins_time/base_time));
				f_data = ins_vt_sum / 100.0f;	//100L/min -> 1L/min
				f_data = f_data / 60.0f;		//L/min -> ml/ms
				Run_Param.ins_vt = (uint32_t)(f_data*(float)base_time*100.0f);//ml/ms -> 100ml  The base time is 10ms.
				/* Calculate the tital volume about expiration. */
				ex_vt_sum = ex_vt_sum - (Run_Param.leak_mean*(Run_Param.ex_time/base_time));
				ex_vt_sum = ex_vt_sum<0 ? -ex_vt_sum : ex_vt_sum;
				f_data = ex_vt_sum / 100.0f;	//100L/min -> 1L/min
				f_data = f_data / 60.0f;		//L/min -> ml/ms
				Run_Param.ex_vt = (uint32_t)(f_data*(float)base_time*100.0f);//ml/ms -> 100ml  The base time is 10ms.				
				/* Calculate minute volume. */
				Run_Param.mv = (uint32_t)(((float)Run_Param.ins_vt/100000.0f) * ((float)Run_Param.bpm/10.0f) * 100.0f);
				/* Clear parameters. */
				Run_Param.ins_time = 0;
				Run_Param.ex_time = 0;
				leak_sum = 0;
				ins_vt_sum = 0;
				ex_vt_sum = 0;
			}
			Run_Param.ins_time += base_time;
			leak_sum += Run_Param.flow_data;
			ins_vt_sum += Run_Param.flow_data;
			break;
		case Ins_End:
			Run_Param.ins_time += base_time;
			leak_sum += Run_Param.flow_data;
			ins_vt_sum += Run_Param.flow_data;
			break;
		case Ex_Start:
			Run_Param.ex_time += base_time;
			leak_sum += Run_Param.flow_data;
			ex_vt_sum += Run_Param.flow_data;
			break;
		case Ex_End:
			Run_Param.ex_time += base_time;
			leak_sum += Run_Param.flow_data;
			ex_vt_sum += Run_Param.flow_data;
			break;
		default:break;
	}
	last_stage = now_stage;
}




