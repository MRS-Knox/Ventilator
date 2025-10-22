#include "Middle_Timer.h"

/*!
	@brief 		 This initial function is about timer6.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
	@note		 The timer6 clock frequency is 90MHz.
*/
void Mid_Timer6_Init(void){
	Dri_Timer6Hardware_Init(90,10000);	//10ms
}


