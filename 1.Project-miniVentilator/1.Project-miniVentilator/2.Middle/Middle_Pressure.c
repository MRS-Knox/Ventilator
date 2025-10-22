#include "Middle_Pressure.h"

/*!
	@brief 		 This initial function is about pressure.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_Pressure_Init(void){
	Dri_PHardware_Init();
}

/*!
	@brief 		 Voltage converts to pressure.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
int Mid_CalculatePRESS(void){
	float press_volatage = 0.0f;
	int press = 0;
	int press_com = 0;
	if((CalibrationData.press_compensation&(0x01<<15)) == (0x01<<15))
		press_com = -(CalibrationData.press_compensation - (0x01<<15));
	else
		press_com = CalibrationData.press_compensation;

	press_volatage = Dri_ReadPRESS_V()*3300.0f/4095.0f; 	//ADC	->	mV
	press = (int)(2.00147f*press_volatage - 425.80759f);	//mV 	->	0.1mmH2O
	press -= press_com;
	/* ERROR!!!! */
	if(press < -1000 || press >= 5000)	
		press = 0;

	taskENTER_CRITICAL();
	Run_Param.measure_p = (short)press;			//Record pressure.
	taskEXIT_CRITICAL();
	
	return press;
}





