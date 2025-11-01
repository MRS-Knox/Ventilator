#include "Application_ControlBlower.h"

/*!
	@brief 		 Control blower.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
unsigned char controlblower_time = 0;
void App_ControlBlower_Task(void *pvParameter){
	int measure_press = 0;
	uint8_t time_count = 0;
	Blower_State_t blower_data;
	Run_Param_t run_param;
	EventBits_t machine_event = 0x00;
	eBlowerRunStage blowerstage = Blower_Stop;
	while(1){
		controlblower_time = controlblower_time>200 ? 0 : controlblower_time+1;
		machine_event = xEventGroupWaitBits(MachineStateEvent_Handle,Machine_On_Event,pdFALSE,pdFALSE,0);

		if(blowerstage == Blower_Stop){
			/* Open the blower when only one of the machine,the mask detection,and the calibration is on. */
			if(((machine_event&Machine_On_Event) == Machine_On_Event) && ((machine_event&TestMask_Start_Event) != TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) != CalibrateStartBlower_Event))
				blowerstage = Blower_Start;
			else if(((machine_event&Machine_On_Event) != Machine_On_Event) && ((machine_event&TestMask_Start_Event) == TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) != CalibrateStartBlower_Event))
				blowerstage = Blower_Start;
			else if(((machine_event&Machine_On_Event) != Machine_On_Event) && ((machine_event&TestMask_Start_Event) != TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) == CalibrateStartBlower_Event))
				blowerstage = Blower_Start;
		}
		else if(blowerstage != Blower_Stop){
			/* Close the blower when the machine,the mask detection,and the calibration are all off. */
			if(((machine_event&Machine_On_Event) != Machine_On_Event) && ((machine_event&TestMask_Start_Event) != TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) != CalibrateStartBlower_Event))
				blowerstage = Blower_Stop;
			/* Close the blower when any two or all of the machine,the mask detection,and the calibration are on. */
			else if(((machine_event&Machine_On_Event) == Machine_On_Event) && ((machine_event&TestMask_Start_Event) == TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) != CalibrateStartBlower_Event))
				blowerstage = Blower_Stop;
			else if(((machine_event&Machine_On_Event) == Machine_On_Event) && ((machine_event&TestMask_Start_Event) != TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) == CalibrateStartBlower_Event))
				blowerstage = Blower_Stop;
			else if(((machine_event&Machine_On_Event) != Machine_On_Event) && ((machine_event&TestMask_Start_Event) == TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) == CalibrateStartBlower_Event))
				blowerstage = Blower_Stop;
			else if(((machine_event&Machine_On_Event) == Machine_On_Event) && ((machine_event&TestMask_Start_Event) == TestMask_Start_Event)
				&& ((machine_event&CalibrateStartBlower_Event) == CalibrateStartBlower_Event))
				blowerstage = Blower_Stop;
		}
	
		measure_press = Mid_CalculatePRESS();

		switch(blowerstage){
			case Blower_Stop:
				Mid_SendToBlower(SEND_RPM,0);
				break;
			case Blower_Start:
				Mid_SendToBlower(SEND_RPM,START_RPM);
				xQueueReceive(BlowerStateQueue_Handle,&blower_data,0);
				if(blower_data.actual_rpm >= MOTOR_SPEED_MIN){
					if(time_count++ >= 10){		//Delay 100ms.
						time_count = 0;
						if((machine_event&CalibrateStartBlower_Event) != CalibrateStartBlower_Event)
							blowerstage = Blower_PIDRPM;
						else
							blowerstage = Blower_ConstantRPM;
					}
				}
				else
					time_count = 0;
				break;
			case Blower_ConstantRPM:
				xQueueReceive(BlowerStateQueue_Handle,&blower_data,0);
				Mid_SendToBlower(SEND_RPM,blower_data.set_rpm);
				break;
			case Blower_PIDRPM:
				xQueueReceive(RunParamQueue_Handle,&run_param,0);
				Mid_IncreasePID(run_param.now_run_p,(uint16_t)measure_press);
				break;
			default:break;
		}

		if(blowerstage == Blower_PIDRPM)
			xEventGroupSetBits(MachineStateEvent_Handle,MachineOn_PID_Event);
		else
			xEventGroupClearBits(MachineStateEvent_Handle,MachineOn_PID_Event);

		vTaskDelay(pdMS_TO_TICKS(10));
	}
}




