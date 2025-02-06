/********************************************************************************

 **** Copyright (C), 2019, Fortior Technology Co., Ltd.                      ****

 ********************************************************************************
    File Name     : UART.c
    Author        : Bruce HW&RD
    Date          : 2019-09-11
    Description   : .C file function description
    Version       : 1.0
    Function List :

    Record        :
    1.Date        : 2019-09-11
     Author      : Bruce HW&RD
     Modification: Created file

********************************************************************************/

#include "AddFunction.h"
#include "FU68xx_4_MCU.h"
#include "UART.h"

void UART2_Init(void)
{
    SetBit(PH_SEL, UART2EN);    //P0[1]as UART2_RXD; P0[0]as UART2_TXD

	ClrBit(P0_OE, P01);         //输入使能
    SetBit(P0_PU, P01);         //上拉电阻
    SetBit(P0_OE, P00);         //输出使能
    SetBit(P0_PU, P00);         //上拉电阻
	
    ClrBit(UT2_CR, UT2MOD1);    //00-->单线制8bit        01-->8bit uart(波特率可设置)
    SetBit(UT2_CR, UT2MOD0);    //10-->单线制9bit        11-->9bit uart(波特率可设置)
	
    ClrBit(UT2_CR, UT2SM2);     //0-->单机通讯          	1-->多机通讯；
	
    SetBit(UT2_CR, UT2REN);     //0-->不允许串行输入 	1-->允许串行输入，软件清0;
	
    ClrBit(UT2_CR, UT2TB8);     //模式2/3下数据发送第9位，在多机通信中，可用于判断当前数据帧的数据是地址还是数据，TB8=0为数据，TB8=1为地址
    ClrBit(UT2_CR, UT2RB8);     //模式2/3下数据接收第9位，若SM2=0,作为停止位
	
    ClrBit(UT2_BAUD, BAUD2_SEL); //倍频使能0-->Disable  1-->Enable
	
    SetBit(UT2_BAUD, UART2CH);   //UART2端口功能转移使能1    0：P36->RXD P37->TXD 1:P01->RXD P00->TXD
	
    SetBit(UT2_BAUD, UART2IEN);  //UART2中断使能0-->Disable  1-->Enable
	
    UT2_BAUD = 0x600C;           //波特率可设置 = 24000000/(16/(1+ UT_BAUD[BAUD_SEL]))/(UT_BAUD+1)    0x009B-->9600 0x000c-->115200 0x0005-->256000
	
	PSPI_UT20 = 1;
	PSPI_UT21 = 1;				 //UART2中断优先级为3	
}

unsigned char Send_CRC8(unsigned char dat[],unsigned char length)
{
	unsigned char crc = 0xFF;
	unsigned char count = 0;
	unsigned char crc_count = 0;
	for(count = 0;count < length;count++)
	{
		crc ^= dat[count];
		for(crc_count = 0;crc_count < 8;crc_count++)	//模二除法
		{ 
			if(crc & 0x80)
			{ 
				crc <<= 1;
				crc ^= 0x31;
			}
			else
			{
				crc <<= 1;
			}	
		}
	}
	return (crc^0x00);
}

unsigned char Check_CRC8(unsigned char dat[],unsigned char length)
{
	unsigned char crc = 0xFF;
	unsigned char count = 0;
	unsigned char crc_count = 0;
	for(count = 0;count < length;count++)
	{
		crc ^= dat[count];
		for(crc_count = 0;crc_count < 8;crc_count++)	//模二除法
		{ 
			if(crc & 0x80)
			{ 
				crc <<= 1;
				crc ^= 0x31;
			}
			else
			{
				crc <<= 1;
			}	
		}
	}
	return (crc^0x00);
}


unsigned char Motor_Return(void){
	static unsigned char flag_poweron = 0;
	static unsigned char flag_error = 0;
	static unsigned char error_200mstick = 0;
	/* 刚通电返回 0x0011 */
	if(0x0001 == blower_state_t.rec_command)
	{
		Usart2_SendValue(DATATYPE_COMMAND,POWER_ON);
		blower_state_t.rec_command = 0x0;
		flag_poweron = 1;
		return 1;
	}
	if(flag_poweron == 0)
		return 0;
	/* 发送实际转速 */
	Usart2_SendValue(DATATYPE_DATA,blower_state_t.actual_speed);
	/* 电机有故障,两秒后故障还没消除,则上报主芯片 */
    if(mcFaultSource != FaultNoSource && flag_error == 0)	
	{
		error_200mstick++;
		if(error_200mstick >= 20)
		{	
			flag_error = 1;
			Usart2_SendValue(DATATYPE_COMMAND,BLOWER_ERROR);	//电机报错
		}
	}
	return 0;
}

void UART2_ReciveData(void)
{
	unsigned char agroup_data[7] 	= {0};
	unsigned char agroup_count 		= 0;
	unsigned char flag_headertrue 	= 0;
	//uart2_data_t.rec_outcount point to frame header.
	while(uart2_data_t.uart2_recdata[uart2_data_t.rec_outcount] != 0x09){
		if(uart2_data_t.rec_outcount == uart2_data_t.rec_incount)
			return;
		uart2_data_t.rec_outcount++;
		if(uart2_data_t.rec_outcount >= UART_MAX) 
			uart2_data_t.rec_outcount = 0;
	}
	//No data.
	if(uart2_data_t.rec_outcount == uart2_data_t.rec_incount)
		return;
	//Copy uart2_data_t.uart2_recdata to agroup_data.
	for(agroup_count = 0;agroup_count < 7;agroup_count++)	
	{
		agroup_data[agroup_count] = uart2_data_t.uart2_recdata[uart2_data_t.rec_outcount++];
		if(uart2_data_t.rec_outcount >= UART_MAX)
			uart2_data_t.rec_outcount = 0;
	}
	//Judge CRC and frame end.
	if((Check_CRC8(agroup_data,4) == agroup_data[4]) && (agroup_data[5] == 0x01) && (agroup_data[6] == 0x23))
	{
		if(agroup_data[1] == DATATYPE_DATA)	//Data
		{
			blower_state_t.set_speed = (agroup_data[2] << 8) | agroup_data[3];
		}
		else if(agroup_data[1] == DATATYPE_COMMAND) //Command
		{
			blower_state_t.rec_command = (agroup_data[2] << 8) | agroup_data[3];
		}
	}
}

void Usart2_SendValue(uint8 data_type,uint16 send_command)
{
	unsigned char send_databuff[7];
	unsigned char i;
	send_databuff[0] = 0x09;		//Frame begin.
	send_databuff[1] = data_type;	//Send data type. 
	send_databuff[2] = (send_command >> 8);	
	send_databuff[3] = send_command;
	send_databuff[4] = Send_CRC8(send_databuff,4);	
	send_databuff[5] = 0x01;	//Frame end.	
	send_databuff[6] = 0x23;		
	for(i = 0;i < 7;i++){
		uart2_data_t.uart2_senddata[uart2_data_t.send_incount++] = send_databuff[i];
		if(uart2_data_t.send_incount >= UART_MAX)
			uart2_data_t.send_incount = 0;
	}
	//Triggle transmisson interrupt.
	UT2_DR = uart2_data_t.uart2_senddata[uart2_data_t.send_outcount++];
	if(uart2_data_t.send_outcount >= UART_MAX)
		uart2_data_t.send_outcount = 0;
}
