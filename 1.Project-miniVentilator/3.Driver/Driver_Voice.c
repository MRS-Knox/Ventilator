#include "Driver_Voice.h"

/*
    @brief        Voice GPIO Init
    @param[in]    none
    @param[out]   none
    @retval       none
*/
void Dri_Voice_GPIO_Init(){
	GPIO_InitTypeDef 	Struct_Voice_GPIO;	
	
	Struct_Voice_GPIO.GPIO_Pin 	 = Data_Pin; 
	Struct_Voice_GPIO.GPIO_Mode  = GPIO_Mode_OUT;
	Struct_Voice_GPIO.GPIO_Speed = GPIO_Speed_25MHz;
	Struct_Voice_GPIO.GPIO_OType = GPIO_OType_PP;
	Struct_Voice_GPIO.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(DataBusy_Group,&Struct_Voice_GPIO);
	Struct_Voice_GPIO.GPIO_Pin 	 = CAP_Pin; 
	GPIO_Init(CAP_Group,&Struct_Voice_GPIO);
	Struct_Voice_GPIO.GPIO_Pin 	 = Power_Pin; 
	GPIO_Init(Power_Group,&Struct_Voice_GPIO);
	Struct_Voice_GPIO.GPIO_Pin 	 = Busy_Pin; 
	Struct_Voice_GPIO.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_Init(DataBusy_Group,&Struct_Voice_GPIO);
	
	GPIO_WriteBit(Power_Group,Power_Pin,Bit_RESET);
	GPIO_WriteBit(CAP_Group,CAP_Pin,Bit_RESET);
	GPIO_WriteBit(DataBusy_Group,Data_Pin,Bit_SET);
}

/*
    @brief        Voice data port.
    @param[in]    data_level:....
    @param[out]   none
    @retval       none
*/
void Dri_Set_Data(BitAction data_level){
	GPIO_WriteBit(DataBusy_Group,Data_Pin,data_level);
}

/*
    @brief        Voice need a start sign.
    @param[in]    none
    @param[out]   none
    @retval       none
*/
void Mid_Voice_Start(){
    Dri_Set_Data(Bit_RESET);
	udelay_100us(50); //About 5ms
}

/*
    @brief        Voice need a stop sign.
    @param[in]    none
    @param[out]   none
    @retval       none
*/
void Mid_Voice_Stop(){
    Dri_Set_Data(Bit_SET);
	udelay_100us(20); //About 2ms.
}

/*
    @brief        Voice data set 1.
    @param[in]    none
    @param[out]   none
    @retval       none
*/
void Dri_Voice_DataOne(){
    Dri_Set_Data(Bit_SET);
    udelay_100us(6);      //About 600us
    Dri_Set_Data(Bit_RESET);
	udelay_100us(2);      //About 200us
}

/*
    @brief        Voice data set 0.
    @param[in]    none
    @param[out]   none
    @retval       none
*/
void Dri_Voice_DataZero(){
    Dri_Set_Data(Bit_SET);
	udelay_100us(2);      //About 200us
    Dri_Set_Data(Bit_RESET);
	udelay_100us(6);      //About 600us
}

/*
    @brief        Read voice busy level.
    @param[in]    none
    @param[out]   none
    @retval       busy_level:.....
*/
FlagStatus Dri_Get_VoiceBusy(){	
	FlagStatus busy_level = RESET;
	
	busy_level = (FlagStatus)GPIO_ReadInputDataBit(DataBusy_Group,Busy_Pin);
	busy_level = busy_level == SET ? RESET : SET;
	
	return busy_level;
}

/*
    @brief        Control capture IO set high.
    @param[in]    cap_level:....
    @param[out]   none
    @retval       none
*/
void Dri_Set_CAP(BitAction cap_level){
	GPIO_WriteBit(CAP_Group,CAP_Pin,cap_level);
}

/*
    @brief        Control power I/O set high.
    @param[in]    power_level:....
    @param[out]   none
    @retval       none
*/
void Dri_Set_Power(BitAction power_level){
	GPIO_WriteBit(Power_Group,Power_Pin,power_level);
}



