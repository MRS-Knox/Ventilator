#include "Middle_WatchDog.h"

/*!
	@brief 		 This initial function is about independent watch dog.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_IWDG_Init(){
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(1250);   //0~4095	About 1s
	IWDG_ReloadCounter();
	IWDG_Enable();
}

/*!
	@brief 		 Feed independent Watch Dog.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_Feed_IWDG(){
	IWDG_ReloadCounter();
}



