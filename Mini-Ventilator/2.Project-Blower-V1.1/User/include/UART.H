#ifndef __UART_H__
#define __UART_H__

/*************************************************************************************///External Function

void UART2_Init(void);
void UART2_ReciveData(void);
unsigned char Motor_Return(void);
//void put_char(unsigned char c);
void Usart2_SendValue(uint8 data_type,uint16 command_data);
void put_string(unsigned char *str);

#define DATATYPE_DATA			0X01
#define DATATYPE_COMMAND		0XFF
#define POWER_ON				0X0011
#define BLOWER_ERROR			0X0012


#endif




