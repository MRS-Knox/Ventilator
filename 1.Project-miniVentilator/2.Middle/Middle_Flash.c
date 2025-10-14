#include "Middle_Flash.h"

/*!
	@brief 		 This initial function is about flash hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_Flash_Init(void){
	Dri_FlashHardware_Init();
}

/*!
	@brief 		 Read calibration data from flash when machine just power on.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_ReadCALData_Power(){
	uint16_t data[4];
	/* Flash no data. */
	data[0] = Dri_Flash_Read(CalDataPressStart_Address);
	data[1] = Dri_Flash_Read(CalDataFlowKStart_Address);
	data[3] = Dri_Flash_Read(CalDataFlowStart_Address);
	if(data[0] == 0xFFFF && data[1] == 0xFFFF && data[3] == 0xFFFF)
		return;
	
	/* Calibrate pressure data. */
	for(uint8_t i = 0;i < CALPRESSMaxCount;i++){
		CalibrationData.calpress_buff[i] = Dri_Flash_Read(CalDataPressStart_Address+i*2);		
	}
	/* Pressure compensation. */
	CalibrationData.press_compensation = Dri_Flash_Read(CalDataPRESSCOMStart_Address);
	/* Calibrate flow coefficient. */
	for(uint8_t i = 0;i < CALFlow_KMaxCount;i++){
		CalibrationData.k[i] = Dri_Flash_Read(CalDataFlowKStart_Address+i*2);
	}
	/* Calibrate flow data. */
	for(uint8_t i = 0;i < CALFlowMaxCount;i++){
		data[0] = Dri_Flash_Read(CalDataFlowStart_Address+i*4);
		data[1] = Dri_Flash_Read(CalDataFlowStart_Address+i*4+2);
		CalibrationData.calflow_buff[i] = (data[0]<<16)|data[1];
	}
	/* RTC. */
	CalibrationData.flag_rtc_init = Dri_Flash_Read(CalDataRTCStart_Address);
	/* Machine model. */
	CalibrationData.machine_model = Dri_Flash_Read(CalDataModelStart_Address);
}

/*!
	@brief 		 This function is about to write calibration data to flash.
	@param[in]	 none
	@param[out]	 none
	@retval		 ErrorStatus:.....
*/
ErrorStatus Mid_WriteCALData(CalibrationData_t *data_t){
	return (Dri_Flash_Write(data_t));
}

