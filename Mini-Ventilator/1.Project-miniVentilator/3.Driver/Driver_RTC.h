#ifndef Driver_RTC_H
#define Driver_RTC_H

#include "stm32f4xx_conf.h"

/* This initial function is about RTC hardware. */
void Dri_RTCHardware_Init(void);
/* This function is about to set RTC time. */
void Dri_Set_RTCTime(uint8_t *rtc_time);
/* This function is about to get RTC time. */
void Dri_Get_RTCTime(RTC_DateTypeDef *RTC_DateTypeDef_t,RTC_TimeTypeDef *RTC_TimeTypeDef_t);

#endif
