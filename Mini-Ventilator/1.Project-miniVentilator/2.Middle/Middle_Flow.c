#include "Middle_Flow.h"

float sensorbuff_sdp_nofilter;
float sensorbuff_sdp_filter;

/*!
	@brief 		 This initial function is about flow sensor hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_Flow_Init(){
	Dri_Flow_SCL_Init();
	Dri_Flow_SDA_State(0);
	Mid_SDP31_RESET();
}

/*!	
	@brief 		 Reset sensor. 
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_SDP31_RESET(){
	FLOW_Start();
	FLOW_Write_Byte(0x00);
	FLOW_Wait_Ack();
	FLOW_Write_Byte(0x06);
	FLOW_Wait_Ack();
	FLOW_Stop();
	udelay_100us(20);
}

/*!	
	@brief 		 SDP810 write cmd
	@param[in]	 data:need write cmd
	@param[out]	 none
	@retval		 none
*/
void Mid_SDP31_WriteCMD(uint16_t data){
	FLOW_Start();
	FLOW_Write_Byte(SDP31_WRITE_CMD);
	FLOW_Wait_Ack();
	FLOW_Write_Byte(data>>8);
	FLOW_Wait_Ack();
	FLOW_Write_Byte(data);
	FLOW_Wait_Ack();
	udelay_100us(10);
}

/*!	
	@brief 		 Read SDP810 data
	@param[in]	 scale_factor:....
				 sdp31_value:....
	@param[out]	 none
	@retval		 none
*/
void Mid_SDP31_ReadData(uint16_t *scale_factor,uint8_t *sdp31_value){
	FLOW_Start();
	FLOW_Write_Byte(SDP31_READ_CMD);
	FLOW_Wait_Ack();

	sdp31_value[0] = FLOW_Read_Byte();
	FLOW_Ack();
	sdp31_value[1] = FLOW_Read_Byte();
	FLOW_Ack();
	sdp31_value[2] = FLOW_Read_Byte();	//Read difference pressure.
	FLOW_Ack();
	
	sdp31_value[3] = FLOW_Read_Byte();
	FLOW_Ack();
	sdp31_value[4] = FLOW_Read_Byte();
	FLOW_Ack();	
	sdp31_value[5] = FLOW_Read_Byte();	//Read temperature	--  give up.
	FLOW_Ack();	
	
	sdp31_value[3] = FLOW_Read_Byte();
	FLOW_Ack();
	sdp31_value[4] = FLOW_Read_Byte();
	FLOW_Ack();	
	sdp31_value[5] = FLOW_Read_Byte();	//Read scale factor.
	FLOW_No_Ack();	
	FLOW_Stop();
	
	if(sdp31_value[5] == Check_SensorCRC8(&sdp31_value[3],2))	//Calculate CRC and judge CRC whether right.
		*scale_factor = (sdp31_value[3]<<8) + sdp31_value[4];	
}

/*!	
	@brief 		 Process SDP31 data and calculate difference pressure.
	@param[in]	 none
	@param[out]	 pdiffdata:....
	@retval		 ErrorStatus:CRC whether right
*/
ErrorStatus Mid_SDP31Data_Process(float *pdiffdata){
	uint16_t minus_value = 0;
	uint8_t sdp31_data[6];
	uint16_t scale = 0;

	Mid_SDP31_WriteCMD(DIFF_PRESS_CONTINUOUS);
	Mid_SDP31_ReadData(&scale,sdp31_data);    
	
	if(sdp31_data[2] == Check_SensorCRC8(sdp31_data,2)){	
		if(sdp31_data[0] & 0x80){	// < 0Pa
			minus_value = Calculate_DataComplement((sdp31_data[0]<<8) + sdp31_data[1]);
			*pdiffdata = -(float)(minus_value&0x7FFF)/scale;
		}
		else	
			*pdiffdata = (float)((sdp31_data[0]<<8) + sdp31_data[1])/scale;
		
		sensorbuff_sdp_nofilter = *pdiffdata;
		*pdiffdata = Kalman_Filter(&Flow_Kalman,*pdiffdata);
		sensorbuff_sdp_filter = *pdiffdata;
		
		*pdiffdata = -*pdiffdata;
		Run_Param.diff_press = *pdiffdata*100;
		return SUCCESS;		
	}
	return ERROR;
}

/*!
	@brief 		 This function is about to calculate flow.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
int Mid_CalculateFlow(void){
	float fdiff_press;
	uint32_t diff_p = 0;
	int measure_flow = 0;

	if(Mid_SDP31Data_Process(&fdiff_press) == SUCCESS){
		if(fdiff_press < 0.0f)
			diff_p = (-fdiff_press)*100;
		else
			diff_p = fdiff_press*100;

		// measure_flow = (int)(240 * sqrt(diff_p));
		/* [0,40) */
		if(diff_p <= CalibrationData.calflow_buff[1]){
			measure_flow = (int)(((float)CalibrationData.k[0]/100.0f) * sqrt(diff_p));
		} 
		/* [40,60) */
		else if(diff_p <= CalibrationData.calflow_buff[2]){
			measure_flow = (int)(((float)CalibrationData.k[1]/100.0f) * sqrt(diff_p));
		}
		/* [60,80) */
		else if(diff_p <= CalibrationData.calflow_buff[3]){
			measure_flow = (int)(((float)CalibrationData.k[2]/100.0f) * sqrt(diff_p));
		}		
		/* [80,100) */
		else if(diff_p <= CalibrationData.calflow_buff[4]){
			measure_flow = (int)(((float)CalibrationData.k[3]/100.0f) * sqrt(diff_p));
		}		
		/* [100,120) */
		else if(diff_p <= CalibrationData.calflow_buff[5]){
			measure_flow = (int)(((float)CalibrationData.k[4]/100.0f) * sqrt(diff_p));
		}
		/* >=120 */
		else if(diff_p > CalibrationData.calflow_buff[5]){
			measure_flow = (int)(((float)CalibrationData.k[5]/100.0f) * sqrt(diff_p));
		}			

		if(fdiff_press < 0.0f &&  measure_flow > 0)
			measure_flow = -measure_flow;

		Run_Param.flow_data = measure_flow;
		xQueueOverwrite(RunParamQueue_Handle,&Run_Param);

	}
	else
		measure_flow = Run_Param.flow_data;
	return measure_flow;
}



// void Mid_CalculateFlow(void *pvParameter){
// 	float fdiff_press;
// 	uint32_t diff_p = 0;
// 	int measure_flow = 0;

// 	while(1){
// 		if(Mid_SDP31Data_Process(&fdiff_press) == SUCCESS){
// 			if(fdiff_press < 0.0f)
// 				diff_p = (-fdiff_press)*100;
// 			else
// 				diff_p = fdiff_press*100;
			
// //			Run_Param.flow_data = (int)(240 * sqrt(diff_p));
// 			/* [0,40) */
// 			if(diff_p <= CalibrationData.calflow_buff[1]){
// 				measure_flow = (int)(((float)CalibrationData.k[0]/100.0f) * sqrt(diff_p));
// 			} 
// 			/* [40,60) */
// 			else if(diff_p <= CalibrationData.calflow_buff[2]){
// 				measure_flow = (int)(((float)CalibrationData.k[1]/100.0f) * sqrt(diff_p));
// 			}
// 			/* [60,80) */
// 			else if(diff_p <= CalibrationData.calflow_buff[3]){
// 				measure_flow = (int)(((float)CalibrationData.k[2]/100.0f) * sqrt(diff_p));
// 			}		
// 			/* [80,100) */
// 			else if(diff_p <= CalibrationData.calflow_buff[4]){
// 				measure_flow = (int)(((float)CalibrationData.k[3]/100.0f) * sqrt(diff_p));
// 			}		
// 			/* [100,120) */
// 			else if(diff_p <= CalibrationData.calflow_buff[5]){
// 				measure_flow = (int)(((float)CalibrationData.k[4]/100.0f) * sqrt(diff_p));
// 			}
// 			/* >=120 */
// 			else if(diff_p > CalibrationData.calflow_buff[5]){
// 				measure_flow = (int)(((float)CalibrationData.k[5]/100.0f) * sqrt(diff_p));
// 			}			

// 			if(fdiff_press < 0.0f &&  measure_flow > 0)
// 				measure_flow = -measure_flow;

// 			taskENTER_CRITICAL();
// 			Run_Param.flow_data = measure_flow;
// 			taskEXIT_CRITICAL();

// 			/* Run Mid_JudgeInsEx Task. */
// 			// if(Machine_State.flag_machine_onoff == SET)
// 			// 	xEventGroupSetBits(MachineStateEvent_Handle,CaculateFlowEnd_Event);
// 			vTaskDelay(pdMS_TO_TICKS(20));
// 		}
// 	}
// }




