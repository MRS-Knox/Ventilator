#ifndef Middle_Voice_H
#define Middle_Voice_H

#include "Driver_Voice.h"
//#include "Middle_Timer.h"

/* Voice Initialization. */
void Mid_Voice_Init(void);
/* Control voice capacitance. */
void Mid_Set_CAP(BitAction cap_level);
/* Control voice power. */
void Mid_Set_Power(BitAction power_level);
/* Alarm voice broadcast. */
void Mid_Voice_Alarm(eAlarm_Priority alarm_value);

#endif /*Middle_Voice_H*/
