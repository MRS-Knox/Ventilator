#include "Middle_RTC.h"

/*!
	@brief 		 This initial function is about RTC hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_RTC_Init(void){ 
	Dri_RTCHardware_Init();
} 

/*!
	@brief 		 Set real-time clock.
	@param[in]	 prtc_time:year,month,day,week,hour,minute,second,AM or PM.
	@param[out]	 none
	@retval		 none
*/
void Mid_Set_RTC(uint8_t *prtc_time){ 
	Dri_Set_RTCTime(prtc_time);
} 

/*!
	@brief 		 Get real-time clock.
	@param[in]	 none
	@param[out]	 RTC_DateTypeDef_t:year,month,date,weekday.
				 RTC_TimeTypeDef_t:hour,minute,second,H12 or H24.
	@retval		 none
*/
void Mid_Get_RTC(RTC_DateTypeDef *RTC_DateTypeDef_t,RTC_TimeTypeDef *RTC_TimeTypeDef_t){  
	Dri_Get_RTCTime(RTC_DateTypeDef_t,RTC_TimeTypeDef_t);
}



