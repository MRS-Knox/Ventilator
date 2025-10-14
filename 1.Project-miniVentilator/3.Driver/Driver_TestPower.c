#include "Driver_TestPower.h"

/*!
	@brief 		 Initialize test power hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_TestPowerHardware_Init(){	
	GPIO_InitTypeDef 	TestPower_GPIO_Init;
	
	TestPower_GPIO_Init.GPIO_Pin 	= TestPower_Pin; 
	TestPower_GPIO_Init.GPIO_Mode 	= GPIO_Mode_IN;
	// TestPower_GPIO_Init.GPIO_Speed 	= GPIO_Speed_50MHz;
	// TestPower_GPIO_Init.GPIO_OType 	= GPIO_OType_OD;
	TestPower_GPIO_Init.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(TestPower_Group,&TestPower_GPIO_Init);
}

/*!
	@brief 		 Test PB1 status.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
FlagStatus Dri_TestPB1_State(){	    
    return ((FlagStatus)GPIO_ReadInputDataBit(TestPower_Group,TestPower_Pin));  
}


