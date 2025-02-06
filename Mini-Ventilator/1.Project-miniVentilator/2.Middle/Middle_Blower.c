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
	uint8_t motor_buff_code[7];
	if(data_type == SEND_RPM){
		if(send_data >= MOTOR_SPEED_MAX)  
			send_data = MOTOR_SPEED_MAX;
		else if(send_data <= MOTOR_SPEED_MIN && send_data != 0)
			send_data = MOTOR_SPEED_MIN;
	}
	motor_buff_code[0] = BLOWER_FRAMEHEADER;
	motor_buff_code[1] = data_type;
	motor_buff_code[2] = (uint8_t)(send_data>>8);
	motor_buff_code[3] = (uint8_t)send_data;
	motor_buff_code[4] = Send_BlowerCRC8(motor_buff_code,4);
	motor_buff_code[5] = (uint8_t)(BLOWER_FRAMEEND>>8);
	motor_buff_code[6] = (uint8_t)BLOWER_FRAMEEND;
	for(uint8_t i = 0;i < 7;i++){
		Blower_UartQueue.tx_buff[Blower_UartQueue.tx_in_count++] = motor_buff_code[i];
		if(Blower_UartQueue.tx_in_count >= UART_MAXBUFF) Blower_UartQueue.tx_in_count = 0;
	}
	Dri_BlowerControlSend(ENABLE); 	
}

/*!
	@brief 		 Main chip receives data from blower chip.
	@param[in]	 status:motor status command 
	@param[out]	 Blower_State.state:motor status
	@retval		 none
	@notic		 It can use after usart2 receive data.If not,program will error!
*/
void Mid_ReciveMotor(void){
	uint8_t motor_buff_code[7];
	static FlagStatus flag_runcommand = RESET;

	/* This variable what is rx_out_count point to data frame header. */
	while(Blower_UartQueue.rx_buff[Blower_UartQueue.rx_out_count] != BLOWER_FRAMEHEADER){
		Blower_UartQueue.rx_out_count++;
		if(Blower_UartQueue.rx_out_count >= UART_MAXBUFF)
			Blower_UartQueue.rx_out_count = 0;
		if(Blower_UartQueue.rx_out_count == Blower_UartQueue.rx_in_count)
			return;
	}
	/* Copy Blower_UartQueue.rx_buff to motor_buff_code. */
	for(uint8_t i = 0;i < 7;i++){
		motor_buff_code[i] = Blower_UartQueue.rx_buff[Blower_UartQueue.rx_out_count++];
		if(Blower_UartQueue.rx_out_count >= UART_MAXBUFF)
			Blower_UartQueue.rx_out_count = 0;
	}
	/* Judge crc-8. */
	if(motor_buff_code[4] == Check_BlowerCRC8(motor_buff_code,4)){
		/* Machine power on just now and this program just runs once. */
		if(flag_runcommand == RESET){
			if(((motor_buff_code[2]<<8)|motor_buff_code[3]) == 0x0011){
				Blower_State.state = BlowerNormal;
				flag_runcommand = SET;
			}
		}
		/* Blower chip return running state. */
		else{
			if(motor_buff_code[1] == SEND_RPM){
				Blower_State.actual_rpm = (motor_buff_code[2]<<8)|motor_buff_code[3];
			}
			else if(motor_buff_code[1] == SEND_COMMAND){
				if(((motor_buff_code[2]<<8)|motor_buff_code[3]) == 0x0014){
					Blower_State.state = BlowerError;
//					Mid_SendToBlower(SEND_COMMAND,0x0003);	
				}
			}
		}
	}
}






