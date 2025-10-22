#include "Add_Function.h"

/*!
	@brief 		 Send to blower CRC-8,CRC polynomial is 0x131,intial number is 0xFF.
	@param[in]	 data[]:data bit
				 length:data count
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
	@brief 		 Check to sensor CRC-8,CRC polynomial is 0x131,intial number is 0xFF.
	@param[in]	 data[]:data bit
				 length:data count
	@param[out]	 none
	@retval		 crc:one byte crc
*/
unsigned char Check_SensorCRC8(unsigned char data[],unsigned char length){
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

/*!
	@brief 		 Average filter(Flow).
	@param[in]	 pdata:point need calculate array
				 count:need calculate count
	@param[out]	 none
	@retval		 average_data:average calculate data
*/
int FlowAverage_Filter(int* pdata,unsigned int count){
	long int data_sum = 0;
	int return_value = 0;

	for(unsigned int i = 0;i < count;i++)	
		data_sum += pdata[i];
	return_value = data_sum / (int)count;

	return return_value;
}

/*
	@brief 		 Kalman Filter 
	@param[in]	 pstruct:point to struct what need use kalman filter
				 data:measure data
	@param[out]	 none
	@retval		 pstruct->x_last:optimal value 
*/
float Kalman_Filter(KalmanFilter_t* pstruct,float measure_data){
//	float x_now = pstruct->x_last;	//caculate now value
	float limit = 1.0f;
	
	if((measure_data-pstruct->x_last > limit) || (pstruct->x_last-measure_data > limit)){
		pstruct->q = pstruct->q>8.0f ? 8.0f : pstruct->q+0.9f;
		pstruct->r = pstruct->r<0.01f ? 0.01f : pstruct->r-1.0f;
//		pstruct->q += 0.9f;
//		pstruct->r -= 1.0f;
	}
	else{
		pstruct->q = 0.08f;
		pstruct->r = 5.0f;
	}
	
	float p_now = pstruct->p_last + pstruct->q;	//update now covariance
	
	float k = p_now/(p_now + pstruct->r);	//update Kk
	
	pstruct->x_last = pstruct->x_last + k*(measure_data - pstruct->x_last);	//update x value
	
	pstruct->p_last = (1 - k) * p_now; //update last covariance
	
	return pstruct->x_last;
}

/*
	@brief 		 Low frequency pass filter.y = k * (measure - last_cal) + last_cal.
	@param[in]	 k:low pass filter coefficient
				 measure_data:measure data
	@param[out]	 none
	@retval		 output:optimal value 
*/
float LowPass_Filter(float k,float measure_data,float last_value){
	float output = k * (measure_data - last_value) + last_value;
	return output;
}


/*!
	@brief 		 Least square method.
	@param[in]	 datax[]:Internal sensor measures data.
				 datay[]:External equipment measures data.
				 n:data number.
	@param[out]	 pk,pb --> y = k*x + b.
	@retval		 none
*/
void LeastSquareMethod(unsigned int datax[],unsigned short datay[],unsigned int n,float *pk,float *pb){
	float sum_x = 0.0f;
	float sum_x2 = 0.0f;
	float sum_y  = 0.0f;
	float sum_xy = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	
	for(unsigned int i = 0;i < n;i++){
		x = (float)datax[i]/100.0f;
		y = (float)datay[i]/100.0f;
		sum_x 	+= x;
		sum_x2 	+= (x * x);
		sum_y 	+= y;
		sum_xy 	+= (x * y);
	}

	*pk = (float)(n*sum_xy - sum_x*sum_y) / (n*sum_x2 - sum_x*sum_x);
	x = *pk;
	*pb = (float)(sum_y - x*sum_x) / n;
}

/*!
	@brief 		 Caculate data complement 
	@param[in]	 data:point need calculate data
	@param[out]	 none
	@retval		 value:data complement 
*/
unsigned short Calculate_DataComplement(unsigned short data){
	unsigned short value = ~data + 1;
	return (value|0x8000);
} 

/*!
	@brief 		 Find maximum value and minimum value.
	@param[in]	 pdata_buff:Point to buff.
				 count:Buff count.
	@param[out]	 pmax_data:Point to maximum value.
				 pmin_data:Point to minimum value.
	@retval		 none
*/
void Find_MIN_MAX(int *pdata_buff,unsigned int count,int *pmax_data,int *pmin_data){
	*pmax_data = pdata_buff[0];
	*pmin_data = pdata_buff[0];
	for(unsigned int i = 0;i < count;i++){
		*pmax_data = *pmax_data<pdata_buff[i] ? pdata_buff[i] : *pmax_data;
		*pmin_data = *pmin_data>pdata_buff[i] ? pdata_buff[i] : *pmin_data;
	}
} 

/*!
	@brief 		 Move right range.
	@param[in]	 pdata_buff:Point to buff.
				 count:Buff count.
				 data:Need to save data.
	@param[out]	 pdata_buff:Point to buff.
	@retval		 ErrorStatus:....
*/
ErrorStatus MoveRight_Range(int *pdata_buff,unsigned short count,int data){
//	if(data - pdata_buff[count-1] > 2000 || data - pdata_buff[count-1] < -2000)
//		return ERROR;
	for(unsigned short i = 0;i < count-1;i++){
		pdata_buff[i] = pdata_buff[i+1];
	}
	pdata_buff[count-1] = data;
	return SUCCESS;
}

/*!
	@brief 		 Calculate the number of hours between two times.
	@param[in]	 pstart_time:Point to start time.	Year(From 2000) Month Day Hour
				 pstop_time:Point to stop time.		Year(From 2000) Month Day Hour
	@param[out]	 none
	@retval		 The number of hours.
*/
#define JUDGE_LEAPYEAR(year)	(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
unsigned int CALHour_BetweenTwoTime(unsigned char *pstart_time,unsigned char *pstop_time){
	unsigned int total_days = 0;
	unsigned int total_hours = 0;
	unsigned char year_type = 0;
	const unsigned char month_day[2][13] = {{0,31,28,31,30,31,30,31,31,30,31,30,31},	//Nonleap year
											{0,31,29,31,30,31,30,31,31,30,31,30,31}};	//Leap year
	unsigned short start_year = 2000 + pstart_time[0];
	unsigned short stop_year  = 2000 + pstop_time[0];
	for(unsigned short i = start_year+1;i < stop_year;i++){
		year_type = JUDGE_LEAPYEAR(i) ? 1 : 0;
		if(year_type == 1)
			total_days += 366;
		else
			total_days += 365;
	}
	if(start_year == stop_year){
		year_type = JUDGE_LEAPYEAR(start_year) ? 1 : 0;
		for(unsigned char i = pstart_time[1];i <= pstop_time[1];i++){
			total_days += month_day[year_type][i];
		}
	}
	else if(start_year < stop_year){
		year_type = JUDGE_LEAPYEAR(start_year) ? 1 : 0;
		for(unsigned short i = pstart_time[1];i <= 12;i++){
			total_days += month_day[year_type][i];
		}
		year_type = JUDGE_LEAPYEAR(stop_year) ? 1 : 0;
		for(unsigned short i = 1;i <= pstop_time[1];i++){
			total_days += month_day[year_type][i];
		}
	}
	else if(start_year > stop_year)
		return 0;
	year_type = JUDGE_LEAPYEAR(stop_year) ? 1 : 0;
	total_days -= ((pstart_time[2]-1) + (month_day[year_type][pstop_time[1]]-pstop_time[2]));
	total_hours = (total_days*24) - pstart_time[3] - (24-pstop_time[3]);
	return total_hours;
}

/*!
	@brief 		 Calculate the first derivative.
	@param[in]	 poriginaldata:Point to the original datas.	
				 count:Buff count.
	@param[out]	 poutdata:Point to the derivative datas.	
	@retval		 none.
*/
void Calculate_Derivative(int *poriginaldata,unsigned short count,int *poutdata){
	for(unsigned short i = 0;i < count-1;i++){
		poutdata[i] = poriginaldata[i+1] - poriginaldata[i];
	}
	poutdata[count-1] = poutdata[count-2];
}

/*!
	@brief 		 Calculate standard deviation.
	@param[in]	 poriginaldata:Point to the original datas.	
				 count:Buff count.
	@param[out]	 poutdata:Point to the derivative datas.	
	@retval		 none.
*/
int Calculate_STD(int *poriginaldata,unsigned short count){
	long int data_sum = 0;
	int mean = 0;
	for(unsigned short i = 0;i < count;i++)
		data_sum += poriginaldata[i];
	mean = data_sum/count;
	data_sum = 0;
	for(unsigned short i = 0;i < count;i++)
		data_sum += pow(poriginaldata[i]-mean,2);
	return sqrt(data_sum/count);
}


/*****					Delay functions(180MHz).							*****/
void udelay_1us(unsigned short count){
	for(unsigned short i = 0;i < count;i++)
		for(unsigned short j = 0;j < 28;j++)
			__NOP();
}

void udelay_100us(unsigned short count){
	for(unsigned short i = 0;i < count;i++)
		for(unsigned short j = 0;j < 2580;j++)
			__NOP();
}
