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

#define SDP31_WRITE_CMD				(0x21<<1)			//0 is write
#define SDP31_READ_CMD				((0x21<<1)|0x01)    //1 is read
#define MASS_FLOW_CONTINUOUS		0x3608 				//start continuous mode and data is mass flow
#define DIFF_PRESS_CONTINUOUS		0x3615 				//start continuous mode and data is difference pressure	
#define STOP_CONTINUOUS_MODE 		0X3FF9 				//stop continuous mode 


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


#endif






