#ifndef Add_Function_H
#define Add_Function_H

#include "stm32f4xx_conf.h"
#include "FreeRTOS.h"
#include "Use_Struct.h"
#include "stdint.h"


/* CRC. */
unsigned char Send_BlowerCRC8(unsigned char data[],unsigned char length);
unsigned char Check_BlowerCRC8(unsigned char data[],unsigned char length);
unsigned char Check_SensorCRC8(unsigned char data[],unsigned char length);
/* Pressure filter. */
unsigned short int Average_Filter_Int(unsigned short int* pdata,unsigned char count);
/* Kalman filter. */
float Kalman_Filter(KalmanFilter_t* pstruct,float measure_data);
/* Low frequency pass filter. */
float LowPass_Filter(float k,float measure_data,float last_value);


/* Data fitting algorithm. */
void LeastSquareMethod(unsigned int datax[],unsigned short datay[],unsigned int n,float *pk,float *pb);


/* Caculate data complement. */
unsigned short Calculate_DataComplement(unsigned short data);

/* Find maximum value and minimum value. */
void Find_MIN_MAX(int *pdata_buff,unsigned char count,int *pmax_data,int *pmin_data);
/* Move right range. */
void MoveRight_Range(int *pdata_buff,unsigned short count,int data);
/* Calculate the number of hours between two times. */
unsigned int CALHour_BetweenTwoTime(unsigned char *pstart_time,unsigned char *pstop_time);

/* Delay functions. */
void udelay_1us(unsigned short count);
void udelay_100us(unsigned short count);


#endif 
