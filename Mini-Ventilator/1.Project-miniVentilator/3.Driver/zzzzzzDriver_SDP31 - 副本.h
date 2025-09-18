#ifndef Driver_SDP31_H
#define Driver_SDP31_H

#include "stm32f4xx_conf.h"
#include "Add_Function.h"
#include <stdio.h>

#define IIC2_SCL_Pin		GPIO_Pin_10
#define IIC2_SDA_Pin		GPIO_Pin_12
#define IIC2_SCL_Group		GPIOB
#define IIC2_SDA_Group		GPIOC

#define FLOW_SDA_OUT 	Dri_Flow_SDA_State(0)
#define FLOW_SDA_IN 	Dri_Flow_SDA_State(1)
#define FLOW_SCL_High  	GPIO_WriteBit(IIC2_SCL_Group,IIC2_SCL_Pin,Bit_SET)
#define FLOW_SCL_Low   	GPIO_WriteBit(IIC2_SCL_Group,IIC2_SCL_Pin,Bit_RESET)
#define FLOW_SDA_High  	GPIO_WriteBit(IIC2_SDA_Group,IIC2_SDA_Pin,Bit_SET)
#define FLOW_SDA_Low   	GPIO_WriteBit(IIC2_SDA_Group,IIC2_SDA_Pin,Bit_RESET)



/* The GZP6897D sensor parameter. */
#define Sensor_Address		0x6D
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


/* Initialize Flow SCL pin. */
void Dri_Flow_SCL_Init(void);
/* Initialize Flow SDA pin. */
void Dri_Flow_SDA_State(uint8_t state);
/* IIC start sign. */
void FLOW_Start(void);
/* IIC stop sign. */
void FLOW_Stop(void);
/* IIC ACK. */
void FLOW_Ack(void);
/* IIC wait ACK. */
void FLOW_Wait_Ack(void);
/* IIC NACK. */
void FLOW_No_Ack(void);
/* IIC write one byte. */
void FLOW_Write_Byte(uint8_t data);
/* IIC read one byte. */
uint8_t FLOW_Read_Byte(void);

// /* This initial function is about flow sensor hardware. */
// void Dri_FlowHardware_Init(uint8_t self_address,uint32_t iic_clock);
// /* This function is about main chip send data to flow sensor. */
// void Dri_FlowReadData(uint8_t *pdata,FlagStatus *flag_measurefinish);
// void Dri_ReadSDP3x(uint8_t *pdata,FlagStatus *flag_measurefinish);

#endif






