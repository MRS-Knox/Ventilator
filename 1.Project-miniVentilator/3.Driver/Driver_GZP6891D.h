#ifndef Driver_GZP6891D_H
#define Driver_GZP6891D_H

#include "stm32f4xx_conf.h"
#include "Add_Function.h"
#include <stdio.h>

#define IIC2_SCL_Pin		GPIO_Pin_10
#define IIC2_SDA_Pin		GPIO_Pin_12
#define IIC2_SCL_Group		GPIOB
#define IIC2_SDA_Group		GPIOC

/* The GZP6897D sensor parameter. */
#define Sensor_Address		0x7F
#define DATA_MSB			0x06
#define DATA_CSB			0x07
#define DATA_LSB			0x08
#define TEMP_MSB			0x09
#define TEMP_LSB			0x0A
#define CMD_Address			0x30
#define SYS_Config			0xA5
#define PRESS_Config		0xA6
#define AllMeasure_Mode		0x0A
#define Sensor_K			8192

/* This initial function is about flow sensor hardware. */
void Dri_FlowHardware_Init(uint8_t self_address,uint32_t iic_clock);
/* This function is about main chip send data to flow sensor. */
void Dri_FlowReadData(uint8_t *pdata,FlagStatus *flag_measurefinish);


#endif


