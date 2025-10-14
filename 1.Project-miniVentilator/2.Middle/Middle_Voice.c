#include "Middle_Voice.h"

/*
    @brief        CPU send data to voice.
    @param[in]    voice_data:voice need broadcast content
    @param[out]   none
    @retval       none
*/
void Mid_Voice_SendData(eVoice_Content voice_data){
	eVoice_Content value = voice_data;
    
	Mid_Voice_Start();  
    for(uint8_t i = 0;i < 8;i++){
        if(value & 0x01)      
            Dri_Voice_DataOne();
        else 
            Dri_Voice_DataZero();
        value >>= 1;
    }	
    Mid_Voice_Stop();
}

/*
    @brief        Voice Initialization.
    @param[in]    none
    @param[out]   none
    @retval       none
*/
void Mid_Voice_Init(){
	Dri_Voice_GPIO_Init();
	Dri_Set_CAP(Bit_SET);
	Dri_Set_Power(Bit_RESET);	//Power on.
    Mid_Voice_SendData(max_volume);
	udelay_100us(2);
}

/*
    @brief        Control voice capacitance.
    @param[in]    cap_level:....
    @param[out]   none
    @retval       none
*/
void Mid_Set_CAP(BitAction cap_level){
	Dri_Set_CAP(cap_level);
}

/*
    @brief        Control voice power.
    @param[in]    power_level:....
    @param[out]   none
    @retval       none
*/
void Mid_Set_Power(BitAction power_level){
	Dri_Set_Power(power_level);
}

/*
    @brief        Alarm voice broadcast.
    @param[in]    Alarm_Priority:high_pri or middle_pri or low_pri
    @param[out]   none
    @retval       none
    @note         Run cycle is 50ms.
*/
void Mid_Voice_Alarm(eAlarm_Priority alarm_value){
	int run_cycle = 50;    
    static uint16_t alarm_timecount = 0;
	static eAlarm_Priority last_alarmvalue = none_alarm;
	/* Clear alarm_timecount when alarm priority changes. */
	if(last_alarmvalue != alarm_value){
		alarm_timecount = 0;
		last_alarmvalue = alarm_value;
		Machine_State.flag_voicealarm_first = SET;
	}
    switch(alarm_value){
        case high_pri:
			if(Machine_State.flag_voicealarm_first){
				Machine_State.flag_voicealarm_first = RESET;
				Mid_Voice_SendData(high_alarm);
				return;
			}
            if(RESET == Dri_Get_VoiceBusy()){     //Voice chip stop work.
                alarm_timecount++;
                if(alarm_timecount >= (2500/run_cycle)){ //2.5s
                    alarm_timecount = 0;
                    Mid_Voice_SendData(high_alarm);
                }
            }
            break;
        case middle_pri:
			if(Machine_State.flag_voicealarm_first){
				Machine_State.flag_voicealarm_first = RESET;
				Mid_Voice_SendData(middle_alarm);
				return;
			}
            if(RESET == Dri_Get_VoiceBusy()){     //Voice chip stop work.
                alarm_timecount++;
                if(alarm_timecount >= (5000/run_cycle)){     //5s
                    alarm_timecount = 0;
                    Mid_Voice_SendData(middle_alarm);
                }
            }
            break;
        case low_pri:
			if(Machine_State.flag_voicealarm_first){
				Machine_State.flag_voicealarm_first = RESET;
				Mid_Voice_SendData(low_alarm);
				return;
			}
            if(RESET == Dri_Get_VoiceBusy()){     //Voice chip stop work.
                alarm_timecount++;
                if(alarm_timecount >= (20000/run_cycle)){     //20s
                    alarm_timecount = 0;
                    Mid_Voice_SendData(low_alarm);
                }
            }
            break;
        default:
			alarm_value = none_alarm;
			last_alarmvalue = none_alarm;
			alarm_timecount = 0;
            break;
    }
}
	



