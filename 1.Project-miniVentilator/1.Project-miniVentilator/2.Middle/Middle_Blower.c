#include "Middle_Blower.h"

/*!
	@brief 		 This initial function is about blower.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_Blower_Init(void){
	Dri_BlowerHardware_Init();
}

/*!
	@brief 		 This function is about send data to blower chip.
	@param[in]	 data_type:SEND_RPM or SEND_COMMAND.
				 send_data:rpm or command.
	@param[out]	 none
	@retval		 none
*/
void Mid_SendToBlower(uint8_t data_type,uint16_t send_data){
	if(data_type == SEND_RPM){
		if(send_data >= MOTOR_SPEED_MAX)  
			send_data = MOTOR_SPEED_MAX;
		else if(send_data <= MOTOR_SPEED_MIN && send_data != 0)
			send_data = MOTOR_SPEED_MIN;
		Blower_State.set_rpm = send_data;
	}
	
	BlowerSendQueue.buff[0] = BLOWER_FRAMEHEADER;
	BlowerSendQueue.buff[1] = data_type;
	BlowerSendQueue.buff[2] = (uint8_t)(send_data>>8);
	BlowerSendQueue.buff[3] = (uint8_t)send_data;
	BlowerSendQueue.buff[4] = Send_BlowerCRC8(&BlowerSendQueue.buff[0],4);
	BlowerSendQueue.buff[5] = (uint8_t)(BLOWER_FRAMEEND>>8);
	BlowerSendQueue.buff[6] = (uint8_t)BLOWER_FRAMEEND;
	
	xQueueSendToFront(BlowerSendQueue_Handle,&BlowerSendQueue,0);

	Dri_BlowerControlSend(ENABLE); 	
}

// /*!
// 	@brief 		 Main chip receives data from blower chip.
// 	@param[in]	 status:motor status command 
// 	@param[out]	 Blower_State.state:motor status
// 	@retval		 none
// 	@notic		 It can use after usart2 receive data.If not,program will error!
// */
// FlagStatus flag_runcommand = RESET;
// void Mid_ReciveMotor(void *pvPatameter){
// 	UARTQueue_t motor_buff_code;
// 	while(1){
// 		xQueueReceive(BlowerRECQueue_Handle,&motor_buff_code,portMAX_DELAY);
// 		/* Judge data frame header and frame end and crc-8. */
// 		if((motor_buff_code.buff[0] == BLOWER_FRAMEHEADER) && (((motor_buff_code.buff[5] << 8) | motor_buff_code.buff[6]) == BLOWER_FRAMEEND) &&
// 			(motor_buff_code.buff[4] == Check_BlowerCRC8(&motor_buff_code.buff[0],4))){
				
// 			/* Machine power on just now and this program just runs once. */
// 			if(flag_runcommand == RESET){
// 				if(((motor_buff_code.buff[2]<<8) | motor_buff_code.buff[3]) == 0x0011){
// 					Blower_State.state = BlowerNormal;
// 					flag_runcommand = SET;
// 				}
// 			}
// 			/* Blower chip return running state. */
// 			else{
// 				if(motor_buff_code.buff[1] == SEND_RPM){
// 					Blower_State.actual_rpm = (motor_buff_code.buff[2]<<8)|motor_buff_code.buff[3];
// 				}
// 				else if(motor_buff_code.buff[1] == SEND_COMMAND){
// 					if(((motor_buff_code.buff[2]<<8)|motor_buff_code.buff[3]) == 0x0012){
// 						Blower_State.state = BlowerError;
// 					}
// 				}
// 			}	
// 		}
// 		xQueueOverwrite(BlowerStateQueue_Handle,&Blower_State);
// 	}
// }

/*!
	@brief 		 Base pressure calculate blower rpm. 
	@param[in]	 press_value:Input value.
	@param[out]	 none
	@retval		 motor_rpm:Output value.
*/
uint16_t Mid_CalBlower_RPM(uint16_t press_value){
	uint16_t motor_rpm = 0;
	/* The pressure unit is converted to cmH2O. */
	float x = press_value/100.0f;	
	float k = 0.0f;	
	float b = 0.0f;
	
	if(x < 5.1f){ 			// <=5cmH20
		k = CalibrationData.calpress_buff[0] / 9.0f;
		b = CalibrationData.calpress_buff[0] - (5.0f * k);
	}
	else if(x < 10.1f){		// <=10cmH20
		k = (CalibrationData.calpress_buff[1] - CalibrationData.calpress_buff[0]) / 5.0f;
		b = CalibrationData.calpress_buff[1] - (10.0f * k);
	}
	else if(x < 15.1f){		// <=15cmH20
		k = (CalibrationData.calpress_buff[2] - CalibrationData.calpress_buff[1]) / 5.0f;
		b = CalibrationData.calpress_buff[2] - (15.0f * k);
	}
	else if(x < 20.1f){		// <=20cmH20
		k = (CalibrationData.calpress_buff[3] - CalibrationData.calpress_buff[2]) / 5.0f;
		b = CalibrationData.calpress_buff[3] - (20.0f * k);
	}
	else if(x < 25.1f){		//	<=25cmH20
		k = (CalibrationData.calpress_buff[4] - CalibrationData.calpress_buff[3]) / 5.0f;
		b = CalibrationData.calpress_buff[4] - (25.0f * k);
	}
	else{					//	>25cmH20
		k = (CalibrationData.calpress_buff[5] - CalibrationData.calpress_buff[4]) / 5.0f;
		b = CalibrationData.calpress_buff[5] - (30.0f * k);
	}	
	motor_rpm = (uint16_t)(k * x + b);
	return motor_rpm;
}


