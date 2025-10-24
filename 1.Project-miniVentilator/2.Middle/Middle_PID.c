#include "Middle_PID.h"

#define E_CHANGE_PID		300
#define DEAD_ZONE			100			

#define P_PRESS_IN			1.8		//1.8
#define I_PRESS_IN			0.20	//0.23
#define D_PRESS_IN			0.10	//0.20

#define P_PRESS_RE			1.9    	//1.5
#define I_PRESS_RE			0.25	//0.25
#define D_PRESS_RE			0.20 	//0.3

#define P_PRESS_KEEP		1.0		//1.0
#define I_PRESS_KEEP		0.12    //0.1
#define D_PRESS_KEEP		0.5		//0.01

/*!
	@brief 		 Increase PID algorithm intial parameter function. 
	@param[in]	 none
	@param[out]	 none
	@retval		 none
*/
void Mid_IncreasePID_Init(){
	IncreasePID.ec 				= 0;
	IncreasePID.e 				= 0;
	IncreasePID.max_e 			= 1800;		//1500
	IncreasePID.min_e 			= -1800;	//-1500
	IncreasePID.last_e1 		= 0;
	IncreasePID.last_e2 		= 0;
	IncreasePID.kd_e 			= 0;
	IncreasePID.present_output 	= 0;
	IncreasePID.max_output 		= 8000;
	IncreasePID.min_output 		= -8000;
	IncreasePID.last_output 	= START_RPM;
	IncreasePID.total_output 	= 0;
	IncreasePID.kp 				= P_PRESS_IN;
	IncreasePID.ki 				= I_PRESS_IN;
	IncreasePID.kd 				= D_PRESS_IN; 
}

/*!
	@brief 		 Increase PID algorithm function. 
	@param[in]	 setpoint:Target pressure.
				 measure_value:Real-time pressure.
	@param[out]	 none
	@retval		 none
*/
void Mid_IncreasePID(uint16_t target_press,uint16_t measure_press){
	uint16_t target_rpm = Mid_CalBlower_RPM(target_press);
	uint16_t real_rpm   = Mid_CalBlower_RPM(measure_press);

	/* Update data. */	
	IncreasePID.e  	= target_rpm - real_rpm;
	IncreasePID.e 	= IncreasePID.e>IncreasePID.max_e ? IncreasePID.max_e : IncreasePID.e;
	IncreasePID.e 	= IncreasePID.e<IncreasePID.min_e ? IncreasePID.min_e : IncreasePID.e;
	IncreasePID.ec 	= IncreasePID.e - IncreasePID.last_e1;
	IncreasePID.kd_e= IncreasePID.e - 2*IncreasePID.last_e1 + IncreasePID.last_e2;
	
	/* Update parameter. */	
	if(IncreasePID.e > E_CHANGE_PID){
		IncreasePID.kp 				= P_PRESS_IN;
		IncreasePID.ki 				= I_PRESS_IN;
		IncreasePID.kd 				= D_PRESS_IN;
	}
	else if(IncreasePID.e < -E_CHANGE_PID){
		IncreasePID.kp 				= P_PRESS_RE;
		IncreasePID.ki 				= I_PRESS_RE;
		IncreasePID.kd 				= D_PRESS_RE;
	}
	else if(IncreasePID.e <= E_CHANGE_PID && IncreasePID.e >= -E_CHANGE_PID){
		IncreasePID.kp 				= P_PRESS_KEEP;
		IncreasePID.ki 				= I_PRESS_KEEP;
		IncreasePID.kd 				= D_PRESS_KEEP;
		if(IncreasePID.e <= DEAD_ZONE && IncreasePID.e >= -DEAD_ZONE)
			return;
	}
	
	/* Calculate change rpm. */	
	IncreasePID.present_output = IncreasePID.kp * IncreasePID.ec + IncreasePID.ki * IncreasePID.e + IncreasePID.kd * IncreasePID.kd_e;
	IncreasePID.present_output = IncreasePID.present_output>IncreasePID.max_output ? IncreasePID.max_output : IncreasePID.present_output;
	IncreasePID.present_output = IncreasePID.present_output<IncreasePID.min_output ? IncreasePID.min_output : IncreasePID.present_output;
	IncreasePID.total_output   = IncreasePID.present_output + IncreasePID.last_output;
	
	/* Limit output rpm. */	
	IncreasePID.total_output = IncreasePID.total_output >= MOTOR_SPEED_MAX ? MOTOR_SPEED_MAX : IncreasePID.total_output;
	IncreasePID.total_output = IncreasePID.total_output <= MOTOR_SPEED_MIN ? MOTOR_SPEED_MIN : IncreasePID.total_output;
	Mid_SendToBlower(SEND_RPM,IncreasePID.total_output);
	
	/* Update data. */	
	IncreasePID.last_e2 = IncreasePID.last_e1;
	IncreasePID.last_e1 = IncreasePID.e;
	IncreasePID.last_output = IncreasePID.total_output;
}






