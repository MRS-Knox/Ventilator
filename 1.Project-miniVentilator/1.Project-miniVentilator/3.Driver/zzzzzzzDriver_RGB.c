#include "Driver_RGB.h"

/*!
	@brief 		 This initial function is about RGB light hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_RGBHardware_Init(void){
	GPIO_InitTypeDef 	RGB_GPIO_Init;
	SPI_InitTypeDef		RGB_SPI_Init;
	/* Initialize SPI2. */
	RGB_GPIO_Init.GPIO_Pin 		= SPI2_MOSI_Pin; 
	RGB_GPIO_Init.GPIO_Mode 	= GPIO_Mode_AF;
	RGB_GPIO_Init.GPIO_Speed 	= GPIO_Speed_50MHz;
	RGB_GPIO_Init.GPIO_OType 	= GPIO_OType_PP;
	RGB_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(SPI2MOSI_Group,&RGB_GPIO_Init);
//	RGB_GPIO_Init.GPIO_Pin 		= SPI2_SCK_Pin; 
//	GPIO_Init(SPI2SCKNSS_Group,&RGB_GPIO_Init);
//	RGB_GPIO_Init.GPIO_Pin 		= SPI2_NSS_Pin; 
//	GPIO_Init(SPI2SCKNSS_Group,&RGB_GPIO_Init);
	GPIO_PinAFConfig(SPI2MOSI_Group,GPIO_PinSource3,GPIO_AF_SPI2);
//	GPIO_PinAFConfig(SPI2SCKNSS_Group,GPIO_PinSource12,GPIO_AF_SPI2);
//	GPIO_PinAFConfig(SPI2SCKNSS_Group,GPIO_PinSource13,GPIO_AF_SPI2);
	
	RGB_SPI_Init.SPI_Direction 			= SPI_Direction_1Line_Tx;	//Only transmission.
	RGB_SPI_Init.SPI_Mode				= SPI_Mode_Master;
	RGB_SPI_Init.SPI_DataSize			= SPI_DataSize_16b;
	RGB_SPI_Init.SPI_CPOL				= SPI_CPOL_High;
	RGB_SPI_Init.SPI_CPHA				= SPI_CPHA_1Edge;			//MOSI idle is high level.
	RGB_SPI_Init.SPI_NSS				= SPI_NSS_Soft;
	RGB_SPI_Init.SPI_BaudRatePrescaler 	= SPI_BaudRatePrescaler_16;	//The MOSI frequency is 2MHz,1 bit is 250ns.
	RGB_SPI_Init.SPI_FirstBit			= SPI_FirstBit_MSB;
	RGB_SPI_Init.SPI_CRCPolynomial		= 7;
	SPI_Init(SPI2,&RGB_SPI_Init);
	SPI_Cmd(SPI2,ENABLE);
}

/*!
	@brief 		 This function is about main chip send data to WS2812b.
	@param[in]	 data:.....
	@param[out]	 none
	@retval		 SUCCESS or ERROR
*/
ErrorStatus Dri_SPI_Send(uint16_t data){
	uint16_t time_out = 0;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET){	//Wait for transmission buffer empty.
		if(time_out++ > 60000)
			return ERROR;
	}		
	SPI_I2S_SendData(SPI2,data); 
	return SUCCESS;
}

/*!
	@brief 		 This function is about main chip send data to WS2812b.
	@param[in]	 rgb_count:The rgb light count.
				 prgb_depth:The red depth,green depth,blue depth.
	@param[out]	 none
	@retval		 none
*/
void Dri_SendDataTo_WS2812B(uint8_t rgb_count,uint8_t (*prgb_depth)[3]){
	uint16_t send_16bit = 0x0000;		
	uint8_t  send_8bit 	= 0x00;
	/* Send reset. */
	for(uint16_t i = 0;i < 75;i++){
		if(Dri_SPI_Send(0x0000) == ERROR)
			return;
	}
	for(uint8_t j = 0;j < rgb_count;j++){
		/* Send green depth. */
		send_8bit = prgb_depth[j][1];
		for(uint8_t i = 0;i < 4;i++){
			if((send_8bit & 0x80) == 0x80)
				send_16bit |= (Code_1<<8);
			else
				send_16bit |= (Code_0<<8);
			send_8bit <<= 1;
			if((send_8bit & 0x80) == 0x80)
				send_16bit |= Code_1;
			else
				send_16bit |= Code_0;
			Dri_SPI_Send(send_16bit);
			send_16bit = 0x0000;
			send_8bit <<= 1;
		}
		Dri_SPI_Send(0x0000);
		/* Send red depth. */
		send_8bit = prgb_depth[j][0];
		for(uint8_t i = 0;i < 4;i++){
			if((send_8bit & 0x80) == 0x80)
				send_16bit |= (Code_1<<8);
			else
				send_16bit |= (Code_0<<8);
			send_8bit <<= 1;
			if((send_8bit & 0x80) == 0x80)
				send_16bit |= Code_1;
			else
				send_16bit |= Code_0;
			Dri_SPI_Send(send_16bit);
			send_16bit = 0x0000;
			send_8bit <<= 1;
		} 
		Dri_SPI_Send(0x0000);
		/* Send blue depth. */
		send_8bit = prgb_depth[j][2];
		for(uint8_t i = 0;i < 4;i++){
			if((send_8bit & 0x80) == 0x80)
				send_16bit |= (Code_1<<8);
			else
				send_16bit |= (Code_0<<8);
			send_8bit <<= 1;
			if((send_8bit & 0x80) == 0x80)
				send_16bit |= Code_1;
			else
				send_16bit |= Code_0;
			Dri_SPI_Send(send_16bit);
			send_16bit = 0x0000;
			send_8bit <<= 1;
		}
		Dri_SPI_Send(0x0000);
	}
}




