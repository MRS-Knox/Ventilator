#include "Add_Function.h"

/*!
	@brief 		 Send to blower CRC-8,CRC polynomial is 0x131,intial number is 0xFF.
	@param[in]	 data[]:data bit
				 lenth:data count
	@param[out]	 none
	@retval		 crc:one byte crc
*/
unsigned char Send_BlowerCRC8(unsigned char data[],unsigned char length){
	unsigned char crc = 0xFF;
	for(unsigned char count = 0;count < length;count++){
		crc ^= data[count];
		for(unsigned char crc_count = 0;crc_count < 8;crc_count++){ 
			if(crc & 0x80){ 
				crc <<= 1;
				crc ^= 0x31;
			}
			else{
				crc <<= 1;
			}	
		}
	}
	return (crc^0x00);
}

/*!
	@brief 		 Check to blower CRC-8,CRC polynomial is 0x131,intial number is 0xFF.
	@param[in]	 data[]:data bit
				 lenth:data count
	@param[out]	 none
	@retval		 crc:one byte crc
*/
unsigned char Check_BlowerCRC8(unsigned char data[],unsigned char length){
	unsigned char crc = 0xFF;
	for(unsigned char count = 0;count < length;count++){
		crc ^= data[count];
		for(unsigned char crc_count = 0;crc_count < 8;crc_count++){ 
			if(crc & 0x80){ 
				crc <<= 1;
				crc ^= 0x31;
			}
			else{
				crc <<= 1;
			}	
		}
	}
	return (crc^0x00);
}

/*!
	@brief 		 Short int 16bits average filter.
	@param[in]	 pdata:point need calculate array
				 count:need calculate count
	@param[out]	 none
	@retval		 average_data:average calculate data
*/
unsigned short int Average_Filter_Int(unsigned short int* pdata,unsigned char count){
	unsigned short int max_value = 0;
	unsigned short int min_value = pdata[0];
	unsigned int average_data = 0;
	max_value = pdata[0];
	min_value = pdata[0];
	for(unsigned char i = 0;i < count;i++){
		if(max_value < pdata[i]) 
			max_value = pdata[i];
		if(min_value > pdata[i])
			min_value = pdata[i];
	}
	for(unsigned char i = 0;i < count;i++)	
		average_data += pdata[i];
	average_data = (average_data-(max_value+min_value)) / (count-2);
	return average_data;
}






/*****					Delay functions(180MHz).							*****/
void udelay_4us(unsigned short count){
	for(unsigned short i = 0;i < count;i++)
		__NOP();
}

void udelay_1ms(unsigned short count){
	for(unsigned short i = 0;i < count;i++)
		for(unsigned short j = 0;j < 25050;j++)
			__NOP();
}
