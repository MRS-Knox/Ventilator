#include "Middle_Alarm.h"

/* The highest priority alarm. */
void Mid_HighPriority_Alarm(void);

/*!
	@brief 		 Machine alarm.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_MachineAlarm(void *pvParameter){
	EventBits_t event_bit = 0;
    while(1){
		event_bit = xEventGroupWaitBits(MachineStateEvent_Handle,Machine_On_Event,pdFALSE,pdFALSE,0);
		/* -------------------------- TEST ALARM ------------------------------ */
		/* Machine is running now. */
		if((event_bit&Machine_On_Event) == Machine_On_Event){
			/* Test the highest priority alarm and the middle priority alarm. */
			Mid_TestHighPriority();
			Mid_TestMiddlePriority();
		}
		/* Test the lowest priority alarm and run cycle is 30s. */
		Mid_TestLowPriority();

		/* -------------------------- ALARM VOICE ------------------------------ */
		if(Machine_State.Alarm_State.flag_highpri == SET){
			Mid_HighPriority_Alarm();
		}
		else if(Machine_State.Alarm_State.flag_middlepri == SET){
			Mid_Voice_Alarm(middle_pri);
		}
		else if(Machine_State.Alarm_State.flag_lowpri == SET){
			Mid_Voice_Alarm(low_pri);
		}
		else{
			/* No alarm. */
			Machine_State.flag_alarmvoice_state = RESET;
			Machine_State.flag_voicealarm_first = RESET;
		}

		vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/*!
	@brief 		 Test highest priority.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_TestHighPriority(){
	static uint8_t poweroffalarm_count = 0;
	/* The mask falls off alarm when the machine is running now. */
	// if(Machine_State.Alarm_State.flag_maskfall == SET && Machine_State.flag_machine_onoff == SET){

	// }
	/* Power off alarm when the machine is running now. */
	if(Mid_TestPower() == RESET){
		if(poweroffalarm_count++ >= 10){		//5*50ms
			poweroffalarm_count = 0;
			Machine_State.Alarm_State.flag_poweroff = SET;
		}	
	}
	else{
		poweroffalarm_count = 0;
	}
	/* Update the priority flag of highest priority alarm. */
	if(Machine_State.Alarm_State.flag_poweroff == SET || Machine_State.Alarm_State.flag_maskfall == SET)
		Machine_State.Alarm_State.flag_highpri = SET;
	else
		Machine_State.Alarm_State.flag_highpri = RESET;
}

/*!
	@brief 		 Test middle priority.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_TestMiddlePriority(){
	/* High leakage alarm when the machine is running now. */
	// if(Machine_State.Alarm_State.flag_highleak == SET && Machine_State.flag_machine_onoff == SET){
		// Machine_State.Alarm_State.flag_middlepri = SET;
	// }	

}

/*!
	@brief 		 Test lowest priority.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_TestLowPriority(){
	static uint16_t low_count = 0;
	if(low_count++ >= (1800000/50)){	//1800000ms -- 30min
		low_count = 0;
		Mid_CALConsumable_ChangeTime();
	}
}

/*!
	@brief 		 The highest priority alarm.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_HighPriority_Alarm(){
	FlagStatus flag_set_rgb = SET;
	uint16_t time_count = 0;
	uint8_t count = 0;
	/* Power off alarm. */
	if(Machine_State.Alarm_State.flag_poweroff == SET){
		/* Turn off the machine. */
		xEventGroupSetBits(MachineStateEvent_Handle,Machine_Off_Event);
		Machine_State.flag_machine_onoff = RESET;
		Mid_SendToBlower(SEND_RPM,0);
		/* Suspened other tasks. */
//		vTaskSuspend(MachineRunTaskHandle);
//		vTaskSuspend(TestMaskTaskHandle);
//		vTaskSuspend(FlowTaskHandle);
		while(1){
			/* Check power. */
			if(Mid_TestPower() == SET){
				if(count++ >= 5){
					//Reset chip.
				}
			}
			/* Sound-light alarm. */
			Mid_Voice_Alarm(high_pri);
			Mid_RGB_Flash(R_Light,2,50,flag_set_rgb);
			flag_set_rgb = RESET;
			vTaskDelay(pdMS_TO_TICKS(50));
			if(time_count++ >= 30000/50){	//30s
				time_count = 0;
				count = 0;
				while(1){
					/* Close voice and light. */
					if(RESET == Dri_Get_VoiceBusy()) 
						Mid_RGB_Close();
					/* Check power. */
					if(Mid_TestPower() == SET){
						if(count++ >= 5){
							//Reset chip.
						}
					}
				}
			}
		}
	}

}

/*!
	@brief 		 Calculate the usage time of consumables whether over the setting time of consumables.
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_CALConsumable_ChangeTime(){
	uint8_t rtc_time_buff[4];
	/* Update the set limit time of consumables. */
	RTC_DateTypeDef	RTC_Date_t;
	RTC_TimeTypeDef	RTC_Time_t;
	Dri_Get_RTCTime(&RTC_Date_t,&RTC_Time_t);
	rtc_time_buff[0] = RTC_Date_t.RTC_Year;	//From 2000 year.
	rtc_time_buff[1] = RTC_Date_t.RTC_Month;
	rtc_time_buff[2] = RTC_Date_t.RTC_Date;
	rtc_time_buff[3] = RTC_Time_t.RTC_Hours;

	if(Machine_State.flag_set_airfilter == SET){
		Machine_State.flag_set_airfilter = RESET;
		Machine_Time.airfilter_warn_hour = 0;
		SetConsumable_Warn.set_airfilter_date[0] = RTC_Date_t.RTC_Year;	//From 2000 year.
		SetConsumable_Warn.set_airfilter_date[1] = RTC_Date_t.RTC_Month;
		SetConsumable_Warn.set_airfilter_date[2] = RTC_Date_t.RTC_Date;
		SetConsumable_Warn.set_airfilter_date[3] = RTC_Time_t.RTC_Hours;
	}
	if(Machine_State.flag_set_maintenance == SET){
		Machine_State.flag_set_maintenance = RESET;
		Machine_Time.maintenace_warn_hour = 0;
		SetConsumable_Warn.set_maintenance_date[0] = RTC_Date_t.RTC_Year;	//From 2000 year.
		SetConsumable_Warn.set_maintenance_date[1] = RTC_Date_t.RTC_Month;
		SetConsumable_Warn.set_maintenance_date[2] = RTC_Date_t.RTC_Date;
		SetConsumable_Warn.set_maintenance_date[3] = RTC_Time_t.RTC_Hours;
	}
	if(Machine_State.flag_set_mask == SET){
		Machine_State.flag_set_mask = RESET;
		Machine_Time.mask_warn_hour = 0;
		SetConsumable_Warn.set_mask_date[0] = RTC_Date_t.RTC_Year;	//From 2000 year.
		SetConsumable_Warn.set_mask_date[1] = RTC_Date_t.RTC_Month;
		SetConsumable_Warn.set_mask_date[2] = RTC_Date_t.RTC_Date;
		SetConsumable_Warn.set_mask_date[3] = RTC_Time_t.RTC_Hours;
	}
	if(Machine_State.flag_set_mfc == SET){
		Machine_State.flag_set_mfc = RESET;
		Machine_Time.mfc_warn_hour = 0;
		SetConsumable_Warn.set_mfc_date[0] = RTC_Date_t.RTC_Year;	//From 2000 year.
		SetConsumable_Warn.set_mfc_date[1] = RTC_Date_t.RTC_Month;
		SetConsumable_Warn.set_mfc_date[2] = RTC_Date_t.RTC_Date;
		SetConsumable_Warn.set_mfc_date[3] = RTC_Time_t.RTC_Hours;
	}
	if(Machine_State.flag_set_pipe == SET){
		Machine_State.flag_set_pipe = RESET;
		Machine_Time.pipe_warn_hour = 0;
		SetConsumable_Warn.set_pipe_date[0] = RTC_Date_t.RTC_Year;	//From 2000 year.
		SetConsumable_Warn.set_pipe_date[1] = RTC_Date_t.RTC_Month;
		SetConsumable_Warn.set_pipe_date[2] = RTC_Date_t.RTC_Date;
		SetConsumable_Warn.set_pipe_date[3] = RTC_Time_t.RTC_Hours;
	}
	/* Update the usage time of consumables. */
	Machine_Time.airfilter_warn_hour 	= CALHour_BetweenTwoTime(SetConsumable_Warn.set_airfilter_date,		rtc_time_buff);
	Machine_Time.maintenace_warn_hour 	= CALHour_BetweenTwoTime(SetConsumable_Warn.set_maintenance_date,	rtc_time_buff);
	Machine_Time.mask_warn_hour 		= CALHour_BetweenTwoTime(SetConsumable_Warn.set_mask_date,			rtc_time_buff);
	Machine_Time.mfc_warn_hour 			= CALHour_BetweenTwoTime(SetConsumable_Warn.set_mfc_date,			rtc_time_buff);
	Machine_Time.pipe_warn_hour 		= CALHour_BetweenTwoTime(SetConsumable_Warn.set_pipe_date,			rtc_time_buff);
	/* Check whether the usage time of the consumables have exceeded the set limit. */
	if((Machine_Time.airfilter_warn_hour/24) >= SetConsumable_Warn.airfilter_warn_day){
		Machine_State.Alarm_State.flag_changeairfilter = SET;
	}
	if((Machine_Time.maintenace_warn_hour/24) >= SetConsumable_Warn.clearmachine_warn_day){
		Machine_State.Alarm_State.flag_maintenance = SET;
	}
	if((Machine_Time.mask_warn_hour/24) >= SetConsumable_Warn.mask_warn_day){
		Machine_State.Alarm_State.flag_changemask = SET;
	}
	if((Machine_Time.mfc_warn_hour/24) >= SetConsumable_Warn.mfc_warn_day){
		Machine_State.Alarm_State.flag_changemfc = SET;
	}
	if((Machine_Time.pipe_warn_hour/24) >= SetConsumable_Warn.pipe_warn_day){
		Machine_State.Alarm_State.flag_changepipe = SET;
	}
	/* Update the priority flag of lowest priority alarm. */
	if(Machine_State.Alarm_State.flag_changeairfilter == SET || Machine_State.Alarm_State.flag_maintenance == SET || \
	   Machine_State.Alarm_State.flag_changemask == SET || Machine_State.Alarm_State.flag_changemfc == SET || \
	   Machine_State.Alarm_State.flag_changepipe == SET)
		Machine_State.Alarm_State.flag_lowpri = SET;
	else
		Machine_State.Alarm_State.flag_lowpri = RESET;


}




