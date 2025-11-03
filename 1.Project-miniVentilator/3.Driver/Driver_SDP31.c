#include "Driver_SDP31.h"

/*!
	@brief 		 Flow SCL initialize
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_Flow_SCL_Init(){	
	GPIO_InitTypeDef Flow_SCL_Init;
	
	Flow_SCL_Init.GPIO_Pin 	 = IIC2_SCL_Pin;
	Flow_SCL_Init.GPIO_Mode  = GPIO_Mode_OUT;
	Flow_SCL_Init.GPIO_Speed = GPIO_Fast_Speed;
	Flow_SCL_Init.GPIO_OType = GPIO_OType_PP;
	Flow_SCL_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(IIC2_SCL_Group,&Flow_SCL_Init);
}

/*!
	@brief 		 set Flow SDA state
	@param[in]	 state:1 is input,0 is output
	@param[out]	 none
	@retval		 none
*/
void Dri_Flow_SDA_State(uint8_t state){
	GPIO_InitTypeDef Flow_SDA_Init;
	
	Flow_SDA_Init.GPIO_Pin = IIC2_SDA_Pin;
	if(state == 1){
		Flow_SDA_Init.GPIO_Mode = GPIO_Mode_IN;
		Flow_SDA_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	}
	else{
		Flow_SDA_Init.GPIO_Mode = GPIO_Mode_OUT;
		Flow_SDA_Init.GPIO_Speed = GPIO_Fast_Speed;
		Flow_SDA_Init.GPIO_OType = GPIO_OType_PP;
		Flow_SDA_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	}
	GPIO_Init(IIC2_SDA_Group,&Flow_SDA_Init);
}

/*!
	@brief 		 IIC start
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void FLOW_Start(){
	FLOW_SDA_OUT;
	
	FLOW_SCL_Low;
	FLOW_SDA_High;
	FLOW_SCL_High;
	udelay_1us(2);
	
	FLOW_SDA_Low;
	udelay_1us(1);
	FLOW_SCL_Low;	
}

/*!
	@brief 		 IIC stop
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void FLOW_Stop(){
	FLOW_SDA_OUT;
	
	FLOW_SCL_Low;
	FLOW_SDA_Low;
	FLOW_SCL_High;
	udelay_1us(2);
	
	FLOW_SDA_High;
	udelay_1us(1);
	FLOW_SCL_Low;	
}

/*!
	@brief 		 IIC ack
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void FLOW_Ack(){
	FLOW_SDA_OUT;
	
	FLOW_SCL_Low;
	FLOW_SDA_Low;
	udelay_1us(1);
	FLOW_SCL_High;
	udelay_1us(1);
	FLOW_SCL_Low;
}

/*!
	@brief 		 IIC wait ack
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void FLOW_Wait_Ack(){
	uint8_t count = 0;
	FLOW_SDA_OUT;
	
	FLOW_SCL_Low;
	FLOW_SDA_High;
	udelay_1us(1);
	
	FLOW_SDA_IN;
	udelay_1us(1);
	
	FLOW_SCL_High;
	while(GPIO_ReadInputDataBit(IIC2_SDA_Group,IIC2_SDA_Pin)){
		udelay_1us(1);
		if(++count >= 20){
			FLOW_Stop();
			break;
		}
	}
//	udelay_1us(1);
	
	FLOW_SCL_Low;
	FLOW_SDA_OUT;
}

/*!
	@brief 		 IIC wait no ack
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void FLOW_No_Ack(){
	FLOW_SDA_OUT;
	
	FLOW_SCL_Low;
	FLOW_SDA_High;
	udelay_1us(1);
	
	FLOW_SCL_High;
	udelay_1us(1);
	
	FLOW_SCL_Low;
	FLOW_SDA_Low;
}

/*!
	@brief 		 IIC write one byte
	@param[in]	 data:need write data
	@param[out]	 none
	@retval		 none
*/
void FLOW_Write_Byte(uint8_t data){
	FLOW_SDA_OUT;
	
	uint8_t count;
	uint8_t dat = 0x00;
	FLOW_SCL_Low;
	
	for(count = 0;count < 8;count++){
		FLOW_SCL_Low;
		dat = data & 0x80;
		data <<= 1;
		if(dat == 0x80){
			FLOW_SDA_High;
		}
		else{
			FLOW_SDA_Low;
		}
		udelay_1us(2);
		FLOW_SCL_High;
		udelay_1us(2);
	}
	FLOW_SCL_Low;
}

/*!
	@brief 		 IIC read one byte
	@param[in]	 none
	@param[out]	 none
	@retval		 data:read data
*/
uint8_t FLOW_Read_Byte(){
	FLOW_SDA_IN;

	uint8_t count;
	uint8_t data = 0x00;
	
	FLOW_SCL_Low;	
	for(count = 0;count < 8;count++){
		FLOW_SCL_Low;
		udelay_1us(1);
		
		FLOW_SCL_High;
		data <<= 1;
		if(GPIO_ReadInputDataBit(IIC2_SDA_Group,IIC2_SDA_Pin)){
			data++;	
		}
		udelay_1us(1);
	}
	
	FLOW_SCL_Low;
	FLOW_SDA_OUT;
	return data;
}





















































// /*!
// 	@brief 		 This initial function is about flow sensor hardware.
// 	@param[in]	 slave_address:The slave address.
// 				 iic_clock:The iic clock frequency.
// 	@param[out]	 none
// 	@retval		 none
// */
// void Dri_FlowHardware_Init(uint8_t self_address,uint32_t iic_clock){
	
	
// 	GPIO_InitTypeDef 	Flow_IIC2GPIO_Init;
// 	Flow_IIC2GPIO_Init.GPIO_Pin   = IIC2_SDA_Pin; 
// 	Flow_IIC2GPIO_Init.GPIO_Mode  = GPIO_Mode_OUT;
// 	Flow_IIC2GPIO_Init.GPIO_Speed = GPIO_Speed_25MHz;
// 	Flow_IIC2GPIO_Init.GPIO_OType = GPIO_OType_PP;
// 	Flow_IIC2GPIO_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
// 	GPIO_Init(IIC2_SDA_Group,&Flow_IIC2GPIO_Init);
// 	Flow_IIC2GPIO_Init.GPIO_Pin   = IIC2_SCL_Pin; 
// 	GPIO_Init(IIC2_SCL_Group,&Flow_IIC2GPIO_Init);
	
// 	GPIO_WriteBit(IIC2_SDA_Group,IIC2_SDA_Pin,Bit_SET);
// 	GPIO_WriteBit(IIC2_SCL_Group,IIC2_SCL_Pin,Bit_SET);
	
	
// //	GPIO_InitTypeDef 	Flow_IIC2GPIO_Init;
// //	I2C_InitTypeDef 	Flow_IIC2_Init;
// //	I2C_DeInit(I2C2);
// //	/* Initialize IIC2. */
// //	Flow_IIC2GPIO_Init.GPIO_Pin 	= IIC2_SCL_Pin; 
// //	Flow_IIC2GPIO_Init.GPIO_Mode 	= GPIO_Mode_AF;
// //	Flow_IIC2GPIO_Init.GPIO_Speed 	= GPIO_Speed_50MHz;
// //	Flow_IIC2GPIO_Init.GPIO_OType 	= GPIO_OType_OD;
// //	Flow_IIC2GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_UP;
// //	GPIO_Init(IIC2_SCL_Group,&Flow_IIC2GPIO_Init);
// //	Flow_IIC2GPIO_Init.GPIO_Pin 	= IIC2_SDA_Pin; 
// //	GPIO_Init(IIC2_SDA_Group,&Flow_IIC2GPIO_Init);
// //	GPIO_PinAFConfig(IIC2_SCL_Group,GPIO_PinSource10,GPIO_AF_I2C2);
// //	GPIO_PinAFConfig(IIC2_SDA_Group,GPIO_PinSource12,GPIO_AF_I2C2);
// //	
// //    Flow_IIC2_Init.I2C_Mode 		= I2C_Mode_I2C;
// //    Flow_IIC2_Init.I2C_DutyCycle 	= I2C_DutyCycle_2;
// //    Flow_IIC2_Init.I2C_OwnAddress1 	= self_address; 		
// //    Flow_IIC2_Init.I2C_Ack 			= I2C_Ack_Enable;
// //    Flow_IIC2_Init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
// //	iic_clock = iic_clock >= 400000 ? 400000 : iic_clock;	//Max clock frequency is 400KHz.
// //    Flow_IIC2_Init.I2C_ClockSpeed 	= iic_clock; 		
// //    I2C_Init(I2C2, &Flow_IIC2_Init);

// //    I2C_Cmd(I2C2, ENABLE);
// }

// /*!
// 	@brief 		 This function is about main chip send data to flow sensor.
// 	@param[in]	 none
// 	@param[out]	 pdata:Measure data.
// 				 flag_measurefinish:Measure finish or not.
// 	@retval		 none
// */
// void Dri_FlowReadData(uint8_t *pdata,FlagStatus *flag_measurefinish){
// 	static uint32_t time_out = 0;
// 	static uint8_t flow_step = 0;
// 	uint16_t time_limit = 50000;
// 	*flag_measurefinish = RESET;
//     /* Wait for the iic to be idle. */
//     while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)){
// 		if(time_out++ >= time_limit){
// 			Dri_FlowHardware_Init(0x00,50000);
// 			time_out = 0;
// 			return;
// 		}
// 	}
// 	time_out = 0;
	
// 	switch(flow_step){
// 		case 0:
// 			/* -------- Send AllMeasure_Mode to CMD registor address. -------- */
// 			I2C_GenerateSTART(I2C2,ENABLE);			//Send start bit.			
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_Send7bitAddress(I2C2,Sensor_Address,I2C_Direction_Transmitter);		//Send slave address(write).
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_SendData(I2C2,CMD_Address);		//Send register address.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return; 
// 				}
// 			}
// 			time_out = 0;
// 			I2C_SendData(I2C2,AllMeasure_Mode);		//Send mode.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_GenerateSTOP(I2C2,ENABLE);		//Send stop bit.
// 			flow_step = 1;
// 			break;	
// 		case 1:
// 			/* -------- Wait for the flow senor measures finished. -------- */		
// 			I2C_GenerateSTART(I2C2,ENABLE);			//Send start bit.			
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_Send7bitAddress(I2C2,Sensor_Address,I2C_Direction_Transmitter);		//Send slave address(write).
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_SendData(I2C2,CMD_Address);		//Send register address.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_GenerateSTART(I2C2,ENABLE);		//Send start bit again.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			I2C_Send7bitAddress(I2C2,Sensor_Address,I2C_Direction_Receiver);	//Send slave address(read).
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;  
// 			I2C_AcknowledgeConfig(I2C2,DISABLE); //Send NACK,after finish read next byte.  
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			if((I2C_ReceiveData(I2C2)&0x08) == 0x0){	//Sensor measures finished.
// 				flow_step = 2;
// 			}
// 			I2C_GenerateSTOP(I2C2,ENABLE);		//Send stop bit.
// 			I2C_AcknowledgeConfig(I2C2,ENABLE);	//Enable ACK.
// 			break;
// 		case 2:
// 			/* -------- Read temperature and difference pressure. -------- */
// 			I2C_GenerateSTART(I2C2,ENABLE);			//Send start bit.			
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_Send7bitAddress(I2C2,Sensor_Address,I2C_Direction_Transmitter);		//Send slave address(write).
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_SendData(I2C2,DATA_MSB);		//Send register address.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_GenerateSTART(I2C2,ENABLE);		//Send start bit again.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			I2C_Send7bitAddress(I2C2,Sensor_Address,I2C_Direction_Receiver);	//Send slave address(read).
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[0] = I2C_ReceiveData(I2C2);	//Read difference pressure high byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[1] = I2C_ReceiveData(I2C2);	//Read difference pressure middle byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[2] = I2C_ReceiveData(I2C2);	//Read difference pressure low byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[3] = I2C_ReceiveData(I2C2);	//Read temperature high byte.
// 			I2C_AcknowledgeConfig(I2C2,DISABLE); //Send NACK,after finish read next byte.  
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[4] = I2C_ReceiveData(I2C2);	//Read temperature low byte.	
// 			I2C_GenerateSTOP(I2C2,ENABLE);		//Send stop bit.
// 			I2C_AcknowledgeConfig(I2C2,ENABLE);	//Enable ACK.
// 			*flag_measurefinish = SET;
// 			flow_step = 0;
// 			break;
// 		default:
// 			flow_step = 0;
// 			break;
// 	}	
// }


// void Dri_ReadSDP3x(uint8_t *pdata,FlagStatus *flag_measurefinish){
// 	static uint32_t time_out = 0;
// 	static uint8_t flow_step = 0;
// 	uint16_t time_limit = 50000;
// 	*flag_measurefinish = RESET;
//     /* Wait for the iic to be idle. */
//     while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY)){
// 		if(time_out++ >= time_limit){
// 			Dri_FlowHardware_Init(0x00,50000);
// 			time_out = 0;
// 			return;
// 		}
// 	}
// 	time_out = 0;
// 	switch(flow_step){
// 		case 0:
// 			/* -------- Send command. -------- */
// 			I2C_GenerateSTART(I2C2,ENABLE);			//Send start bit.			
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_Send7bitAddress(I2C2,0x21,I2C_Direction_Transmitter);		//Send slave address(write).
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_SendData(I2C2,0x36);		//Send register address.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return; 
// 				}
// 			}
// 			time_out = 0;
// 			I2C_SendData(I2C2,0x24);		//Send register address.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return; 
// 				}
// 			}
// 			time_out = 0;
// 			I2C_GenerateSTOP(I2C2,ENABLE);		//Send stop bit.
// 			flow_step = 1;
// 			break;	
// 		case 1:
// 			/* -------- Read temperature and difference pressure. -------- */
// 			I2C_GenerateSTART(I2C2,ENABLE);			//Send start bit.			
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;
// 			I2C_Send7bitAddress(I2C2,0x21,I2C_Direction_Receiver);		//Send slave address(write).
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[0] = I2C_ReceiveData(I2C2);	//Read difference pressure high byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[1] = I2C_ReceiveData(I2C2);	//Read difference pressure low byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[2] = I2C_ReceiveData(I2C2);	//Read CRC-8.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[3] = I2C_ReceiveData(I2C2);	//Read temperature high byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[4] = I2C_ReceiveData(I2C2);	//Read temperature low byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[5] = I2C_ReceiveData(I2C2);	//Read CRC-8.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[6] = I2C_ReceiveData(I2C2);	//Read temperature high byte.
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[7] = I2C_ReceiveData(I2C2);	//Read temperature low byte.
// 			I2C_AcknowledgeConfig(I2C2,DISABLE); //Send NACK,after finish read next byte.  
// 			while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)){
// 				if(time_out++ >= time_limit){
// 					time_out = 0;
// 					return;
// 				}
// 			}
// 			time_out = 0;   
// 			pdata[8] = I2C_ReceiveData(I2C2);	//Read CRC-8.
// 			I2C_GenerateSTOP(I2C2,ENABLE);		//Send stop bit.
// 			I2C_AcknowledgeConfig(I2C2,ENABLE);	//Enable ACK.
// 			*flag_measurefinish = SET;
// 			flow_step = 0;
// 			break;
// 		default:
// 			flow_step = 0;
// 			break;
// 	}	
// }







