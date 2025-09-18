#include "Application_Calibration.h"

void Mid_CalibratePRESS(uint32_t rpm);
void Mid_CALCoefficient(void);

/*!
	@brief 		 Calibrate pressure and flow.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void App_Calibration_Task(void *pvParameter){
	uint8_t count = 0;
	uint16_t cali_set_rpm = MOTOR_SPEED_MIN;
	FlagStatus flag_calibrate_start = RESET;
	EventBits_t calibrate_event = 0x0;
	Calibration_t calibration_rec;
	Run_Param_t run_param;
	Blower_State_t blower_state;
	while(1){
		calibrate_event = xEventGroupWaitBits(MachineStateEvent_Handle,CalibrateStart_Event,pdFALSE,pdFALSE,0);
		/* ERROR! */
		if((calibrate_event&CalibrateStart_Event) == CalibrateStart_Event && (calibrate_event&CalibrateStop_Event) == CalibrateStop_Event){
			xEventGroupClearBits(MachineStateEvent_Handle,CalibrateStart_Event);
			calibrate_event = CalibrateStop_Event;
		}

		/* Start calibration. */
		if((calibrate_event&CalibrateStart_Event) == CalibrateStart_Event){
			flag_calibrate_start = SET;
			if(xQueueReceive(CALDataQueue_Handle,&calibration_rec,0) == pdPASS){
				/* Record difference pressure and rpm. */
				if(calibration_rec.flag_califlow == SET && calibration_rec.flag_calipress == RESET){
					xQueueReceive(RunParamQueue_Handle,&run_param,0);
					CalibrationData.calflow_buff[CalibrationData.calflow_count++] = run_param.diff_press;
					if(CalibrationData.calflow_count >= CALFlowMaxCount){
						CalibrationData.calflow_count = 0;
						Mid_CALCoefficient();
					}	
				}
				else if(calibration_rec.flag_calipress == SET && calibration_rec.flag_califlow == RESET){
					xQueueReceive(BlowerStateQueue_Handle,&blower_state,portMAX_DELAY);
					Mid_CalibratePRESS(blower_state.actual_rpm);
				}
				/* Set blower rpm. */
				if(calibration_rec.flag_uprpm == SET && calibration_rec.flag_rerpm == RESET && calibration_rec.flag_stopblower == RESET){
					cali_set_rpm = cali_set_rpm >= MOTOR_SPEED_MAX ? MOTOR_SPEED_MAX : cali_set_rpm+calibration_rec.inrpm_interval;
					Blower_State.set_rpm = cali_set_rpm;
					xQueueOverwrite(BlowerStateQueue_Handle,&Blower_State);
					xEventGroupSetBits(MachineStateEvent_Handle,CalibrateStartBlower_Event);
					xEventGroupClearBits(MachineStateEvent_Handle,CalibrateStopBlower_Event);
				}
				else if(calibration_rec.flag_rerpm == SET && calibration_rec.flag_uprpm == RESET && calibration_rec.flag_stopblower == RESET){
					cali_set_rpm = cali_set_rpm <= MOTOR_SPEED_MIN ? MOTOR_SPEED_MIN : cali_set_rpm-calibration_rec.rerpm_interval;
					Blower_State.set_rpm = cali_set_rpm;
					xQueueOverwrite(BlowerStateQueue_Handle,&Blower_State);
				}
				else if(calibration_rec.flag_stopblower == SET && calibration_rec.flag_rerpm == RESET && calibration_rec.flag_uprpm == RESET){					
					cali_set_rpm = MOTOR_SPEED_MIN;
					Blower_State.set_rpm = 0;
					xQueueOverwrite(BlowerStateQueue_Handle,&Blower_State);
					xEventGroupSetBits(MachineStateEvent_Handle,CalibrateStopBlower_Event);
					xEventGroupClearBits(MachineStateEvent_Handle,CalibrateStartBlower_Event);
				}
				/* Real-time clock. */
				if(calibration_rec.flag_recordrtc == SET){	
				    Mid_Set_RTC(calibration_rec.rtc_time);
					CalibrationData.flag_rtc_init = 0x01;				
				}	
				/* Machine model. */
				if(calibration_rec.flag_recordmodel == SET){	
					CalibrationData.machine_model = (unsigned short)calibration_rec.machinemodel;
				}					
			}
		}
		/* Stop calibration. */
		if(((calibrate_event&CalibrateStop_Event) == CalibrateStop_Event) && (flag_calibrate_start == SET)){
			cali_set_rpm = MOTOR_SPEED_MIN;
			if(Mid_WriteCALData(&CalibrationData) == SUCCESS){
				flag_calibrate_start = RESET;
			}
		}
		/* Suspend task. */
		if(((calibrate_event&CalibrateStop_Event) == CalibrateStop_Event) && (flag_calibrate_start == RESET)){
			if(count++ >= 10){
				count = 0;
				xEventGroupWaitBits(MachineStateEvent_Handle,CalibrateStart_Event,pdFALSE,pdFALSE,portMAX_DELAY);
			}
		}
		else
			count = 0;
		
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

/*!
	@brief 		 Calibrate pressure.(pressure -> rpm)
	@param[in]	 rpm:....
	@param[out]	 none
	@retval		 none
*/
void Mid_CalibratePRESS(uint32_t rpm){
	static int press_compensation_buff[CALPRESSMaxCount];
	int sum_presscom = 0;
	int measure_press = Mid_CalculatePRESS();
	press_compensation_buff[CalibrationData.calpress_count] = ((CalibrationData.calpress_count+1)*500)-measure_press;
	CalibrationData.calpress_buff[CalibrationData.calpress_count++] = rpm;
	if(CalibrationData.calpress_count >= CALPRESSMaxCount){
		CalibrationData.calpress_count = 0;
		for(uint8_t i = 0;i < CALPRESSMaxCount;i++){
			sum_presscom += press_compensation_buff[i];
		}
		sum_presscom = sum_presscom/CALPRESSMaxCount;
		if(sum_presscom < 0)
			CalibrationData.press_compensation = (0x01<<15) | (-sum_presscom);
		else
			CalibrationData.press_compensation = (uint16_t)sum_presscom;
	}
}

/*!
	@brief 		 Calculate coefficient(k).
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_CALCoefficient(){
	uint16_t flow_buff[CALFlowMaxCount] = {0,4000,6000,8000,10000,12000,14000};
	float middle_data = 0.0f;
	uint8_t count = 0;
	
	/* [0,40) */
	middle_data = flow_buff[count+1]/sqrt(CalibrationData.calflow_buff[count+1]);
	CalibrationData.k[count] = (uint16_t)(middle_data*100);
	count++;	
	/* [40,60) */
	middle_data = ((flow_buff[count]/sqrt(CalibrationData.calflow_buff[count])) + (flow_buff[count+1]/sqrt(CalibrationData.calflow_buff[count+1])))/2;
	CalibrationData.k[count] = (uint16_t)(middle_data*100);
	count++;
	/* [60,80) */
	middle_data = ((flow_buff[count]/sqrt(CalibrationData.calflow_buff[count])) + (flow_buff[count+1]/sqrt(CalibrationData.calflow_buff[count+1])))/2;
	CalibrationData.k[count] = (uint16_t)(middle_data*100);
	count++;
	/* [80,100) */
	middle_data = ((flow_buff[count]/sqrt(CalibrationData.calflow_buff[count])) + (flow_buff[count+1]/sqrt(CalibrationData.calflow_buff[count+1])))/2;
	CalibrationData.k[count] = (uint16_t)(middle_data*100);
	count++;
	/* [100,120) */
	middle_data = ((flow_buff[count]/sqrt(CalibrationData.calflow_buff[count])) + (flow_buff[count+1]/sqrt(CalibrationData.calflow_buff[count+1])))/2;
	CalibrationData.k[count] = (uint16_t)(middle_data*100);
	count++;
	/* [120,140) */
	middle_data = ((flow_buff[count]/sqrt(CalibrationData.calflow_buff[count])) + (flow_buff[count+1]/sqrt(CalibrationData.calflow_buff[count+1])))/2;
	CalibrationData.k[count] = (uint16_t)(middle_data*100);
	count++;
}

/*!
	@brief 		 Debug function.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
int flag_flow = 0;
int flag_press = 0;
int flag_stopblo = 0;
int flag_stopcali = 0;
int flag_startcali = 0;
int flag_model = 0;
int flag_rtc = 0;
int flag_4 = 0;
void App_Cali_Debug_Function(void *pvParameter){
	Calibration_t calibration_send;
	
	calibration_send.inrpm_interval = 20;
	calibration_send.rerpm_interval = 20;
	
	calibration_send.flag_stopblower = RESET;
	calibration_send.flag_recordmodel = RESET;
	calibration_send.flag_recordrtc = RESET;
	while(1){
		if(flag_flow == 1){
			calibration_send.flag_califlow = SET;
			calibration_send.flag_calipress = RESET;
		}
		if(flag_press == 1){
			calibration_send.flag_calipress = SET;
			calibration_send.flag_califlow = RESET;
		}
		if(flag_startcali == 1){
			xEventGroupClearBits(MachineStateEvent_Handle,CalibrateStop_Event);
			xEventGroupSetBits(MachineStateEvent_Handle,CalibrateStart_Event);
		}
		if(flag_stopcali == 1){
			xEventGroupClearBits(MachineStateEvent_Handle,CalibrateStart_Event);
			xEventGroupSetBits(MachineStateEvent_Handle,CalibrateStop_Event);
		}
		if(flag_stopblo == 1){
			calibration_send.flag_stopblower = SET;
		}
		if(flag_4 == 1){
			calibration_send.flag_uprpm = SET;
			calibration_send.flag_rerpm = RESET;
		}
		else if(flag_4 == 2){
			calibration_send.flag_rerpm = SET;
			calibration_send.flag_uprpm = RESET;
		}
		if(flag_model == 1){
			calibration_send.machinemodel = Mini_1_P20;
			calibration_send.flag_recordmodel = SET;
		}
		if(flag_rtc == 1){
			calibration_send.rtc_time[0] = 25;
			calibration_send.rtc_time[1] = 9;
			calibration_send.rtc_time[2] = 10;
			calibration_send.rtc_time[3] = 3;
			calibration_send.rtc_time[4] = 9;		//Hour
			calibration_send.rtc_time[5] = 4;
			calibration_send.rtc_time[6] = 0;
			calibration_send.rtc_time[7] = 0;
			calibration_send.flag_recordrtc = SET;
		}
		flag_flow = 0;
		flag_press = 0;
		flag_startcali = 0;
		flag_stopcali = 0;
		flag_stopblo = 0;
		flag_model = 0;
		flag_rtc = 0;
		//flag_4 = 0;
		xQueueOverwrite(CALDataQueue_Handle,&calibration_send);
		calibration_send.flag_califlow = RESET;
		calibration_send.flag_calipress = RESET;
		calibration_send.flag_uprpm = RESET;
		calibration_send.flag_rerpm = RESET;
		calibration_send.flag_stopblower = RESET;
		calibration_send.flag_recordmodel = RESET;
		calibration_send.flag_recordrtc = RESET;
		
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}


