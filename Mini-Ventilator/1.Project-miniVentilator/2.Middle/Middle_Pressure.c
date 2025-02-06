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
void Mid_CalculatePRESS(void *pvPatameter){
	float press_volatage = 0.0f;
	while(1){
		press_volatage = Dri_ReadPRESS_V()*3300.0f/4095.0f; 	//ADC	->	mV
		press_volatage = 2.011f*press_volatage - 394.2f; 		//mV 	->	Pa
		Run_Param.press_data = press_volatage * 1.01974f; 		//Pa	->	0.1mmH2O
		/* ERROR!!!! */
		if(Run_Param.press_data < -20 || Run_Param.press_data >= 5000)	
			Run_Param.press_data = 0;
		
		vTaskDelay(pdTICKS_TO_MS(5));
	}
}

