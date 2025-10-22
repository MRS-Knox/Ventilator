#include "Middle_Flow.h"


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
	// Dri_FlowHardware_Init(0x00,10000);
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
	@brief 		 read SDP810 data
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
	@param[out]	 none
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
		
		*pdiffdata = Kalman_Filter(&Flow_Kalman,*pdiffdata);
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
void Mid_CalculateFlow(void *pvParameter){
	float fdiff_press;
	float k_buff[7];
	float b_buff[7]; 
	uint32_t diff_p = 0;
	for(uint8_t i = 0;i < 7;i++){
		k_buff[i] = (float)CALPress_Flow.k[i]/COE_Multiple;
		b_buff[i] = (float)CALPress_Flow.b[i]/COE_Multiple;
		if((CALPress_Flow.flag_coesymbol&(0x01<<(7+i))) > 0)
			k_buff[i] = -k_buff[i];
		if((CALPress_Flow.flag_coesymbol&(0x01<<i)) > 0)
			b_buff[i] = -b_buff[i];
	}
	while(1){
		if(Mid_SDP31Data_Process(&fdiff_press) == SUCCESS){
			if(fdiff_press < 0.0f)
				diff_p = (-fdiff_press)*100;
			else
				diff_p = fdiff_press*100;
			
			/* [0,40) */
			if(diff_p <= CALPress_Flow.calflow_buff[1]){
				Run_Param.flow_data = (int)((k_buff[0] * fdiff_press + b_buff[0])*100);
			} 
			/* [40,60) */
			else if(diff_p <= CALPress_Flow.calflow_buff[11]){
				Run_Param.flow_data = (int)((k_buff[1] * fdiff_press + b_buff[1])*100);
			}
			/* [60,80) */
			else if(diff_p <= CALPress_Flow.calflow_buff[21]){
				Run_Param.flow_data = (int)((k_buff[2] * fdiff_press + b_buff[2])*100);
			}		
			/* [80,100) */
			else if(diff_p <= CALPress_Flow.calflow_buff[31]){
				Run_Param.flow_data = (int)((k_buff[3] * fdiff_press + b_buff[3])*100);
			}		
			/* [100,120) */
			else if(diff_p <= CALPress_Flow.calflow_buff[41]){
				Run_Param.flow_data = (int)((k_buff[4] * fdiff_press + b_buff[4])*100);
			}
			/* [120,140) */
			else if(diff_p <= CALPress_Flow.calflow_buff[51]){
				Run_Param.flow_data = (int)((k_buff[5] * fdiff_press + b_buff[5])*100);
			}
			/* >=140 */
			else if(diff_p > CALPress_Flow.calflow_buff[51]){
				Run_Param.flow_data = (int)((k_buff[6] * fdiff_press + b_buff[6])*100);
			}

			if(fdiff_press < 0.0f &&  Run_Param.flow_data > 0)
				Run_Param.flow_data = -Run_Param.flow_data;
			vTaskDelay(pdMS_TO_TICKS(50));
		}
	}
}

/*!
	@brief 		 Calibrate flow base on difference pressure.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
// void Mid_CalibrateFlow(){
// 	CALPress_Flow.calflow_buff[CALPress_Flow.calflow_count++] = Run_Param.diff_press;
// 	if(CALPress_Flow.calflow_count >= CALFlowMaxCount){
// 		CALPress_Flow.calflow_count = 0;
// 		Mid_CALCoefficient();
// 	}	
// }

/*!
	@brief 		 Calculate coefficient(k,b).
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_CALCoefficient(){
	unsigned short flow_buff[CALFlowMaxCount-1];
	unsigned char count = 1;
	float k_buff[7],b_buff[7];
	flow_buff[0] = 0;
	for(unsigned char i = 40;i <= 150;i=i+2){
		flow_buff[count++] = i*100;
	}
	CALPress_Flow.flag_coesymbol = 0;
	/* [0,40) */
	LeastSquareMethod(&CALPress_Flow.calflow_buff[0],&flow_buff[0],2,&k_buff[0],&b_buff[0]);
	/* [40,60) */
	LeastSquareMethod(&CALPress_Flow.calflow_buff[1],&flow_buff[1],11,&k_buff[1],&b_buff[1]);
	/* [60,80) */
	LeastSquareMethod(&CALPress_Flow.calflow_buff[11],&flow_buff[11],11,&k_buff[2],&b_buff[2]);
	/* [80,100) */
	LeastSquareMethod(&CALPress_Flow.calflow_buff[21],&flow_buff[21],11,&k_buff[3],&b_buff[3]);
	/* [100,120) */
	LeastSquareMethod(&CALPress_Flow.calflow_buff[31],&flow_buff[31],11,&k_buff[4],&b_buff[4]);
	/* [120,140) */
	LeastSquareMethod(&CALPress_Flow.calflow_buff[41],&flow_buff[41],11,&k_buff[5],&b_buff[5]);
	/* [140,150] */
	LeastSquareMethod(&CALPress_Flow.calflow_buff[51],&flow_buff[51],6,&k_buff[6],&b_buff[6]);
	
	for(unsigned char i = 0;i < 7;i++){
		CALPress_Flow.k[i] = (unsigned int)(fabs((double)k_buff[i])*10000);
		CALPress_Flow.b[i] = (unsigned int)(fabs((double)b_buff[i])*10000);
		if(k_buff[i] < 0.0f)
			CALPress_Flow.flag_coesymbol = CALPress_Flow.flag_coesymbol | (0x01<<(7+i));
		if(b_buff[i] < 0.0f)
			CALPress_Flow.flag_coesymbol = CALPress_Flow.flag_coesymbol | (0x01<<i);
	}
}


/*!	
	@brief 		 Calculate the flow from the difference pressure 
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
//uint16_t calflow_buff[10] = {0,3000,5900,12000,21136,32243,12559,16503};	//calibrate flow buff 	start 20lpm  interval 20lpm
//void Mid_CalculateFlow(void *pvParameter){
//	uint16_t value = 0;
//	uint8_t count = 0;
//	float flowtemp_buff[2] = {0.0f,0.0f};

//	while(1){
//		if(Mid_CalDifferenceP(flowtemp_buff) == SET){
//			if(flowtemp_buff[0] < 0)		//judge pressure difference <0 or >0
//				value = (-flowtemp_buff[0])*100;
//			else
//				value = flowtemp_buff[0]*100;	
//			
//			for(count = 0;count < 10;count++){	//search the flow seg in table
//				if(value <= calflow_buff[count])
//					break;
//			}
//			Run_Param.flow_data = (count-1)*2000 + 2000*(value-calflow_buff[count-1])/(calflow_buff[count]-calflow_buff[count-1]);
//			
//			
//			if((flowtemp_buff[0] < 0) && (Run_Param.flow_data > 0))//flow_data and pressure difference same symbol
//				Run_Param.flow_data = -Run_Param.flow_data;	
//		}
//		vTaskDelay(pdMS_TO_TICKS(50));

//	}

//} 


/*--------------GZP6897D--------------*/
//FlagStatus Mid_CalDifferenceP(float *pdata){
// 	static float zero_diffpress[5];
// 	static uint8_t flag_zero = 0;
// 	uint8_t diffp_buff[5] = {0,0,0,0,0};
// 	uint32_t middle_data = 0x00;
// 	FlagStatus flag_measurefinish = RESET;
// 	Dri_FlowReadData(diffp_buff,&flag_measurefinish);
// 	/* Measure finish or not. */
// 	if(flag_measurefinish == RESET)
// 		return flag_measurefinish;
// 	/* Calculate difference pressure. */
// 	middle_data = (diffp_buff[0]<<16) | (diffp_buff[1]<<8) | diffp_buff[2];
// 	if((diffp_buff[0]&0x80) == 0x80)
// 		pdata[0] = (float)((middle_data - 16777216.0f) / Sensor_K);
// 	else
// 		pdata[0] = (float)middle_data / Sensor_K;
// 	/* Calculate temperature. */
// 	middle_data	= (diffp_buff[3]<<8) | diffp_buff[4];
// 	if((diffp_buff[3]&0x80) == 0x80)
// 		pdata[1] = (float)((middle_data - 65536.0f) / 256.0f);
// 	else
// 		pdata[1] = (float)(middle_data / 256.0f);	
//	
// 	if(flag_zero < 5){
// 		zero_diffpress[flag_zero++] = pdata[0];
// 		return RESET;
// 	}
// 	else if(flag_zero == 5){
// 		flag_zero = 6;
// 		zero_diffpress[0] = (zero_diffpress[0]+zero_diffpress[1]+zero_diffpress[2]+zero_diffpress[3]+zero_diffpress[4])/5;
// 	}
// 	pdata[0] -= zero_diffpress[0];	
// 	pdata[0] = Kalman_Filter(&Flow_Kalman,pdata[0]);
// 	Run_Param.diff_press = pdata[0]*100;
// 	return flag_measurefinish;
// }
