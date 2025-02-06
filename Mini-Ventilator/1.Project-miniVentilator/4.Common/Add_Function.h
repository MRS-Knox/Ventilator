#ifndef Add_Function_H
#define Add_Function_H

#include "stm32f4xx_conf.h"


/* Blower CRC. */
unsigned char Send_BlowerCRC8(unsigned char data[],unsigned char length);
unsigned char Check_BlowerCRC8(unsigned char data[],unsigned char length);
/* Pressure filter. */
unsigned short int Average_Filter_Int(unsigned short int* pdata,unsigned char count);

/* Delay functions. */
void udelay_4us(unsigned short count);
void udelay_1ms(unsigned short count);


#endif 
