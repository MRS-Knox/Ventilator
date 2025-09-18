#include "Middle_Calibration.h"

/*!
	@brief 		 Calibrate flow and pressure.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
// void Mid_CalFlow_Pressure(void *pvParameter){
// 	Calibration_t calibration;
// 	Blower_State_t blower_t;
// 	uint16_t rpm = MOTOR_SPEED_MIN;
// 	while(1){
// 		xQueueReceive(CALDataQueue_Handle,&calibration,portMAX_DELAY);
// 		if(calibration.calibrate_start == SET && calibration.calibrate_stop == RESET){
// 			if(calibration.flag_recordflow == SET && calibration.flag_recordpress == RESET)
// 				Mid_CalibrateFlow();
// 			else if(calibration.flag_recordpress == SET && calibration.flag_recordflow == RESET){
// 				xQueueReceive(BlowerStateQueue_Handle,&blower_t,portMAX_DELAY);
// 				Mid_CalibratePRESS(blower_t.actual_rpm);
// 			}
// 			if(calibration.flag_uprpm == SET && calibration.flag_rerpm == RESET){
// 				rpm = rpm >= MOTOR_SPEED_MAX ? MOTOR_SPEED_MAX : rpm+calibration.inrpm_interval;
// 				Mid_SendToBlower(SEND_RPM,rpm);
// 			}
// 			else if(calibration.flag_rerpm == SET && calibration.flag_uprpm == RESET){
// 				rpm = rpm <= MOTOR_SPEED_MIN ? MOTOR_SPEED_MIN : rpm-calibration.rerpm_interval;
// 				Mid_SendToBlower(SEND_RPM,rpm);
// 			}
// 		}
// 		else if(calibration.calibrate_stop == SET && calibration.calibrate_start == RESET){
// 			rpm = MOTOR_SPEED_MIN;
// 			Mid_SendToBlower(SEND_RPM,0);
// 			/* Save data to flash. */
// 			xEventGroupSetBits(MachineStateEvent_Handle,CalibrateEnd_Event);
// 		}
// 	}
// }

/*!
	@brief 		 Debug function.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
// int flag_flow = 0;
// int flag_press = 0;
// int flag_stop = 0;
// int flag_4 = 0;
// void Mid_CalFlow_Pressure_Debug_Function(void *pvParameter){
// 	Calibration_t calibration;
// 	calibration.calibrate_start= SET;
// 	calibration.calibrate_stop = RESET;
// 	calibration.inrpm_interval = 20;
// 	calibration.rerpm_interval = 20;
// 	while(1){
// 		if(flag_flow == 1){
// 			calibration.flag_recordflow = SET;
// 			calibration.flag_recordpress = RESET;
// 		}
// 		if(flag_press == 1){
// 			calibration.flag_recordpress = SET;
// 			calibration.flag_recordflow = RESET;
// 		}
// 		if(flag_stop == 1){
// 			calibration.calibrate_start = RESET;
// 			calibration.calibrate_stop = SET;
// 		}
// 		if(flag_4 == 1){
// 			calibration.flag_uprpm = SET;
// 			calibration.flag_rerpm = RESET;
// 		}
// 		else if(flag_4 == 2){
// 			calibration.flag_rerpm = SET;
// 			calibration.flag_uprpm = RESET;
// 		}
// 		flag_flow = 0;
// 		flag_press = 0;
// 		flag_stop = 0;
// 		//flag_4 = 0;
// 		xQueueOverwrite(CALDataQueue_Handle,&calibration);
// 		calibration.flag_recordflow = RESET;
// 		calibration.flag_recordpress = RESET;
// 		calibration.calibrate_stop = RESET;
// 		calibration.flag_uprpm = RESET;
// 		calibration.flag_rerpm = RESET;
// 		calibration.calibrate_start = SET;
// 		vTaskDelay(pdMS_TO_TICKS(500));
// 	}
// }

