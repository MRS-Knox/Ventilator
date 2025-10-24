#ifndef Use_Struct_H
#define Use_Struct_H

#include "Use_Enum.h"
#include "stm32f4xx_conf.h"

#define MAXFLOWBUFF_COUNT				200		//4s -- 200 points(f = 50hz).
#define MACHINE_MINPRESS 				200		//2cmH2O	
#define MACHINE_MAXPRESS 				2500	//25cmH2O	
#define SINGLEMACHINERUN_MAXTIME		168		//168hour -- 1week
#define TOTALMACHINERUN_MAXTIME			100000	//100000h -- about 11year  

/* Serial port queue struct. */
#define UART_MAXBUFF	7
typedef struct{
	unsigned char buff_count;
	unsigned char buff[UART_MAXBUFF]; 			
}UARTQueue_t;

/* Blower state struct. */
typedef struct{
	unsigned int 	set_rpm; 	
	unsigned int 	actual_rpm; 	
	eBlower_State 	state;	 		
}Blower_State_t;

/* Machine runs parameters struct. */
typedef struct{	
	int diff_press;
	int flow_data;  					//0.01 lpm	
	long int flow_sum;
	int flow_mean;
	int flow_mean_5[5];
	int max_flow;
	int min_flow;

	short measure_p; 					//0.01 cmH2O
	unsigned short now_set_p;
	unsigned short now_run_p;
	unsigned short delay_end_p;
	unsigned short testmask_set_p;

	unsigned char  delayp_time; 		//The unit is minute.
	unsigned char  delayp_remaintime;	//Remaining time for delay increase pressure.
	
	unsigned short ins_time;			//1ms
	unsigned short ex_time;				//1ms
	unsigned char  ins_ex_scale[2];		//Inspiration : Expiration.Remain one decimal:0.1 -> 10.
	unsigned short bpm;					//Remain one decimal:0.1 -> 10.

	eBreathe_Stage breathe_stage;
}Run_Param_t;

/* Machine sets parameters struct. */
typedef struct{
	/* Therapy parameters. */
	eTherapy_Mode  mode;
	unsigned short  start_press;
	unsigned short  therapy_press;		//CPAP
	unsigned short  mintherapy_press;
	unsigned short  maxtherapy_press;
	unsigned char 	ajustpress_sense;	//Pressure ajustment sensetivity.
	FlagStatus 	    flag_leakage_warn;	//The big leakage warning.		
	// unsigned short  ipap;
	// unsigned short  epap;
	// unsigned char	slove_uppress;		//The slove of increase pressure.
	// unsigned char	ins_sensetivity;	//Inspiration sensetivity.
	// unsigned char	ex_sensetivity;		//Expiration sensetivity.
	
	/* Comfort parameters. */
	unsigned char delaypress_min; 	
	unsigned char epr;					
	FlagStatus 	  flag_auto_on;
	FlagStatus 	  flag_auto_off;
	unsigned char pipe_type;
	unsigned char mask_type;
	unsigned char mfc_type;				//Moisturizing filter core type.

	/* Maintance parameters. */
	unsigned char change_airfilter_day;	
	unsigned char change_mask_day;	
	unsigned char change_pipe_day;	
	unsigned char change_mfc_day;		//Change moisturizing filter core time.
	unsigned char clearmachine_day;		
}Set_Param_t;

/* Machine alarm. */
typedef struct{
	FlagStatus flag_highpri;
	FlagStatus flag_middlepri;
	FlagStatus flag_lowpri;
	
	FlagStatus flag_maskfall;			//Mask falls off.					High Priority
	FlagStatus flag_poweroff;			//Machine powers off.				High Priority
	FlagStatus flag_highleak;			//High leakage.						Middle Priority
	FlagStatus flag_changeairfilter;	//Change air filter.				Low Priority
	FlagStatus flag_changepipe;			//Change pipe.						Low Priority
	FlagStatus flag_changemask;			//Change mask.						Low Priority
	FlagStatus flag_changemfc;			//Change moisturize filter core.	Low Priority
	FlagStatus flag_maintenance;		//Maintenance.						Low Priority
}Alarm_State_t;

/* Machine state struct. */
typedef struct{
	FlagStatus flag_machine_onoff; 	
	FlagStatus flag_machine_switch;	
	FlagStatus flag_bluetooth_onoff;
	FlagStatus flag_bluetooth_switch;
	FlagStatus flag_alarmvoice_state;
	FlagStatus flag_delaypress;	
	FlagStatus flag_voicealarm_first;
	FlagStatus flag_judgeins_ex;
	FlagStatus flag_ajust_mask;

	FlagStatus flag_set_airfilter;		
	FlagStatus flag_set_mask;				
	FlagStatus flag_set_pipe;				
	FlagStatus flag_set_mfc;				
	FlagStatus flag_set_maintenance;		

	Alarm_State_t Alarm_State;
}Machine_State_t;

/* Machine time. */
typedef struct{
	unsigned short 	push_btkey_time_10ms;
	unsigned int  	basetime_10ms; 
	unsigned int  	machine_runtime_10ms; 
	unsigned short  machine_runtime_s;
	unsigned short  machine_runtime_min;
	unsigned short  machine_runtime_hour;
	unsigned int  	machine_totalruntime_hour;
	unsigned short 	delaypresstime_1s; 

	/* Record the usage time of consumables. */
	unsigned int	airfilter_warn_hour;		
	unsigned int 	mask_warn_hour;		
	unsigned int 	pipe_warn_hour;		
	unsigned int 	mfc_warn_hour;			//Moisturize filter core.		
	unsigned int 	maintenace_warn_hour;	//Maintenace.
}Machine_Time_t;

/* Calibrate pressure and flow struct. */
#define CALFlowMaxCount 	7
#define CALFlow_KMaxCount 	6
#define CALPRESSMaxCount 	6
typedef struct{
	unsigned short calpress_buff[CALPRESSMaxCount]; //5 10 15 20 25 30
	unsigned short press_compensation;
	unsigned char  calpress_count;

	unsigned short k[CALFlow_KMaxCount];    		//k*100. < 65535. 		
	unsigned int  calflow_buff[CALFlowMaxCount];	//difference pressure*100. 0 40 60 80 100 120 140
	unsigned char calflow_count;

	unsigned short flag_rtc_init;					//RTC intialization.
	unsigned short machine_model;	
}CalibrationData_t;

/* Increase pid struct. */
typedef struct{
	int e;
	int max_e;
	int min_e;
	int ec;
	int last_e1;
	int last_e2;
	int kd_e;
	int present_output;
	int max_output;
	int min_output;
	int last_output;
	int total_output;
	float kp;
	float ki;
	float kd; 
}IncreasePID_t;

/* Kalman filter. */
typedef struct{
	float x_last;	//x with last time 
	float p_last;	//p with last time 
	float q;		//system process error
	float r;		//measure error
}KalmanFilter_t;

/* Calibration flow and pressure. */
typedef struct{
	eMachine_Model machinemodel;
	FlagStatus flag_recordmodel;	//Record rtc time.	
	unsigned char rtc_time[8];		//Year,Month,Day,Week,Hour,Minute,Second,AM or PM.
	FlagStatus flag_recordrtc;		//Record rtc time.	
	// FlagStatus calibrate_start;	
	// FlagStatus calibrate_stop;	
	FlagStatus flag_stopblower;		//Stop blower.
	FlagStatus flag_uprpm;			//Increase rpm.
	FlagStatus flag_rerpm;			//Reduce rpm.
	FlagStatus flag_califlow; 		//flow -> difference pressure.
	FlagStatus flag_calipress; 		//pressure -> rpm.
	unsigned short inrpm_interval;	//Increase rpm interval. 
	unsigned short rerpm_interval;	//Reduce rpm interval. 
}Calibration_t;

/* Set warning of consumables.The unit is day. */
typedef struct{
	unsigned char set_airfilter_date[4];		//Year(From 2000) Month Day Hour
	unsigned char set_mask_date[4];				//Year(From 2000) Month Day Hour
	unsigned char set_pipe_date[4];				//Year(From 2000) Month Day Hour
	unsigned char set_mfc_date[4];				//Year(From 2000) Month Day Hour
	unsigned char set_maintenance_date[4];		//Year(From 2000) Month Day Hour
	
	unsigned char airfilter_warn_day;		
	unsigned char mask_warn_day;		
	unsigned char pipe_warn_day;		
	unsigned char mfc_warn_day;					//Moisturize filter core.		
	unsigned char clearmachine_warn_day;		//Clear machine.
}SetConsumable_Warn_t;



/* Struct declaration. */
extern Blower_State_t			Blower_State;		//Record blower state.
extern Run_Param_t				Run_Param;			//Record run-data.
extern Set_Param_t				Set_Param;			//Record set-data.
extern CalibrationData_t		CalibrationData;	//Record calibration data.
extern UARTQueue_t				BlowerSendQueue;	//Blower send queue.
extern UARTQueue_t				BlowerReceiveQueue;	//Blower receive queue.
extern Machine_State_t			Machine_State;		//Record machine state.
extern Machine_Time_t			Machine_Time;		//Record machine time.
extern IncreasePID_t			IncreasePID;		//PID parameters.
extern KalmanFilter_t			Flow_Kalman;		//Flow filter.
extern Calibration_t			Calibration;		//Calibration struct.
extern SetConsumable_Warn_t    	SetConsumable_Warn;	//Set warning period of consumable.

#endif 
