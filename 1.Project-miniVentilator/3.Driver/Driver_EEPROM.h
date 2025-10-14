#ifndef Driver_EEPROM_H
#define Driver_EEPROM_H

#include "stm32f4xx_conf.h"
#include "Add_Function.h"
#include <stdio.h>

#define EEPROM_EN_Pin		            GPIO_Pin_8
#define EEPROM_SCL_Pin		            GPIO_Pin_6
#define EEPROM_SDA_Pin		            GPIO_Pin_7
#define EEPROM_WC_Pin		            GPIO_Pin_14
#define EEPROM_WC_Group		            GPIOB
#define EEPROM_SCLSDAEN_Group	        GPIOC

#define EEPROM_SDA_OUT 	                Dri_EEPROM_SDA_State(0)
#define EEPROM_SDA_IN 	                Dri_EEPROM_SDA_State(1)
#define EEPROM_SCL_High  	            GPIO_WriteBit(EEPROM_SCLSDAEN_Group,EEPROM_SCL_Pin,Bit_SET)
#define EEPROM_SCL_Low   	            GPIO_WriteBit(EEPROM_SCLSDAEN_Group,EEPROM_SCL_Pin,Bit_RESET)
#define EEPROM_SDA_High  	            GPIO_WriteBit(EEPROM_SCLSDAEN_Group,EEPROM_SDA_Pin,Bit_SET)
#define EEPROM_SDA_Low   	            GPIO_WriteBit(EEPROM_SCLSDAEN_Group,EEPROM_SDA_Pin,Bit_RESET)
#define EEPROM_WC_High  	            GPIO_WriteBit(EEPROM_WC_Group,EEPROM_WC_Pin,Bit_SET)
#define EEPROM_WC_Low   	            GPIO_WriteBit(EEPROM_WC_Group,EEPROM_WC_Pin,Bit_RESET)

#define EEPROM_WRITE_CMD		        0xA0
#define EEPROM_READ_CMD			        0xA1

#define MIN_EEPROMADDRESS               0x0000
#define MAX_EEPROMADDRESS               0X0FFF

// #define MAX_ONEPAGE_COUNT               32
// #define FIRSTPAGE_STARTADDRESS          0x0000
// #define SECONDPAGE_STARTADDRESS         0x0020
// #define THIRDPAGE_STARTADDRESS          0x0040
// #define FOURTHPAGE_STARTADDRESS         0x0060
// #define FIFTHPAGE_STARTADDRESS          0x0080
// #define SIXTHPAGE_STARTADDRESS          0x0100
// #define SEVENTHPAGE_STARTADDRESS        0x0120
// #define EIGHTHPAGE_STARTADDRESS         0x0140
// #define NINETHPAGE_STARTADDRESS         0x0160
// #define TENTHPAGE_STARTADDRESS          0x0180


/* Initialize EEPROM SCL and EN and WC pin. */
void Dri_EEPROM_SCLENWC_Init(void);
/* Initialize EEPROM SDA pin. */
void Dri_EEPROM_SDA_State(uint8_t state);
/* IIC start sign. */
void EEPROM_Start(void);
/* IIC stop sign. */
void EEPROM_Stop(void);
/* IIC ACK. */
void EEPROM_Ack(void);
/* IIC wait ACK. */
void EEPROM_Wait_Ack(void);
/* IIC NACK. */
void EEPROM_No_Ack(void);
/* IIC write one byte. */
void EEPROM_Write_Byte(uint8_t data);
/* IIC read one byte. */
uint8_t EEPROM_Read_Byte(void);


#endif






