#include "Application_ReceiveBlower.h"

/*!
	@brief 		 Main chip receives data from blower chip.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
	@notic		 It can use after usart2 receive data.If not,program will error!
*/
FlagStatus flag_runcommand = RESET;
void App_ReceiveBlower_Task(void *pvPatameter){
	UARTQueue_t motor_buff_code;
	while(1){
		xQueueReceive(BlowerRECQueue_Handle,&motor_buff_code,portMAX_DELAY);
		/* Judge data frame header and frame end and crc-8. */
		if((motor_buff_code.buff[0] == BLOWER_FRAMEHEADER) && 
            (((motor_buff_code.buff[5] << 8) | motor_buff_code.buff[6]) == BLOWER_FRAMEEND) &&
		    (motor_buff_code.buff[4] == Check_BlowerCRC8(&motor_buff_code.buff[0],4))){
				
			/* Machine power on just now and this program just runs once. */
			if(flag_runcommand == RESET){
				if(((motor_buff_code.buff[2]<<8) | motor_buff_code.buff[3]) == 0x0011){
					Blower_State.state = BlowerNormal;
					flag_runcommand = SET;
				}
			}
			/* Blower chip return running state. */
			else{
				if(motor_buff_code.buff[1] == SEND_RPM){
					Blower_State.actual_rpm = (motor_buff_code.buff[2]<<8)|motor_buff_code.buff[3];
				}
				else if(motor_buff_code.buff[1] == SEND_COMMAND){
					if(((motor_buff_code.buff[2]<<8)|motor_buff_code.buff[3]) == 0x0012){
						Blower_State.state = BlowerError;
					}
				}
			}	
		}
		xQueueOverwrite(BlowerStateQueue_Handle,&Blower_State);
	}
}





