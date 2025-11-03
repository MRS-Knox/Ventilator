#include "Application_MaskDetection.h"

#define MASKDETECTION_P			1000
#define MAX_MASKFLOWMEAN		5000
#define MIN_MASKFLOWMEAN		3000

/*!
	@brief 		 Mask detection.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void App_MaskDetection_Task(void *pvPatameter){
	EventBits_t mask_event = 0x0;
	eTestMask_Stage testmask_stge = Test_Stop;
	FlagStatus flag_mask_start = SET;
	int stdflow_buff[10];
	int flow_buff[MAXFLOWBUFF_COUNT];
	int flow_buff1[2*MAXFLOWBUFF_COUNT];
	uint16_t flow_buff_count = 0;
	uint16_t flow_buff1_count = 0;
	int flow1_sum = 0;

	while(1){
		mask_event = xEventGroupWaitBits(MachineStateEvent_Handle,TestMask_Start_Event,pdFALSE,pdFALSE,0);

		if(testmask_stge == Test_Stop && (mask_event&TestMask_Start_Event) == TestMask_Start_Event){
			testmask_stge = Test_Start;
			xEventGroupClearBits(MachineStateEvent_Handle,Machine_On_Event|CalibrateStartBlower_Event);
		}
		else if(testmask_stge != Test_Stop && (mask_event&TestMask_Start_Event) != TestMask_Start_Event)
			testmask_stge = Test_Stop;

		switch(testmask_stge){
			case Test_Stop:
				Run_Param.mask_leak = 0;
				flow1_sum = 0;
				flow_buff1_count = 0;
				flow_buff_count = 0;
				flag_mask_start = SET;
				Run_Param.testmask_set_p 	= 200;
				Run_Param.now_run_p 		= 0;
				Run_Param.flow_sum 			= 0;
				Run_Param.flow_mean 		= 0;
				Run_Param.breathe_stage 	= None;
				Run_Param.breathe_count		= 0;
				Machine_State.flag_testmask_onoff = RESET;
				Machine_State.flag_ajustmask = RESET;
				xEventGroupWaitBits(MachineStateEvent_Handle,TestMask_Start_Event,pdFALSE,pdFALSE,portMAX_DELAY);
				break;
			case Test_Start:
				Run_Param.testmask_set_p = 200;
				Run_Param.now_run_p = Run_Param.testmask_set_p;
				Machine_State.flag_testmask_onoff = SET;
				if((mask_event&MachineOn_PID_Event) == MachineOn_PID_Event)
					testmask_stge = Test_Run;
				break;
			case Test_Run:
				if(flag_mask_start == SET)
					break;
				if(Run_Param.testmask_set_p++ >= MASKDETECTION_P){
					Run_Param.testmask_set_p = MASKDETECTION_P;
					testmask_stge = Test_Run;
				}
				if(Run_Param.mask_leak > MAX_MASKFLOWMEAN && Machine_State.flag_ajustmask == RESET)
					Machine_State.flag_ajustmask = SET;
				else if(Run_Param.mask_leak < MIN_MASKFLOWMEAN && Machine_State.flag_ajustmask == SET)
					Machine_State.flag_ajustmask = RESET;
				break;
			default:break;
		}

		/* Judge the breathing stage and update the control pressure. */
		if(testmask_stge == Test_Run){
			if(flag_mask_start == SET){
				flow_buff[flow_buff_count++] = Run_Param.flow_data;
				Run_Param.flow_sum += Run_Param.flow_data;
				if(flow_buff_count >= MAXFLOWBUFF_COUNT){
					flag_mask_start = RESET;
					for(uint8_t i = 0;i < 10;i++)
						stdflow_buff[i] = flow_buff[MAXFLOWBUFF_COUNT-10+i];
				}
			}
			else{
				Run_Param.flow_sum = (Run_Param.flow_sum-flow_buff[flow_buff_count-MAXFLOWBUFF_COUNT])+Run_Param.flow_data;
				Run_Param.flow_mean = Run_Param.flow_sum / MAXFLOWBUFF_COUNT;
				MoveRight_Range(stdflow_buff,10,Run_Param.flow_data);
				/* Rotate right. */
				flow_buff[flow_buff_count-MAXFLOWBUFF_COUNT] = Run_Param.flow_data;
				if((++flow_buff_count) >= (2*MAXFLOWBUFF_COUNT))
					flow_buff_count = MAXFLOWBUFF_COUNT;
				
				Mid_Judge_BreatheStage(stdflow_buff,Run_Param.flow_mean,&Run_Param.breathe_stage);
				Mid_EPR(&Run_Param.now_run_p,Run_Param.testmask_set_p,Run_Param.breathe_stage,Run_Param.breathe_count,RESET);
			}
			if(flow_buff1_count < 2*MAXFLOWBUFF_COUNT){
				flow_buff1[flow_buff1_count++] = Run_Param.flow_data;
				flow1_sum += Run_Param.flow_data;
			}
			else{
				Run_Param.mask_leak = flow1_sum/(2*MAXFLOWBUFF_COUNT);
				flow1_sum = flow1_sum - flow_buff1[flow_buff1_count-(2*MAXFLOWBUFF_COUNT)] +  Run_Param.flow_data;
				/* Rotate right. */
				flow_buff1[flow_buff1_count-(2*MAXFLOWBUFF_COUNT)] = Run_Param.flow_data;
				flow_buff1_count++;
				flow_buff1_count = flow_buff1_count>=4*MAXFLOWBUFF_COUNT ? 2*MAXFLOWBUFF_COUNT : flow_buff1_count;
			}
		}

		if(testmask_stge == Test_Run)
			xQueueOverwrite(RunParamQueue_Handle,&Run_Param);
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}


