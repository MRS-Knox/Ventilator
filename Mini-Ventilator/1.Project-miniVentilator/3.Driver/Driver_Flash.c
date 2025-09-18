#include "Driver_Flash.h"
//Stm32f44ret6 flash is 512k,4 x 16k,1 x 64k,3 x 128k,FLASH_Sector_0 ~ FLASH_Sector_7
//FLASH_Sector_7:0x0806 0000 - 0x0807 FFFF

/*!
	@brief 		 This initial function is about flash hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_FlashHardware_Init(void){
	FLASH_SetLatency(FLASH_Latency_5);	//180Mhz.
	FLASH_PrefetchBufferCmd(ENABLE);	//Accelerate reading data from flash.
	FLASH_InstructionCacheCmd(DISABLE);
	FLASH_DataCacheCmd(DISABLE);
}

/*!
	@brief 		 This function is about to write data to flash.
	@param[in]	 data_t:...
	@param[out]	 none
	@retval		 ERROR or SUCCESS
*/
ErrorStatus Dri_Flash_Write(CalibrationData_t *data_t){
	uint16_t data[2];
	FLASH_Status status = FLASH_COMPLETE;
	FLASH_Unlock();
	/* STM32F4xx needs it. */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR |
					FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR | FLASH_FLAG_BSY| FLASH_FLAG_RDERR);
                        
	status = FLASH_EraseSector(FLASH_Sector_7,VoltageRange_3);
	if(status != FLASH_COMPLETE){
		FLASH_ClearFlag(status);
		FLASH_Lock();
		return ERROR;	
	}
	/* Pressure -> rpm. */
	for(uint8_t i = 0;i < CALPRESSMaxCount;i++){
		status = FLASH_ProgramHalfWord((CalDataPressStart_Address+i*2),data_t->calpress_buff[i]);
		if(status != FLASH_COMPLETE){
			FLASH_ClearFlag(status);
			FLASH_Lock();
			return ERROR;
		}		
	}
	/* Pressure compensation. */
	status = FLASH_ProgramHalfWord((CalDataPRESSCOMStart_Address),data_t->press_compensation);
	if(status != FLASH_COMPLETE){
		FLASH_ClearFlag(status);
		FLASH_Lock();
		return ERROR;
	}		
	/* k*100. */
	for(uint8_t i = 0;i < CALFlow_KMaxCount;i++){
		status = FLASH_ProgramHalfWord((CalDataFlowKStart_Address+i*2),data_t->k[i]);
		if(status != FLASH_COMPLETE){
			FLASH_ClearFlag(status);
			FLASH_Lock();
			return ERROR;
		}		
	}
	/* Difference pressure,step 2. */
	for(uint8_t i = 0;i < CALFlowMaxCount;i++){
		status = FLASH_ProgramHalfWord((CalDataFlowStart_Address+i*4),(data_t->calflow_buff[i]>>16));
		if(status != FLASH_COMPLETE){
			FLASH_ClearFlag(status);
			FLASH_Lock();
			return ERROR;
		}
		status = FLASH_ProgramHalfWord((CalDataFlowStart_Address+i*4+2),(uint16_t)data_t->calflow_buff[i]);
		if(status != FLASH_COMPLETE){
			FLASH_ClearFlag(status);
			FLASH_Lock();
			return ERROR;
		}
	}
	/* About RTC. */
	status = FLASH_ProgramHalfWord((CalDataRTCStart_Address),data_t->flag_rtc_init);
	if(status != FLASH_COMPLETE){
		FLASH_ClearFlag(status);
		FLASH_Lock();
		return ERROR;
	}		
	/* Machine model. */
	status = FLASH_ProgramHalfWord((CalDataModelStart_Address),data_t->machine_model);
	if(status != FLASH_COMPLETE){
		FLASH_ClearFlag(status);
		FLASH_Lock();
		return ERROR;
	}		
	FLASH_Lock();

	/* Check pressure->rpm and flow->DP. */
	for(uint8_t i = 0;i < CALPRESSMaxCount;i++){
		data[0] = Dri_Flash_Read((CalDataPressStart_Address+i*2));
		if(data[0] != data_t->calpress_buff[i])
			return ERROR; 	
	}
	for(uint8_t i = 0;i < CALFlowMaxCount;i++){
		data[0] = Dri_Flash_Read(CalDataFlowStart_Address+i*4);
		data[1] = Dri_Flash_Read(CalDataFlowStart_Address+i*4+2);
		if(((data[0]<<16)|data[1]) != data_t->calflow_buff[i])
			return ERROR; 	
	}
	return SUCCESS;
}

/*!
	@brief 		 This function is about to read data from flash.
	@param[in]	 read_address:...
				 read_data:read out data buff
	@param[out]	 none
	@retval		 none
*/
uint16_t Dri_Flash_Read(uint32_t read_address){
	return (*(__IO uint16_t*)read_address);
}


