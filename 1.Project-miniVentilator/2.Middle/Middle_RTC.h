#ifndef Middle_RTC_H
#define Middle_RTC_H

#include "Driver_RTC.h"

/* This initial function is about RTC hardware. */
void Mid_RTC_Init(void);
/* Set real-time clock. */
void Mid_Set_RTC(uint8_t *prtc_time);
/* Get real-time clock. */
void Mid_Get_RTC(RTC_DateTypeDef *RTC_DateTypeDef_t,RTC_TimeTypeDef *RTC_TimeTypeDef_t);


#endif




