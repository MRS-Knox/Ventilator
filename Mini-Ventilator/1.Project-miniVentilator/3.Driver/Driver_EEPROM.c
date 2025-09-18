#include "Driver_EEPROM.h"

/*!
	@brief 		 Initialize EEPROM SCL,EN and WC pin.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_EEPROM_SCLENWC_Init(){	
	GPIO_InitTypeDef EEPROM_GPIO_Init;
	
	EEPROM_GPIO_Init.GPIO_Pin   = EEPROM_EN_Pin;
	EEPROM_GPIO_Init.GPIO_Mode  = GPIO_Mode_OUT;
	EEPROM_GPIO_Init.GPIO_Speed = GPIO_Fast_Speed;
	EEPROM_GPIO_Init.GPIO_OType = GPIO_OType_PP;
	EEPROM_GPIO_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(EEPROM_SCLSDAEN_Group,&EEPROM_GPIO_Init);
	EEPROM_GPIO_Init.GPIO_Pin   = EEPROM_SCL_Pin;
	GPIO_Init(EEPROM_SCLSDAEN_Group,&EEPROM_GPIO_Init);
	EEPROM_GPIO_Init.GPIO_Pin   = EEPROM_WC_Pin;
	GPIO_Init(EEPROM_WC_Group,&EEPROM_GPIO_Init);

	GPIO_WriteBit(EEPROM_SCLSDAEN_Group,EEPROM_EN_Pin,Bit_RESET);
	EEPROM_WC_High;
}

/*!
	@brief 		 Initialize EEPROM SDA pin. 
	@param[in]	 state:1 is input,0 is output
	@param[out]	 none
	@retval		 none
*/
void Dri_EEPROM_SDA_State(uint8_t state){
	GPIO_InitTypeDef EEPROM_GPIO_Init;
	
	EEPROM_GPIO_Init.GPIO_Pin = EEPROM_SDA_Pin;
	if(state == 1){
		EEPROM_GPIO_Init.GPIO_Mode 	= GPIO_Mode_IN;
		EEPROM_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	}
	else{
		EEPROM_GPIO_Init.GPIO_Mode 	= GPIO_Mode_OUT;
		EEPROM_GPIO_Init.GPIO_Speed = GPIO_Fast_Speed;
		EEPROM_GPIO_Init.GPIO_OType = GPIO_OType_PP;
		EEPROM_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	}
	GPIO_Init(EEPROM_SCLSDAEN_Group,&EEPROM_GPIO_Init);
}

/*!
	@brief 		 IIC start
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void EEPROM_Start(){
	EEPROM_SDA_OUT;
	
	EEPROM_SCL_Low;
	EEPROM_SDA_High;
	EEPROM_SCL_High;
	udelay_1us(2);
	
	EEPROM_SDA_Low;
	udelay_1us(1);
	EEPROM_SCL_Low;	
}

/*!
	@brief 		 IIC stop
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void EEPROM_Stop(){
	EEPROM_SDA_OUT;
	
	EEPROM_SCL_Low;
	EEPROM_SDA_Low;
	EEPROM_SCL_High;
	udelay_1us(2);
	
	EEPROM_SDA_High;
	udelay_1us(1);
	EEPROM_SCL_Low;	
}

/*!
	@brief 		 IIC ack
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void EEPROM_Ack(){
	EEPROM_SDA_OUT;
	
	EEPROM_SCL_Low;
	EEPROM_SDA_Low;
	udelay_1us(2);
	EEPROM_SCL_High;
	udelay_1us(3);
	EEPROM_SCL_Low;
}

/*!
	@brief 		 IIC wait ack
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void EEPROM_Wait_Ack(){
	uint8_t count = 0;
	EEPROM_SDA_OUT;
	
	EEPROM_SCL_Low;
	EEPROM_SDA_High;
	udelay_1us(1);
	
	EEPROM_SDA_IN;
	udelay_1us(1);
	
	EEPROM_SCL_High;
	while(GPIO_ReadInputDataBit(EEPROM_SCLSDAEN_Group,EEPROM_SDA_Pin)){
		udelay_1us(1);
		if(++count >= 20){
			EEPROM_Stop();
			break;
		}
	}
	udelay_1us(1);
	
	EEPROM_SCL_Low;
	EEPROM_SDA_OUT;
}

/*!
	@brief 		 IIC wait no ack
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void EEPROM_No_Ack(){
	EEPROM_SDA_OUT;
	
	EEPROM_SCL_Low;
	EEPROM_SDA_High;
	udelay_1us(1);
	
	EEPROM_SCL_High;
	udelay_1us(1);
	
	EEPROM_SCL_Low;
	EEPROM_SDA_Low;
}

/*!
	@brief 		 IIC write one byte
	@param[in]	 data:need write data
	@param[out]	 none
	@retval		 none
*/
void EEPROM_Write_Byte(uint8_t data){
	EEPROM_SDA_OUT;
	
	uint8_t count;
	uint8_t dat = 0x00;
	EEPROM_SCL_Low;
	
	for(count = 0;count < 8;count++){
		EEPROM_SCL_Low;
		dat = data & 0x80;
		data <<= 1;
		if(dat == 0x80){
			EEPROM_SDA_High;
		}
		else{
			EEPROM_SDA_Low;
		}
		udelay_1us(2);
		EEPROM_SCL_High;
		udelay_1us(5);
	}
	EEPROM_SCL_Low;
}

/*!
	@brief 		 IIC read one byte
	@param[in]	 none
	@param[out]	 none
	@retval		 data:read data
*/
uint8_t EEPROM_Read_Byte(){
	EEPROM_SDA_IN;

	uint8_t count;
	uint8_t data = 0x00;
	
	EEPROM_SCL_Low;	
	for(count = 0;count < 8;count++){
		EEPROM_SCL_Low;
		udelay_1us(5);
		
		EEPROM_SCL_High;
		data <<= 1;
		if(GPIO_ReadInputDataBit(EEPROM_SCLSDAEN_Group,EEPROM_SDA_Pin)){
			data++;	
		}
		udelay_1us(2);
	}
	
	EEPROM_SCL_Low;
	EEPROM_SDA_OUT;
	return data;
}


