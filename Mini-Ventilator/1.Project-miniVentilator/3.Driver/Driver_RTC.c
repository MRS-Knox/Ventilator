#include "Driver_RTC.h"

/*!
	@brief 		 This initial function is about RTC hardware.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Dri_RTCHardware_Init(void){  
	RTC_InitTypeDef 	RTC_Init_t;	
    /* The RTC initialization. */
    PWR_BackupAccessCmd(ENABLE);
	/* Enable LSE clock(32.768KHz). */
	RCC_LSEConfig(RCC_LSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);	
	RTC_WaitForSynchro();
	RTC_Init_t.RTC_HourFormat	= RTC_HourFormat_24;
	RTC_Init_t.RTC_AsynchPrediv = 128 - 1;
	RTC_Init_t.RTC_SynchPrediv	= 256 - 1;
	RTC_Init(&RTC_Init_t);
}

/*!
	@brief 		 This function is about to set RTC time.
	@param[in]	 *rtc_time:year,month,day,week,hour,minute,second,AM or PM 
	@param[out]	 none
	@retval		 none
*/
void Dri_Set_RTCTime(uint8_t *rtc_time){  
	RTC_TimeTypeDef		RTC_Time_t;
	RTC_DateTypeDef		RTC_Date_t;
	
	RTC_Date_t.RTC_Year			= rtc_time[0];		//From 2000 years.
	RTC_Date_t.RTC_Month		= rtc_time[1];
	RTC_Date_t.RTC_Date			= rtc_time[2];	
	RTC_Date_t.RTC_WeekDay		= rtc_time[3];
	RTC_SetDate(RTC_Format_BIN,&RTC_Date_t);
	RTC_WaitForSynchro();
	 
	RTC_Time_t.RTC_Hours		= rtc_time[4];
	RTC_Time_t.RTC_Minutes		= rtc_time[5];
	RTC_Time_t.RTC_Seconds		= rtc_time[6];
	if(rtc_time[7] == 0)
		RTC_Time_t.RTC_H12			= RTC_H12_AM;	//24 Hours
	else if(rtc_time[7] == 1)
		RTC_Time_t.RTC_H12			= RTC_H12_PM;	//12 Hours
	RTC_SetTime(RTC_Format_BIN,&RTC_Time_t);
	RTC_WaitForSynchro();
}


/*!
	@brief 		 This function is about to get RTC time.
	@param[in]	 none
	@param[out]	 RTC_DateTypeDef_t:year,month,date,weekday.
				 RTC_TimeTypeDef_t:hour,minute,second,H12 or H24.
	@retval		 none
*/
void Dri_Get_RTCTime(RTC_DateTypeDef *RTC_DateTypeDef_t,RTC_TimeTypeDef *RTC_TimeTypeDef_t){  
	RTC_GetDate(RTC_Format_BIN,RTC_DateTypeDef_t);
	RTC_GetTime(RTC_Format_BIN,RTC_TimeTypeDef_t);
}




