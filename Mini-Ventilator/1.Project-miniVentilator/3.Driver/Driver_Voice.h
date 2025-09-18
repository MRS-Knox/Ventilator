#ifndef Driver_Voice_H
#define Driver_Voice_H

#include "stm32f4xx_conf.h"
#include "Add_Function.h"

#define Data_Pin			GPIO_Pin_6
#define Busy_Pin			GPIO_Pin_7
#define DataBusy_Group		GPIOB
#define CAP_Pin				GPIO_Pin_3
#define CAP_Group			GPIOB
#define Power_Pin			GPIO_Pin_13
#define Power_Group			GPIOC


/* Voice GPIO Init. */
void Dri_Voice_GPIO_Init(void);
/* Voice data port. */
void Dri_Set_Data(BitAction data_level);
/* Voice need a start sign. */
void Mid_Voice_Start(void);
/* Voice need a stop sign. */
void Mid_Voice_Stop(void);
/* Voice data set 1. */
void Dri_Voice_DataOne(void);
/* Voice data set 0. */
void Dri_Voice_DataZero(void);
/* Read voice busy level. */
FlagStatus Dri_Get_VoiceBusy(void);
/* Control capture IO set high. */
void Dri_Set_CAP(BitAction cap_level);
/* Control power I/O set high. */
void Dri_Set_Power(BitAction power_level);


#endif 
