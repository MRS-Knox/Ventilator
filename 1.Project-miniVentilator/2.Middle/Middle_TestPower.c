#include "Middle_TestPower.h"


/*!
	@brief 		 Initialize test power.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_TestPower_Init(){	
	Dri_TestPowerHardware_Init();
}

/*!
	@brief 		 Test power.
	@param[in]	 none
	@param[out]	 none
	@retval		 SET or RESET
*/
FlagStatus Mid_TestPower(){	
	return Dri_TestPB1_State();
}



